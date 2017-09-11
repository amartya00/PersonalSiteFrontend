#include "Auth.h"
#include "Utils.h"

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include <string>
#include <iostream>

void Sigsegv::Personalsite::Auth::UserItem::setName(const std::string& name) {
    this->name = name;
}

void Sigsegv::Personalsite::Auth::UserItem::setUid(const std::string& uid) {
    this->uid = uid;
}

void Sigsegv::Personalsite::Auth::UserItem::setPwd(const std::string& pwd) {
    this->pwd = pwd;
}

void Sigsegv::Personalsite::Auth::UserItem::setDate(const long int date) {
    this->date = date;
}

std::string Sigsegv::Personalsite::Auth::UserItem::getName() const {
    return name;
}

std::string Sigsegv::Personalsite::Auth::UserItem::getUid() const {
    return uid;
}

std::string Sigsegv::Personalsite::Auth::UserItem::getPwd() const {
    return pwd;
}

long int Sigsegv::Personalsite::Auth::UserItem::getDate() const {
    return date;
}

void Sigsegv::Personalsite::Auth::UserItem::fromAttributeMap(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap) {
    if (attrMap.find("u") != attrMap.end()) {
        uid = Sigsegv::Personalsite::Utils::toStdString(attrMap.at("u").GetS());
    }
    if (attrMap.find("p") != attrMap.end()) {
        pwd = Sigsegv::Personalsite::Utils::toStdString(attrMap.at("p").GetS());
    }
    if (attrMap.find("n") != attrMap.end()) {
        name = Sigsegv::Personalsite::Utils::toStdString(attrMap.at("n").GetS());
    }
    if (attrMap.find("d") != attrMap.end()) {
        date = std::stol(Sigsegv::Personalsite::Utils::toStdString(attrMap.at("d").GetN()));
    }
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Personalsite::Auth::UserItem::toAttributeMap() const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval["u"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(uid));
    }
    if (pwd.compare("") != 0) {
        retval["p"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(pwd));
    }
    if (name.compare("") != 0) {
        retval["n"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(name));
    }
    if (date != -1) {
        retval["d"] = Aws::DynamoDB::Model::AttributeValue().SetN(Sigsegv::Personalsite::Utils::toAwsString(std::to_string(date)));
    }
    return retval;
}

std::string Sigsegv::Personalsite::Auth::UserItem::toKeyConditionExpression() const {
    std::vector<std::string> expressions;
    if (uid.compare("") != 0) {
        expressions.push_back("u = :u");
    }
    if (pwd.compare("") != 0) {
        expressions.push_back("p = :p");
    }
    std::string retval = "";
    for (unsigned int i = 0; i < expressions.size() - 1; i++) {
        retval += expressions[i] + " and ";
    }
    retval += expressions[expressions.size() - 1];
    return retval;
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Personalsite::Auth::UserItem::toKeyConditionAttributeValues() const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval[":u"] =  Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(uid));
    }
    if (pwd.compare("") != 0) {
        retval[":p"] =  Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Personalsite::Utils::toAwsString(pwd));
    }
    return retval;
}
