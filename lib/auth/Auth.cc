#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <fstream>

#include <json/json.h>

Sigsegv::Personalsite::Auth::Service::Service(std::ifstream& configInputStream) {
    Json::Value jsonObj;
    configInputStream >> jsonObj;
    ddbtable = jsonObj["DDBTableName"].asString();
    if (ddbtable.compare("") == 0) {
        Sigsegv::Personalsite::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_CONFIG);
        exc.setMessage("DDBTableName not found in auth config");
        throw exc;
    }
}
