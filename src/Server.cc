#include <folly/init/Init.h>
#include <glog/logging.h>
#include <thrift/lib/cpp2/server/ThriftServer.h>

#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "Service.h"
#include "Utils.h"

using namespace Sigsegv::Personalsite;

bool savepid() {
    std::string root {"/opt/PersonalSite"};
    std::string pidfile {".pid"};
    DIR* dir = opendir(root.c_str());
    if (!dir) {
        int retval = mkdir(root.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        if (retval !=0) {
            return false;
        }
    } else {
        closedir(dir);
    }
    std::ofstream ofs {root + "/"+ pidfile};
    if (!ofs) {
        return false;
    }
    ofs << getpid() << "\n";
    ofs.close();
    return true;
}

int main (int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Please provide path to auth config file and the whitelist file.\n";
        return 1;
    }
    folly::init (&argc, &argv, true);

    // Save pid and other initializations
    if (!savepid()) {
        std::cout << "Could not save pid file.\n";
        return 128;
    }
    Sigsegv::Personalsite::Utils::initAws ();

    // Set up logging
    Sigsegv::Personalsite::Utils::initAwsLogging ("PersonalSiteServer", "/var/log/PersonalSite_aws_sdk_");
    google::SetLogDestination (0, "/var/log/PersonalSite.log");
    google::SetLogDestination (google::WARNING, "");
    // FLAGS_logtostderr = 0; // Meant as an override

    // Open AuthConfig file
    std::string authConfig (argv[1]);
    std::string whitelist (argv[2]);
    std::ifstream authIfs (authConfig, std::ifstream::binary);
    std::ifstream whitelistIfs (whitelist, std::ifstream::binary);

    // Initialize server
    std::shared_ptr<apache::thrift::ServerInterface> s = std::make_shared<Sigsegv::Personalsite::Frontend> (authIfs, whitelistIfs);
    auto server = folly::make_unique<apache::thrift::ThriftServer> ();
    server->setInterface (s);
    server->setPort (9090);
    server->serve ();

    Sigsegv::Personalsite::Utils::shutDownAws ();
}
