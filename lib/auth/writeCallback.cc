#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <string>

size_t Sigsegv::Personalsite::Auth::Service::writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, realsize);
    return realsize;
}
