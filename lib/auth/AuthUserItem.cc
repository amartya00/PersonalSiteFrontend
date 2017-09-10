#include "Auth.h"
#include "Utils.h"

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include <string>
#include <iostream>

void personalsite::AuthUserItem::setName(const std::string& name) {
    this->name = name;
}

void personalsite::AuthUserItem::setUid(const std::string& uid) {
    this->uid = uid;
}

void personalsite::AuthUserItem::setPwd(const std::string& pwd) {
    this->pwd = pwd;
}

void personalsite::AuthUserItem::setDate(const long int date) {
    this->date = date;
}

std::string personalsite::AuthUserItem::getName() const {
    return name;
}

std::string personalsite::AuthUserItem::getUid() const {
    return uid;
}

std::string personalsite::AuthUserItem::getPwd() const {
    return pwd;
}

long int personalsite::AuthUserItem::getDate() const {
    return date;
}

void personalsite::AuthUserItem::fromAttributeMap(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap) {
    if (attrMap.find("u") != attrMap.end()) {
        uid = personalsite::utils::toStdString(attrMap.at("u").GetS());
    }
    if (attrMap.find("p") != attrMap.end()) {
        pwd = personalsite::utils::toStdString(attrMap.at("p").GetS());
    }
    if (attrMap.find("n") != attrMap.end()) {
        name = personalsite::utils::toStdString(attrMap.at("n").GetS());
    }
    if (attrMap.find("d") != attrMap.end()) {
        date = std::stol(personalsite::utils::toStdString(attrMap.at("d").GetN()));
    }
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> personalsite::AuthUserItem::toAttributeMap() const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval["u"] = Aws::DynamoDB::Model::AttributeValue().SetS(personalsite::utils::toAwsString(uid));
    }
    if (pwd.compare("") != 0) {
        retval["p"] = Aws::DynamoDB::Model::AttributeValue().SetS(personalsite::utils::toAwsString(pwd));
    }
    if (name.compare("") != 0) {
        retval["n"] = Aws::DynamoDB::Model::AttributeValue().SetS(personalsite::utils::toAwsString(name));
    }
    if (date != -1) {
        retval["d"] = Aws::DynamoDB::Model::AttributeValue().SetN(personalsite::utils::toAwsString(std::to_string(date)));
    }
    return retval;
}

std::string personalsite::AuthUserItem::toKeyConditionExpression() const {
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

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> personalsite::AuthUserItem::toKeyConditionAttributeValues() const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval[":u"] =  Aws::DynamoDB::Model::AttributeValue().SetS(personalsite::utils::toAwsString(uid));
    }
    if (pwd.compare("") != 0) {
        retval[":p"] =  Aws::DynamoDB::Model::AttributeValue().SetS(personalsite::utils::toAwsString(pwd));
    }
    return retval;
}


