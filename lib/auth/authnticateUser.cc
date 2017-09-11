#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <vector>

bool Sigsegv::Personalsite::Auth::Service::authnticateUser(Sigsegv::Personalsite::Auth::UserItem user) const {
    std::vector<Sigsegv::Personalsite::Auth::UserItem> queryResult = checkIfUserExists(user);
    if (queryResult.size() == 0) {
        return false;
    }
    return true;
}
