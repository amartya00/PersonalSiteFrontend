#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <sstream>
#include <string>
#include<vector>
#include <functional>
#include <string>
#include <ctime>
#include <fstream>

#include <json/json.h>
#include <curl/curl.h>
#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>
#include <aws/dynamodb/model/PutItemRequest.h>

personalsite::Auth::Auth(std::ifstream& configInputStream) {
    Json::Value jsonObj;
    configInputStream >> jsonObj;
    ddbtable = jsonObj["DDBTableName"].asString();
    if (ddbtable.compare("") == 0) {
        personalsite::exceptions::ServiceException exc;
        exc.setErrorCode(personalsite::exceptions::INVALID_CONFIG);
        exc.setMessage("DDBTableName not found in auth config");
        throw exc;
    }
}

size_t personalsite::Auth::writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, realsize);
    return realsize;
}

personalsite::AuthUserItem personalsite::Auth::getInfoFromOAuth(const std::string &accessToken) const {
    personalsite::AuthUserItem retval;
    std::string url = "https://www.googleapis.com/plus/v1/people/me?access_token=" + accessToken;
    personalsite::exceptions::ServiceException exc;
    std::string returned;

    // Set up and make calls with curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        exc.setErrorCode(personalsite::exceptions::CURL_INIT_ERROR);
        exc.setMessage("Failed to initialize curl.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    auto response = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (response != CURLE_OK) {
        exc.setErrorCode(personalsite::exceptions::CURL_SETOPT_ERROR_CURLOPT_URL);
        exc.setMessage("Failed to set curl url.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (response != CURLE_OK) {
        exc.setErrorCode(personalsite::exceptions::CURL_SETOPT_ERROR_CURLOPT_FOLLOWLOCATION);
        exc.setMessage("Failed to set curl follow location.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    if (response != CURLE_OK) {
        exc.setErrorCode(personalsite::exceptions::CURL_SETOPT_ERROR_CURLOPT_WRITEFUNCTION);
        exc.setMessage("Failed to set curl writeback function.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&returned);
    if (response != CURLE_OK) {
        exc.setErrorCode(personalsite::exceptions::CURL_SETOPT_ERROR_CURLOPT_WRITEDATA);
        exc.setMessage("Failed to set curl writeback data.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_perform(curl);
    if (response != CURLE_OK) {
        exc.setErrorCode(personalsite::exceptions::CURL_PERFORM_ERROR);
        exc.setMessage("Failed to call curl.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    curl_easy_cleanup(curl);

    // Parse response into json object
    std::istringstream ifs(returned);
    Json::Value responseJson;
    ifs >> responseJson;

    // Check for errors
    if (responseJson["error"].size() > 0) {
        if (responseJson["error"]["code"].asInt() == 401) {
            exc.setErrorCode(personalsite::exceptions::INVALID_TOKEN);
            exc.setMessage("(401) " + responseJson["error"]["message"].asString());
            throw exc;
        }
        if (responseJson["error"]["code"].asInt() == 403) {
            exc.setErrorCode(personalsite::exceptions::EXPIRED_TOKEN);
            exc.setMessage("(403) " + responseJson["error"]["message"].asString());
            throw exc;
        }
    }
    if (responseJson["name"]["givenName"].asString().compare("") == 0 ||
            responseJson["name"]["familyName"].asString().compare("") == 0 ||
            responseJson["emails"][0]["value"].asString().compare("") == 0) {
        exc.setErrorCode(personalsite::exceptions::OAUTH_FAIL);
        exc.setMessage("Failed to get user info from oauth.");
        throw exc;
    }

    // If no errors, construct and return AuthUserItem object
    personalsite::AuthUserItem user;
    user.setName(responseJson["name"]["givenName"].asString() + " "  + responseJson["name"]["familyName"].asString());
    user.setUid(responseJson["emails"][0]["value"].asString());
    return user;
}

std::vector<personalsite::AuthUserItem> personalsite::Auth::checkIfUserExists(const personalsite::AuthUserItem& item) const {
    std::vector<personalsite::AuthUserItem> retval;
    auto result = ddbClient.Query(
                Aws::DynamoDB::Model::QueryRequest()
                    .WithTableName(personalsite::utils::toAwsString(ddbtable))
                    .WithKeyConditionExpression(personalsite::utils::toAwsString(item.toKeyConditionExpression()))
                    .WithExpressionAttributeValues(item.toKeyConditionAttributeValues()));
    if (!result.IsSuccess()) {
        personalsite::exceptions::ServiceException exc;
        exc.setErrorCode(personalsite::exceptions::DDB_QUERY_FAIL);
        exc.setMessage(personalsite::utils::toStdString(result.GetError().GetMessage()));
        throw exc;
    }
    auto items = result.GetResult().GetItems();
    for (auto returnedItem : items) {
        personalsite::AuthUserItem returned;
        returned.fromAttributeMap(returnedItem);
        retval.push_back(returned);
    }
    return retval;
}

void personalsite::Auth::writeUserEntry(const personalsite::AuthUserItem& item) const {
    auto response= ddbClient.PutItem(
                Aws::DynamoDB::Model::PutItemRequest()
                    .WithTableName(personalsite::utils::toAwsString(ddbtable))
                    .WithItem(item.toAttributeMap()));
    if (!response.IsSuccess()) {
        personalsite::exceptions::ServiceException exc;
        exc.setErrorCode(personalsite::exceptions::DDB_PUT_ITEM_FAIL);
        exc.setMessage(personalsite::utils::toStdString(response.GetError().GetMessage()));
        throw exc;
    }
}

personalsite::AuthUserItem personalsite::Auth::getUserInfo(const std::string& accessToken) const {
    personalsite::AuthUserItem user = getInfoFromOAuth(accessToken);
    std::vector<personalsite::AuthUserItem> queryResult = checkIfUserExists(user);

    // If user don't exist, generate a pwd
    if (queryResult.size() == 0) {
        user.setPwd(std::to_string(std::hash<std::string>{}(user.getUid() + std::to_string(std::time(0)) + std::to_string(std::rand()))));
        user.setDate((long int)std::time(0));
        writeUserEntry(user);
        return user;
    }

    // Otherwise return the credentials with the latest password
    personalsite::AuthUserItem retval = queryResult[0];
    for (auto& userItem : queryResult) {
        if (userItem.getDate() > retval.getDate()) {
            retval = userItem;
        }
    }
    return retval;
}

bool personalsite::Auth::authnticateUser(AuthUserItem user) const {
    std::vector<personalsite::AuthUserItem> queryResult = checkIfUserExists(user);
    if (queryResult.size() == 0) {
        return false;
    }
    return true;
}
