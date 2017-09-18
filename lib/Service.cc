#include "Frontend.h"
#include "Service_types.h"

#include "Auth.h"
#include "Exceptions.h"
#include "Service.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace Sigsegv::Personalsite;

Sigsegv::Personalsite::Frontend::Frontend (std::ifstream& authConfig, std::ifstream& whitelist) {
    authService = std::make_unique<Sigsegv::Personalsite::Auth::Service> (authConfig, whitelist);
}

Sigsegv::Personalsite::Frontend::~Frontend () {
}

void Sigsegv::Personalsite::Frontend::getUidFromToken (GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request) {
    LOG (INFO) << "getUidFromToken called\n";
    Sigsegv::Personalsite::Auth::UserItem retval;
    Sigsegv::Personalsite::ServiceException sExc;
    Sigsegv::Personalsite::ClientException cExc;

    try {
        retval = authService->getUserInfo (request->get_token ());
    } catch (const Sigsegv::Personalsite::Exceptions::ServiceException& e) {
        LOG (ERROR) << e.getMessage ();
        sExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::SERVICE_UNAVAILABLE);
        sExc.set_errorMessage("Service currently unavailable. Please try again later.");
        throw sExc;
    } catch (const Sigsegv::Personalsite::Exceptions::ClientException& e) {
        if (e.getErrorCode() == Sigsegv::Personalsite::Exceptions::ExceptionType::EXPIRED_TOKEN) {
            cExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::EXPIRED_TOKEN);
            cExc.set_errorMessage("Your oauth token has expired. Please sign in again.");
        } else if (e.getErrorCode() == Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_TOKEN) {
            cExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::INCORRECT_TOKEN);
            cExc.set_errorMessage("Your oauth token is incorrect. Something went wrong while you signed in.");
        }
        throw cExc;
    } catch (const std::exception& e) {
        LOG (ERROR) << e.what ();
        return;
    }

    response.set_uid (retval.getUid ());
    response.set_name (retval.getName ());
    response.set_pwd (retval.getPwd ());
}
