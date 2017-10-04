#ifndef PERSONAL_SITE_EXCEPTION
#define PERSONAL_SITE_EXCEPTION

#include <string>

namespace Sigsegv {
    namespace Personalsite {
        namespace Exceptions {
            enum class ExceptionType {
                UNKNOWN_EXCEPTION,
                INVALID_CONFIG,
                INVALID_TOKEN,
                EXPIRED_TOKEN,
                AUTH_EXCEPTION
            };

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

            class ClientException : public std::exception {
                private:
                std::string message;
                ExceptionType errorCode;

                public:
                ClientException () : message{ "Unknown exception" }, errorCode{ ExceptionType::UNKNOWN_EXCEPTION } {
                }
                virtual ~ClientException () {
                }
                void operator= (const ClientException& other);
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
