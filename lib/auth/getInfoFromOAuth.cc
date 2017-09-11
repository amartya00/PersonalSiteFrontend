#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <json/json.h>
#include <curl/curl.h>

#include <sstream>
#include <vector>

Sigsegv::Personalsite::Auth::UserItem Sigsegv::Personalsite::Auth::Service::getInfoFromOAuth(const std::string &accessToken) const {
    std::string url = "https://www.googleapis.com/plus/v1/people/me?access_token=" + accessToken;
    Sigsegv::Personalsite::Exceptions::ServiceException exc;
    std::string returned;

    // Set up and make calls with curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::CURL_INIT_ERROR);
        exc.setMessage("Failed to initialize curl.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    auto response = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::CURL_SETOPT_ERROR_CURLOPT_URL);
        exc.setMessage("Failed to set curl url.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::CURL_SETOPT_ERROR_CURLOPT_FOLLOWLOCATION);
        exc.setMessage("Failed to set curl follow location.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::CURL_SETOPT_ERROR_CURLOPT_WRITEFUNCTION);
        exc.setMessage("Failed to set curl writeback function.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&returned);
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::CURL_SETOPT_ERROR_CURLOPT_WRITEDATA);
        exc.setMessage("Failed to set curl writeback data.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_perform(curl);
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::CURL_PERFORM_ERROR);
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
            exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_TOKEN);
            exc.setMessage("(401) " + responseJson["error"]["message"].asString());
            throw exc;
        }
        if (responseJson["error"]["code"].asInt() == 403) {
            exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::EXPIRED_TOKEN);
            exc.setMessage("(403) " + responseJson["error"]["message"].asString());
            throw exc;
        }
    }
    if (responseJson["name"]["givenName"].asString().compare("") == 0 ||
            responseJson["name"]["familyName"].asString().compare("") == 0 ||
            responseJson["emails"][0]["value"].asString().compare("") == 0) {
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::OAUTH_FAIL);
        exc.setMessage("Failed to get user info from oauth.");
        throw exc;
    }

    // If no errors, construct and return AuthUserItem object
    Sigsegv::Personalsite::Auth::UserItem user;
    user.setName(responseJson["name"]["givenName"].asString() + " "  + responseJson["name"]["familyName"].asString());
    user.setUid(responseJson["emails"][0]["value"].asString());
    return user;
}
