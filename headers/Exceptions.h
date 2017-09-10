#ifndef PERSONAL_SITE_EXCEPTION
#define PERSONAL_SITE_EXCEPTION

#include <string>

namespace personalsite {
    namespace exceptions {
        const unsigned int UNKNOWN_EXCEPTION = 1000;
        const unsigned int INVALID_CONFIG = 0;
        const unsigned int CURL_INIT_ERROR = 1;
        const unsigned int CURL_SETOPT_ERROR_CURLOPT_URL = 2;
        const unsigned int CURL_SETOPT_ERROR_CURLOPT_FOLLOWLOCATION = 3;
        const unsigned int CURL_SETOPT_ERROR_CURLOPT_WRITEFUNCTION = 4;
        const unsigned int CURL_SETOPT_ERROR_CURLOPT_WRITEDATA = 5;
        const unsigned int INVALID_TOKEN = 6;
        const unsigned int EXPIRED_TOKEN = 7;
        const unsigned int DDB_PUT_ITEM_FAIL = 8;
        const unsigned int DDB_QUERY_FAIL = 9;
        const unsigned int OAUTH_FAIL = 10;
        const unsigned int CURL_PERFORM_ERROR = 11;

        class ServiceException : public std::exception {
            private:
            std::string message;
            unsigned int errorCode;

            public:
            ServiceException () : message{ "Unknown exception" }, errorCode{ UNKNOWN_EXCEPTION } {
            }
            virtual ~ServiceException () {
            }
            void operator= (const ServiceException& other);
            const char* what () const noexcept;

            // Setters
            void setMessage (const std::string& message);
            void setErrorCode (const unsigned int errorCode);

            // Getters
            std::string getMessage () const;
            unsigned int getErrorCode () const;
        };
    }
}

#endif
