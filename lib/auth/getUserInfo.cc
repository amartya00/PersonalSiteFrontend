#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <vector>
#include <ctime>
#include <functional>
#include <string>

Sigsegv::Personalsite::Auth::UserItem Sigsegv::Personalsite::Auth::Service::getUserInfo(const std::string& accessToken) const {
    Sigsegv::Personalsite::Auth::UserItem user = getInfoFromOAuth(accessToken);
    std::vector<Sigsegv::Personalsite::Auth::UserItem> queryResult = checkIfUserExists(user);

    // If user don't exist, generate a pwd
    if (queryResult.size() == 0) {
        user.setPwd(std::to_string(std::hash<std::string>{}(user.getUid() + std::to_string(std::time(0)) + std::to_string(std::rand()))));
        user.setDate((long int)std::time(0));
        writeUserEntry(user);
        return user;
    }

    // Otherwise return the credentials with the latest password
    Sigsegv::Personalsite::Auth::UserItem retval = queryResult[0];
    for (auto& userItem : queryResult) {
        if (userItem.getDate() > retval.getDate()) {
            retval = userItem;
        }
    }
    return retval;
}
