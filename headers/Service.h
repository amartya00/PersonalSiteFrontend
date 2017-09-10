#ifndef PERSONAL_SITE_FE_SERVICE
#define PERSONAL_SITE_FE_SERVICE

#include <folly/init/Init.h>
#include <thrift/lib/cpp2/server/ThriftServer.h>

#include <fstream>

#include "Auth.h"
#include "PersonalSiteFrontend.h"

using namespace personalsite::frontend;

namespace personalsite {
    namespace frontend {
        class PersonalSiteFrontend : public PersonalSiteFrontendSvIf {
            private:
            std::unique_ptr<personalsite::Auth> authService;

            public:
            PersonalSiteFrontend (std::ifstream& authConfig);
            virtual ~PersonalSiteFrontend ();
            void getUidFromToken (GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request);
        };
    }
}

#endif
