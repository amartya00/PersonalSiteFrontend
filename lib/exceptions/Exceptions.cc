#include "PersonalSiteExceptions.h"

void Sigsegv::Personalsite::Exceptions::ServiceException::operator= (const Sigsegv::Personalsite::Exceptions::ServiceException& other) {
    this->message = other.message;
    this->errorCode = other.errorCode;
}

const char* Sigsegv::Personalsite::Exceptions::ServiceException::what () const noexcept {
    return message.c_str ();
}

void Sigsegv::Personalsite::Exceptions::ServiceException::setMessage (const std::string& message) {
    this->message = message;
}

void Sigsegv::Personalsite::Exceptions::ServiceException::setErrorCode (const Sigsegv::Personalsite::Exceptions::ExceptionType errorCode) {
    this->errorCode = errorCode;
}

std::string Sigsegv::Personalsite::Exceptions::ServiceException::getMessage () const {
    return message;
}

Sigsegv::Personalsite::Exceptions::ExceptionType Sigsegv::Personalsite::Exceptions::ServiceException::getErrorCode () const {
    return errorCode;
}


void Sigsegv::Personalsite::Exceptions::ClientException::operator= (const Sigsegv::Personalsite::Exceptions::ClientException& other) {
    this->message = other.message;
    this->errorCode = other.errorCode;
}

const char* Sigsegv::Personalsite::Exceptions::ClientException::what () const noexcept {
    return message.c_str ();
}

void Sigsegv::Personalsite::Exceptions::ClientException::setMessage (const std::string& message) {
    this->message = message;
}

void Sigsegv::Personalsite::Exceptions::ClientException::setErrorCode (const Sigsegv::Personalsite::Exceptions::ExceptionType errorCode) {
    this->errorCode = errorCode;
}

std::string Sigsegv::Personalsite::Exceptions::ClientException::getMessage () const {
    return message;
}

Sigsegv::Personalsite::Exceptions::ExceptionType Sigsegv::Personalsite::Exceptions::ClientException::getErrorCode () const {
    return errorCode;
}
