#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <vector>
#include <algorithm>

bool Sigsegv::Personalsite::Auth::Service::authnticateUser(Sigsegv::Personalsite::Auth::UserItem user) const {
    if (std::find(whitelistedIds.begin(), whitelistedIds.end(), user.getUid()) == whitelistedIds.end()) {
        return false;
    }

    std::vector<Sigsegv::Personalsite::Auth::UserItem> queryResult = checkIfUserExists(user);
    if (queryResult.size() == 0) {
        return false;
    }
    return true;
}
