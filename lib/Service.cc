#include "Frontend.h"

#include "Auth.h"
#include "Exceptions.h"
#include "Service.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace Sigsegv::Personalsite;

Sigsegv::Personalsite::Frontend::Frontend (std::ifstream& authConfig) {
    authService = std::make_unique<Sigsegv::Personalsite::Auth::Service> (authConfig);
}

Sigsegv::Personalsite::Frontend::~Frontend () {
}

void Sigsegv::Personalsite::Frontend::getUidFromToken (GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request) {
    LOG (INFO) << "getUidFromToken called\n";
    Sigsegv::Personalsite::Auth::UserItem retval;

    try {
        retval = authService->getUserInfo (request->get_token ());
    } catch (const Sigsegv::Personalsite::Exceptions::ServiceException& e) {
        LOG (ERROR) << e.getMessage ();
        return;
    } catch (const std::exception& e) {
        LOG (ERROR) << e.what ();
        return;
    }

    response.set_uid (retval.getUid ());
    response.set_name (retval.getName ());
    response.set_pwd (retval.getPwd ());
}
