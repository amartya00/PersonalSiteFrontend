#include <folly/init/Init.h>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>

#include <iostream>
#include <thread>

#include "Frontend.h"

using namespace folly;
using namespace Sigsegv::Personalsite;

void testClient (const std::string& token) {
    EventBase eb;
    uint16_t port = 9090;
    auto client = std::make_unique<FrontendAsyncClient> (apache::thrift::HeaderClientChannel::newChannel (apache::thrift::async::TAsyncSocket::newSocket (&eb, { "127.0.0.1", port })));
    GetUidFromTokenRequest request;
    request.set_token (token);
    GetUidFromTokenResponse response;
    client->sync_getUidFromToken (response, request);
    std::cout << "UID: " << response.get_uid () << "\n";
    std::cout << "Name: " << response.get_name () << "\n";
    std::cout << "Password: " << response.get_pwd () << "\n";
}

int main (int argc, char* argv[]) {
    folly::init (&argc, &argv, true);
    std::string token (argv[1]);
    std::thread threads[1];
    for (auto& t : threads) {
        t = std::thread (testClient, token);
    }
    for (auto& t : threads) {
        t.join ();
    }
}
