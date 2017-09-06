#ifndef PERSONAL_SITE_FE_SERVICE
#define PERSONAL_SITE_FE_SERVICE

#include <thrift/lib/cpp2/server/ThriftServer.h>
#include <folly/init/Init.h>

#include "PersonalSiteFrontend.h"

using namespace personalsite::frontend;

namespace personalsite {
    namespace frontend {
        class PersonalSiteFrontend : public PersonalSiteFrontendSvIf {
        public:
            PersonalSiteFrontend();
            virtual ~PersonalSiteFrontend();
            void getUidFromToken(GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request);
        };
    }
}

#endif
