#include <folly/init/Init.h>
#include <glog/logging.h>
#include <thrift/lib/cpp/transport/TSSLServerSocket.h>
#include <thrift/lib/cpp/transport/TTransport.h>
#include <thrift/lib/cpp2/server/ThriftServer.h>

#include <csignal>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>

#include "Auth.h"
#include "Service.h"
#include "Utils.h"

using namespace Sigsegv::Personalsite;

bool savepid () {
    std::string root{ "/opt/PersonalSite" };
    std::string pidfile{ ".pid" };
    DIR* dir = opendir (root.c_str ());
    if (!dir) {
        int retval = mkdir (root.c_str (), S_IRUSR | S_IWUSR | S_IXUSR);
        if (retval != 0) {
            return false;
        }
    } else {
        closedir (dir);
    }
    std::ofstream ofs{ root + "/" + pidfile };
    if (!ofs) {
        return false;
    }
    ofs << getpid () << "\n";
    ofs.close ();
    return true;
}

void signalHandler (int signal) {
    std::cout << "\n\nShutting down server ...\n\n";
    LOG (INFO) << "Shutting down server\n";
    Sigsegv::Personalsite::Utils::shutDownAws ();
    exit (EXIT_SUCCESS);
}

int main (int argc, char* argv[]) {
    std::signal (SIGINT, signalHandler);
    std::signal (SIGKILL, signalHandler);
    std::signal (SIGABRT, signalHandler);
    if (argc < 4) {
        std::cerr << "Please provide 1. path to auth config file, 2. Certificate, 3. Private key.\n";
        return 1;
    }
    folly::init (&argc, &argv, true);

    // Save pid and other initializations
    if (!savepid ()) {
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
    std::ifstream authIfs (authConfig, std::ifstream::binary);

    // Cert and pvt key
    std::string cert (argv[2]);
    std::string keypair (argv[3]);
    auto sslConfig = std::make_shared<wangle::SSLContextConfig> ();
    sslConfig->sessionContext = "Personalsite";
    sslConfig->sessionCacheEnabled = false;
    sslConfig->isDefault = true;
    sslConfig->setNextProtocols ({ "h2" });
    sslConfig->setCertificate (cert, keypair, "");

    // Initialize server
    std::shared_ptr<Aws::DynamoDB::DynamoDBClient> ddbClient = std::make_shared<Aws::DynamoDB::DynamoDBClient> ();
    std::shared_ptr<Sigsegv::Auth::GapiWrapper> gapiClient = std::make_shared<Sigsegv::Auth::GapiWrapper> ();
    std::shared_ptr<apache::thrift::ServerInterface> s = std::make_shared<Sigsegv::Personalsite::Frontend> (authIfs, ddbClient, gapiClient);

    auto server = folly::make_unique<apache::thrift::ThriftServer> ();
    server->setInterface (s);
    server->setPort (9090);
    server->setSSLConfig (sslConfig);
    server->serve ();

    Sigsegv::Personalsite::Utils::shutDownAws ();
}
