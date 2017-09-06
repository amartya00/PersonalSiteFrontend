#include <thrift/lib/cpp2/server/ThriftServer.h>
#include <folly/init/Init.h>
#include <glog/logging.h>

#include "Service.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    folly::init(&argc, &argv, true);

    personalsite::utils::initAwsLogging("PersonalSiteServer", "/var/log/PersonalSite_aws_sdk_");
    google::SetLogDestination(0,"/var/log/PersonalSite.log");
    google::SetLogDestination(google::WARNING,"");

    //FLAGS_logtostderr = 0; // Meant as an override

    std::shared_ptr<apache::thrift::ServerInterface> s = std::make_shared<PersonalSiteFrontend>();
    auto server = folly::make_unique<apache::thrift::ThriftServer>();
    server->setInterface(s);
    server->setPort(9090);
    server->serve();
}
