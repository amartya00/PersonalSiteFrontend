#include <folly/init/Init.h>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>

#include <iostream>
#include <thread>

#include "Frontend.h"
#include "Service_types.h"

using namespace folly;
using namespace Sigsegv::Personalsite;

void testClient (const std::string& ip, const std::string& token) {
    EventBase eb;
    uint16_t port = 9090;
    auto client = std::make_unique<FrontendAsyncClient> (apache::thrift::HeaderClientChannel::newChannel (apache::thrift::async::TAsyncSocket::newSocket (&eb, { ip, port })));
    GetUidFromTokenRequest request;
    request.set_token (token);
    GetUidFromTokenResponse response;
    try {
        client->sync_getUidFromToken (response, request);
        std::cout << "UID: " << response.get_uid () << "\n";
        std::cout << "Name: " << response.get_name () << "\n";
        std::cout << "Password: " << response.get_pwd () << "\n";
    } catch (const Sigsegv::Personalsite::ServiceException& e) {
        std::cout << e.get_errorMessage() << "\n";
    } catch (const Sigsegv::Personalsite::ClientException& e) {
        std::cout << e.get_errorMessage() << "\n";
    }
}

int main (int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Please provide ip address and oauth token.\n";
        return 1;
    }
    folly::init (&argc, &argv, true);

    std::string ip {argv[1]};
    std::string token {argv[2]};

    std::thread threads[1];
    for (auto& t : threads) {
        t = std::thread (testClient, ip, token);
    }
    for (auto& t : threads) {
        t.join ();
    }
}
