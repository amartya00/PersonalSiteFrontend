#ifndef PERSONAL_SITE_UTILS
#define PERSONAL_SITE_UTILS

#include <aws/core/Aws.h>
#include <string>

namespace personalsite {
    namespace utils {
        void initAws ();
        void initAwsLogging (std::string executable, std::string prefix);
        void shutDownLogs ();
        void shutDownAws ();

        Aws::String toAwsString (const std::string&);
        std::string toStdString (const Aws::String&);
    }
}

#endif
