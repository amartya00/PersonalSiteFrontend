#include <folly/init/Init.h>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>

#include <iostream>
#include <thread>

#include "PersonalSiteFrontend.h"

using namespace folly;
using namespace personalsite::frontend;

void testClient() {
    EventBase eb;
    uint16_t port = 9090;
    auto client = std::make_unique<PersonalSiteFrontendAsyncClient>(
                apache::thrift::HeaderClientChannel::newChannel(
                    apache::thrift::async::TAsyncSocket::newSocket(&eb, {"127.0.0.1", port})));
    GetUidFromTokenRequest request;
    request.set_token("test_token");
    GetUidFromTokenResponse response;
    client->sync_getUidFromToken(response, request);
    std::cout << response.get_uid() << "\n";
}

void printStuff() {
    std::cout << "Hello\n";
}

int main(int argc, char *argv[]) {
    folly::init(&argc, &argv, true);
    std::thread threads[100];
    for (auto &t : threads) {
        t = std::thread(testClient);
    }
    for (auto &t : threads) {
        t.join();
    }
}
