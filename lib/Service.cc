#include "Frontend.h"
#include "Service_types.h"

#include "Auth.h"
#include "Exceptions.h"
#include "PersonalSiteExceptions.h"
#include "Service.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <string>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>
#include <aws/dynamodb/model/PutItemRequest.h>


#include <json/json.h>

using namespace Sigsegv::Personalsite;

const std::string Sigsegv::Personalsite::Frontend::SERVICE_NAME {"Personalsite"};

Sigsegv::Personalsite::Frontend::Frontend (std::ifstream& authConfig, const std::shared_ptr<Aws::DynamoDB::DynamoDBClient>& ddbClient, const std::shared_ptr<Sigsegv::Auth::GapiWrapper> gapiClient) {
    authService = std::make_unique<Sigsegv::Auth::Service> (ddbClient, authConfig, gapiClient);
    this->ddbClient = ddbClient;
    Json::Value jsonObj;
    authConfig.seekg(0, std::ios::beg);
    authConfig >> jsonObj;
    userTable = jsonObj["UserTable"].asString();
    if (userTable.compare("") == 0) {
        Sigsegv::Personalsite::Exceptions::ServiceException sexc;
        sexc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_CONFIG);
        sexc.setMessage("User table not found");
        throw sexc;
    }
}

Sigsegv::Personalsite::Frontend::~Frontend () {
}

Sigsegv::Personalsite::UserItem Sigsegv::Personalsite::Frontend::getUserItem(const std::string& uid) {
    Sigsegv::Personalsite::UserItem item;
    if (uid.compare("") == 0) {
        Sigsegv::Personalsite::Exceptions::ClientException exc;
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_INPUT);
        exc.setMessage(Sigsegv::Personalsite::Utils::toStdString("uid cannot be null!!"));
        throw exc;
    }
    item.setUid(uid);
    auto result = ddbClient->Query(
                Aws::DynamoDB::Model::QueryRequest()
                    .WithTableName(Sigsegv::Personalsite::Utils::toAwsString(userTable))
                    .WithKeyConditionExpression(Sigsegv::Personalsite::Utils::toAwsString(item.toKeyConditionExpression()))
                    .WithExpressionAttributeValues(item.toKeyConditionAttributeValues()));
    if (!result.IsSuccess()) {
        Sigsegv::Personalsite::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INTERNAL_FAILURE);
        exc.setMessage(Sigsegv::Personalsite::Utils::toStdString(result.GetError().GetMessage()));
        throw exc;
    }

    // Reset the uid. If nothing is found in ddb, I want to return an empty user item
    item.setUid("");
    auto returnedItems = result.GetResult().GetItems();
    if (returnedItems.size() != 0) {
        auto returnedItem = returnedItems[0];
        item.fromAttributeMap(returnedItem);
        return item;
    }
    return item;
}

void Sigsegv::Personalsite::Frontend::putUserItem(const UserItem& item) {
    Sigsegv::Personalsite::Exceptions::ClientException cexc;
    if (item.getUid().compare("") == 0) {
        cexc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("Uid required while trying to save user data");
        throw cexc;
    }
    if (item.getName().compare("") == 0) {
        cexc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("Name required while trying to save user data");
        throw cexc;
    }
    auto response = ddbClient->PutItem(
                Aws::DynamoDB::Model::PutItemRequest()
                    .WithTableName(Sigsegv::Personalsite::Utils::toAwsString(userTable))
                    .WithItem(item.toAttributeMap()));

    if (!response.IsSuccess()) {
        Sigsegv::Personalsite::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INTERNAL_FAILURE);
        exc.setMessage(Sigsegv::Personalsite::Utils::toStdString(response.GetError().GetMessage()));
        throw exc;
    }
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

    // If the current user entry does not exist, create it.
    try {
        auto userItem = getUserItem(retval.getUid());
        if (userItem.getUid().compare("") == 0) {
            userItem.setUid(retval.getUid());
            userItem.setName(retval.getName());
            putUserItem(userItem);
        }
    } catch (const Sigsegv::Personalsite::Exceptions::ServiceException& e) {
        sExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::SERVICE_UNAVAILABLE);
        sExc.set_errorMessage(e.getMessage());
        throw sExc;
    }

    response.set_uid (retval.getUid ());
    response.set_name (retval.getName ());
    response.set_pwd (retval.getPwd ());
}

void Sigsegv::Personalsite::Frontend::getUserInfo(GetUserInfoResponse& response, std::unique_ptr<GetUserInfoRequest> request) {
    LOG (INFO) << "Operation = GetUserInfo";

    Sigsegv::Personalsite::ServiceException sExc;
    Sigsegv::Personalsite::ClientException cExc;
    bool isAuthorized = false;

    try {
        isAuthorized = authService->authorizeUser(request->get_uid(), request->get_pwd(), Sigsegv::Personalsite::Frontend::SERVICE_NAME);
    } catch (const Sigsegv::Auth::Exceptions::ClientException& e) {
        cExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::AUTH_EXCEPTION);
        cExc.set_errorMessage(e.getMessage());
        throw cExc;
    } catch (const std::exception& e) {
        LOG (ERROR) << e.what ();
        throw e;
    }
    if (!isAuthorized) {
        cExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::AUTH_EXCEPTION);
        cExc.set_errorMessage("User not authorized to perform this operation.");
        throw cExc;
    }

    Sigsegv::Personalsite::UserItem item = getUserItem(request->get_uid());
    if (item.getUid().compare("") == 0 || item.getName().compare("") == 0) {
        sExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::AUTH_EXCEPTION);
        sExc.set_errorMessage("Unexpected: User item not found in records.");
        throw sExc;
    }
    response.set_name(item.getName());
    response.set_isRsvpDay1(item.getRsvp("Day1"));
    response.set_isRsvpDay2(item.getRsvp("Day2"));
}

void Sigsegv::Personalsite::Frontend::updateUserInfo(UpdateUserInfoResponse &response, std::unique_ptr<UpdateUserInfoRequest> request) {
    LOG (INFO) << "Operation = UpdateUserInfo";

    Sigsegv::Personalsite::ServiceException sExc;
    Sigsegv::Personalsite::ClientException cExc;
    bool isAuthorized = false;

    try {
        isAuthorized = authService->authorizeUser(request->get_uid(), request->get_pwd(), Sigsegv::Personalsite::Frontend::SERVICE_NAME);
    } catch (const Sigsegv::Auth::Exceptions::ClientException& e) {
        cExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::AUTH_EXCEPTION);
        cExc.set_errorMessage(e.getMessage());
        throw cExc;
    } catch (const std::exception& e) {
        LOG (ERROR) << e.what ();
        throw e;
    }

    Sigsegv::Personalsite::UserItem item;
    // See if user exists
    try {
        auto userItem = getUserItem(request->get_uid());
        if (userItem.getUid().compare("") == 0) {
            // Does not exist
            if (request->get_name().compare("") == 0) {
                cExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::INVALID_INPUT);
                cExc.set_errorMessage("User does not exist. Name is needed as input in such a case.");
            } else {
                item.setName(request->get_name());
            }
        } else {
            if (request->get_name().compare("") != 0) {
                item.setName(request->get_name());
            } else {
                item.setName(userItem.getName());
            }
        }
        item.setRsvp("Day1", request->get_isRsvpDay1());
        item.setRsvp("Day2", request->get_isRsvpDay2());
        item.setUid(request->get_uid());
        putUserItem(item);
    } catch (const Sigsegv::Personalsite::Exceptions::ServiceException& e) {
        sExc.set_errorCode(Sigsegv::Personalsite::ErrorCode::SERVICE_UNAVAILABLE);
        sExc.set_errorMessage(e.getMessage());
        throw sExc;
    }
}
