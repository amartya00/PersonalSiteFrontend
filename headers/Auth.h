#ifndef PERSONAL_SITE_AUTH
#define PERSONAL_SITE_AUTH

#include <string>

namespace personalsite {
    namespace auth {
        long getUidFromToken(std::string token);
    }
}
#endif
