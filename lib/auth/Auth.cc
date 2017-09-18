#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <fstream>

#include <json/json.h>

Sigsegv::Personalsite::Auth::Service::Service(std::ifstream& configInputStream, std::ifstream& whitelistInputStream) {
    Json::Value jsonObj;
    configInputStream >> jsonObj;
    ddbtable = jsonObj["DDBTableName"].asString();
    whiteListingEnabled = jsonObj["WhitelistingEnabled"].asBool();

    Json::Value whitelistJson;
    whitelistInputStream >> whitelistJson;
    for (auto acct : whitelistJson["WhitelistedAccounts"]) {
        whitelistedIds.push_back(acct.asString());
    }

    if (ddbtable.compare("") == 0) {
        Sigsegv::Personalsite::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_CONFIG);
        exc.setMessage("DDBTableName not found in auth config");
        throw exc;
    }
}
