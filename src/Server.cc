#include <folly/init/Init.h>
#include <glog/logging.h>
#include <thrift/lib/cpp2/server/ThriftServer.h>

#include <fstream>
#include <string>

#include "Service.h"
#include "Utils.h"

using namespace Sigsegv::Personalsite;

int main (int argc, char* argv[]) {
    folly::init (&argc, &argv, true);
    Sigsegv::Personalsite::Utils::initAws ();

    // Set up logging
    Sigsegv::Personalsite::Utils::initAwsLogging ("PersonalSiteServer", "/var/log/PersonalSite_aws_sdk_");
    google::SetLogDestination (0, "/var/log/PersonalSite.log");
    google::SetLogDestination (google::WARNING, "");
    // FLAGS_logtostderr = 0; // Meant as an override

    // Open AuthConfig file
    std::string authConfig (argv[1]);
    std::ifstream authIfs (authConfig, std::ifstream::binary);

    // Initialize server
    std::shared_ptr<apache::thrift::ServerInterface> s = std::make_shared<Sigsegv::Personalsite::Frontend> (authIfs);
    auto server = folly::make_unique<apache::thrift::ThriftServer> ();
    server->setInterface (s);
    server->setPort (9090);
    server->serve ();

    Sigsegv::Personalsite::Utils::shutDownAws ();
}
