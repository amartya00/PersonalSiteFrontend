#include "DataStructures.h"
#include "PersonalSiteExceptions.h"
#include "Utils.h"

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <json/json.h>

void Sigsegv::Personalsite::UserItem::setName(const std::string& name) {
    this->name = name;
}

void Sigsegv::Personalsite::UserItem::setUid(const std::string& uid) {
    this->uid = uid;
}

void Sigsegv::Personalsite::UserItem::setRsvp(const std::string& key, bool val) {
    if (this->rsvp.find(key) == this->rsvp.end()) {
        Sigsegv::Personalsite::Exceptions::ClientException cexc;
        cexc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("Invalid key for RSVP");
        throw cexc;
    }
    this->rsvp[key] = val;
}

std::string Sigsegv::Personalsite::UserItem::getUid() const {
    return uid;
}

std::string Sigsegv::Personalsite::UserItem::getName() const {
    return name;
}

bool Sigsegv::Personalsite::UserItem::getRsvp(const std::string& key) const {
    if (this->rsvp.find(key) == this->rsvp.end()) {
        Sigsegv::Personalsite::Exceptions::ClientException cexc;
        cexc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("Invalid key for RSVP");
        throw cexc;
    }
    return this->rsvp.at(key);
}

void Sigsegv::Personalsite::UserItem::fromAttributeMap(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap) {
    if (attrMap.find("u") != attrMap.end()) {
        uid = Sigsegv::Personalsite::Utils::toStdString(attrMap.at("u").GetS());
    }
    if (attrMap.find("n") != attrMap.end()) {
        name = Sigsegv::Personalsite::Utils::toStdString(attrMap.at("n").GetS());
    }
    if (attrMap.find("r") != attrMap.end()) {
        std::string rsvpJson {Sigsegv::Personalsite::Utils::toStdString(attrMap.at("r").GetS())};
        std::istringstream iss {rsvpJson};
        Json::Value jsonObj;
        iss >> jsonObj;
        rsvp["Day1"] = jsonObj["Day1"].asBool();
        rsvp["Day2"] = jsonObj["Day2"].asBool();
    }
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Personalsite::UserItem::toAttributeMap () const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval["u"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(uid));
    }
    if (name.compare("") != 0) {
        retval["n"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(name));
    }
    Json::Value jsonObj;
    for (auto entry : rsvp) {
        jsonObj[entry.first] = entry.second;
    }
    retval["r"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(jsonObj.toStyledString()));
    return retval;
}

std::string Sigsegv::Personalsite::UserItem::toKeyConditionExpression() const {
    std::vector<std::string> expressions;
    return "u = :u";
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Personalsite::UserItem::toKeyConditionAttributeValues() const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval[":u"] =  Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(uid));
    }
    return retval;
}
