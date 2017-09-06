#ifndef PERSONAL_SITE_UTILS
#define PERSONAL_SITE_UTILS

#include <string>

namespace personalsite {
    namespace utils {
        void initAwsLogging(std::string executable, std::string prefix);
        void shutDownLogs();
    }
}

#endif
