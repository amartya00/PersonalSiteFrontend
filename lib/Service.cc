#include "PersonalSiteFrontend.h"

#include "Auth.h"
#include "Exceptions.h"
#include "Service.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace personalsite::frontend;

personalsite::frontend::PersonalSiteFrontend::PersonalSiteFrontend (std::ifstream& authConfig) {
    authService = std::make_unique<personalsite::Auth> (authConfig);
}

personalsite::frontend::PersonalSiteFrontend::~PersonalSiteFrontend () {
}

void personalsite::frontend::PersonalSiteFrontend::getUidFromToken (GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request) {
    LOG (INFO) << "getUidFromToken called\n";
    personalsite::AuthUserItem retval;

    try {
        retval = authService->getUserInfo (request->get_token ());
    } catch (const personalsite::exceptions::ServiceException& e) {
        LOG (ERROR) << e.getMessage ();
        return;
    }
    response.set_uid (retval.getUid ());
    response.set_name (retval.getName ());
    response.set_pwd (retval.getPwd ());
}
