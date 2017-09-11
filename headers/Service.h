#ifndef PERSONAL_SITE_FE_SERVICE
#define PERSONAL_SITE_FE_SERVICE

#include <folly/init/Init.h>
#include <thrift/lib/cpp2/server/ThriftServer.h>

#include <fstream>

#include "Auth.h"
#include "Frontend.h"

using namespace Sigsegv::Personalsite;

namespace Sigsegv {
    namespace Personalsite {
        class Frontend : public FrontendSvIf {
            private:
            std::unique_ptr<Sigsegv::Personalsite::Auth::Service> authService;

            public:
            Frontend (std::ifstream& authConfig);
            virtual ~Frontend ();
            void getUidFromToken (GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request);
        };
    }
}

#endif
