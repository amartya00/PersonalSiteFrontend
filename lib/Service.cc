#include "Frontend.h"
#include "Service_types.h"

#include "Auth.h"
#include "Exceptions.h"
#include "PersonalSiteExceptions.h"
#include "Service.h"

#include <fstream>
#include <iostream>
#include <string>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>

using namespace Sigsegv::Personalsite;

const std::string Sigsegv::Personalsite::Frontend::SERVICE_NAME {"Personalsite"};

Sigsegv::Personalsite::Frontend::Frontend (std::ifstream& authConfig, const std::shared_ptr<Aws::DynamoDB::DynamoDBClient>& ddbClient, const std::shared_ptr<Sigsegv::Auth::GapiWrapper> gapiClient) {
    authService = std::make_unique<Sigsegv::Auth::Service> (ddbClient, authConfig, gapiClient);
}

Sigsegv::Personalsite::Frontend::~Frontend () {
}

void Sigsegv::Personalsite::Frontend::getUidFromToken (GetUidFromTokenResponse& response, std::unique_ptr<GetUidFromTokenRequest> request) {
    LOG (INFO) << "getUidFromToken called\n";
    Sigsegv::Auth::UserItem retval;
    Sigsegv::Personalsite::ServiceException sExc;
    Sigsegv::Personalsite::ClientException cExc;

    try {
        retval = authService->signinOrRegister(request->get_token(), SERVICE_NAME);
    } catch (const Sigsegv::Auth::Exceptions::ServiceException& e) {
        LOG (ERROR) << e.getMessage ();
        sExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::SERVICE_UNAVAILABLE);
        sExc.set_errorMessage(e.getMessage());
        throw sExc;
    } catch (const Sigsegv::Auth::Exceptions::ClientException& e) {
        if (e.getErrorCode() == Sigsegv::Auth::Exceptions::ExceptionType::EXPIRED_TOKEN) {
            cExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::EXPIRED_TOKEN);
            cExc.set_errorMessage("Your oauth token has expired. Please sign in again.");
        } else if (e.getErrorCode() == Sigsegv::Auth::Exceptions::ExceptionType::INVALID_TOKEN) {
            cExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::INCORRECT_TOKEN);
            cExc.set_errorMessage("Your oauth token is incorrect. Something went wrong while you signed in.");
        }
        throw cExc;
    } catch (const std::exception& e) {
        LOG (ERROR) << e.what ();
        throw e;
    }

    response.set_uid (retval.getUid ());
    response.set_name (retval.getName ());
    response.set_pwd (retval.getPwd ());
}
