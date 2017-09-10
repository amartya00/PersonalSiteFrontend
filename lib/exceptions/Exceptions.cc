#include "Exceptions.h"

void personalsite::exceptions::ServiceException::operator =(const personalsite::exceptions::ServiceException& other) {
    this->message = other.message;
    this->errorCode = other.errorCode;
}

const char* personalsite::exceptions::ServiceException::what() const noexcept {
    return message.c_str();
}

void personalsite::exceptions::ServiceException::setMessage(const std::string& message) {
    this->message = message;
}

void personalsite::exceptions::ServiceException::setErrorCode(const unsigned int errorCode) {
    this->errorCode= errorCode;
}

std::string personalsite::exceptions::ServiceException::getMessage() const {
    return message;
}

unsigned int personalsite::exceptions::ServiceException::getErrorCode() const {
    return errorCode;
}
