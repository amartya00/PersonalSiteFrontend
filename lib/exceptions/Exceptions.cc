#include "Exceptions.h"

void Sigsegv::Personalsite::Exceptions::ServiceException::operator =(const Sigsegv::Personalsite::Exceptions::ServiceException& other) {
    this->message = other.message;
    this->errorCode = other.errorCode;
}

const char* Sigsegv::Personalsite::Exceptions::ServiceException::what() const noexcept {
    return message.c_str();
}

void Sigsegv::Personalsite::Exceptions::ServiceException::setMessage(const std::string& message) {
    this->message = message;
}

void Sigsegv::Personalsite::Exceptions::ServiceException::setErrorCode(const Sigsegv::Personalsite::Exceptions::ExceptionType errorCode) {
    this->errorCode= errorCode;
}

std::string Sigsegv::Personalsite::Exceptions::ServiceException::getMessage() const {
    return message;
}

Sigsegv::Personalsite::Exceptions::ExceptionType Sigsegv::Personalsite::Exceptions::ServiceException::getErrorCode() const {
    return errorCode;
}
