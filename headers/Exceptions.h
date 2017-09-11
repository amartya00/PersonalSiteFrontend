#ifndef PERSONAL_SITE_EXCEPTION
#define PERSONAL_SITE_EXCEPTION

#include <string>

namespace Sigsegv {
    namespace Personalsite {
        namespace Exceptions {
            enum class ExceptionType { UNKNOWN_EXCEPTION, INVALID_CONFIG, CURL_INIT_ERROR, CURL_SETOPT_ERROR_CURLOPT_URL, CURL_SETOPT_ERROR_CURLOPT_FOLLOWLOCATION, CURL_SETOPT_ERROR_CURLOPT_WRITEFUNCTION, CURL_SETOPT_ERROR_CURLOPT_WRITEDATA, INVALID_TOKEN, EXPIRED_TOKEN, DDB_PUT_ITEM_FAIL, DDB_QUERY_FAIL, OAUTH_FAIL, CURL_PERFORM_ERROR };

            class ServiceException : public std::exception {
                private:
                std::string message;
                ExceptionType errorCode;

                public:
                ServiceException () : message{ "Unknown exception" }, errorCode{ ExceptionType::UNKNOWN_EXCEPTION } {
                }
                virtual ~ServiceException () {
                }
                void operator= (const ServiceException& other);
                const char* what () const noexcept;

                // Setters
                void setMessage (const std::string& message);
                void setErrorCode (const ExceptionType errorCode);

                // Getters
                std::string getMessage () const;
                ExceptionType getErrorCode () const;
            };
        }
    }
}

#endif
