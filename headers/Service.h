#ifndef PERSONAL_SITE_FE_SERVICE
#define PERSONAL_SITE_FE_SERVICE

#include <folly/init/Init.h>
#include <thrift/lib/cpp2/server/ThriftServer.h>

#include <fstream>
#include <string>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>

#include "Auth.h"
#include "Frontend.h"

using namespace Sigsegv::Personalsite;

namespace Sigsegv {
    namespace Personalsite {
        class Frontend : public FrontendSvIf {
            private:
            static const std::string SERVICE_NAME;

            std::unique_ptr<Sigsegv::Auth::Service> authService;

            public:
            Frontend (std::ifstream& authConfig, const std::shared_ptr<Aws::DynamoDB::DynamoDBClient>& ddbClient, const std::shared_ptr<Sigsegv::Auth::GapiWrapper> gapiClient);
            virtual ~Frontend ();
            void getUidFromToken (GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request);
        };
    }
}

#endif
