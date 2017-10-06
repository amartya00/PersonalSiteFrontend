#include <folly/init/Init.h>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp/async/TAsyncSSLSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>
//#include <thrift/lib/cpp2/async/HTTPClientChannel.h>

#include <iostream>
#include <thread>

#include "Frontend.h"
#include "Service_types.h"

using namespace folly;
using namespace Sigsegv::Personalsite;

void testClient (const std::string& ip, const std::string& token, const std::string& cert) {
    EventBase eb;
    uint16_t port = 9090;

    // Create ssl context
    std::shared_ptr<folly::SSLContext> sslContext = std::make_shared<folly::SSLContext>();
    sslContext->loadCertificate(cert.c_str());
    sslContext->setAdvertisedNextProtocols({"h2", "http"});

    // Create socket, transport, etc
    apache::thrift::async::TAsyncTransport::UniquePtr transport;
    auto socket = new apache::thrift::async::TAsyncSocket (&eb, { ip, port });
    auto sslSocket = new apache::thrift::async::TAsyncSSLSocket(sslContext, &eb, socket->detachFd(), false);
    sslSocket->sslConn(nullptr);
    transport.reset(sslSocket);
    auto channel = apache::thrift::HeaderClientChannel::newChannel(std::move(transport));
    auto client = std::make_unique<FrontendAsyncClient> (apache::thrift::HeaderClientChannel::newChannel (apache::thrift::async::TAsyncSocket::newSocket (&eb, { ip, port })));

    // Make the request
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
    if (argc < 4) {
        std::cerr << "Please provide ip address and oauth token and cert.\n";
        return 1;
    }
    folly::init (&argc, &argv, true);

    std::string ip {argv[1]};
    std::string token {argv[2]};
    std::string cert {argv[3]};

    std::thread threads[1];
    for (auto& t : threads) {
        t = std::thread (testClient, ip, token, cert);
    }
    for (auto& t : threads) {
        t.join ();
    }
}
