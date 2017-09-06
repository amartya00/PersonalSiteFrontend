#include "PersonalSiteFrontend.h"

#include "Service.h"
#include "Auth.h"


#include <iostream>

using namespace personalsite::frontend;

personalsite::frontend::PersonalSiteFrontend::PersonalSiteFrontend() {

}

personalsite::frontend::PersonalSiteFrontend::~PersonalSiteFrontend() {

}

void personalsite::frontend::PersonalSiteFrontend::getUidFromToken(GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request) {
    LOG(INFO) << "getUidFromToken called\n";
    response.set_uid(personalsite::auth::getUidFromToken(request->get_token()));
}
