#include <folly/init/Init.h>
#include <thrift/lib/cpp/async/TAsyncSSLSocket.h>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>

#include <iostream>
#include <thread>

#include <json/json.h>

#include <boost/python.hpp>

#include "Frontend.h"
#include "Service_types.h"

using namespace folly;
using namespace Sigsegv::Personalsite;

const std::string errorCodeToString (const Sigsegv::Personalsite::ErrorCode code) {
    if (code == Sigsegv::Personalsite::ErrorCode::EXPIRED_TOKEN) {
        return "EXPIRED_TOKEN";
    }
    if (code == Sigsegv::Personalsite::ErrorCode::INCORRECT_TOKEN) {
        return "INCORRECT_TOKEN";
    }
    if (code == Sigsegv::Personalsite::ErrorCode::SERVICE_UNAVAILABLE) {
        return "SERVICE_UNAVAILABLE";
    }
}

const std::string getUidFromToken (const std::string& token, const std::string& ip, const std::string& cert) {
    EventBase eb;
    uint16_t port = 9090;
    Json::Value jsonObj;
    std::string retval;

    // Create ssl context
    std::shared_ptr<folly::SSLContext> sslContext = std::make_shared<folly::SSLContext> ();
    sslContext->loadCertificate (cert.c_str ());
    sslContext->setAdvertisedNextProtocols ({ "h2", "http" });

    // Create socket, transport, etc
    apache::thrift::async::TAsyncTransport::UniquePtr transport;
    auto socket = new apache::thrift::async::TAsyncSocket (&eb, { ip, port });
    auto sslSocket = new apache::thrift::async::TAsyncSSLSocket (sslContext, &eb, socket->detachFd (), false);
    sslSocket->sslConn (nullptr);
    transport.reset (sslSocket);
    auto channel = apache::thrift::HeaderClientChannel::newChannel (std::move (transport));
    auto client = std::make_unique<FrontendAsyncClient> (std::move (channel));

    // Make the request
    GetUidFromTokenRequest request;
    request.set_token (token);
    GetUidFromTokenResponse response;
    try {
        client->sync_getUidFromToken (response, request);
        jsonObj["Response"] = "OK";
        jsonObj["Uid"] = response.get_uid ();
        jsonObj["Pwd"] = response.get_pwd ();
        jsonObj["Name"] = response.get_name ();
        retval = jsonObj.toStyledString ();
    } catch (const Sigsegv::Personalsite::ServiceException& e) {
        jsonObj["Response"] = "ERROR";
        jsonObj["Message"] = e.get_errorMessage ();
        jsonObj["Code"] = errorCodeToString (e.get_errorCode ());
        retval = jsonObj.toStyledString ();
    } catch (const Sigsegv::Personalsite::ClientException& e) {
        jsonObj["Response"] = "ERROR";
        jsonObj["Message"] = e.get_errorMessage ();
        jsonObj["Code"] = errorCodeToString (e.get_errorCode ());
        retval = jsonObj.toStyledString ();
    }
    return retval;
}

std::string getUserInfo (const std::string& uid, const std::string& pwd, const std::string& ip, const std::string& cert) {
    EventBase eb;
    uint16_t port = 9090;
    std::string retval{ "" };
    Json::Value jsonObj;

    // Create ssl context
    std::shared_ptr<folly::SSLContext> sslContext = std::make_shared<folly::SSLContext> ();
    sslContext->loadCertificate (cert.c_str ());
    sslContext->setAdvertisedNextProtocols ({ "h2", "http" });

    // Create socket, transport, etc
    apache::thrift::async::TAsyncTransport::UniquePtr transport;
    auto socket = new apache::thrift::async::TAsyncSocket (&eb, { ip, port });
    auto sslSocket = new apache::thrift::async::TAsyncSSLSocket (sslContext, &eb, socket->detachFd (), false);
    sslSocket->sslConn (nullptr);
    transport.reset (sslSocket);
    auto channel = apache::thrift::HeaderClientChannel::newChannel (std::move (transport));
    auto client = std::make_unique<FrontendAsyncClient> (std::move (channel));

    // Make the request
    GetUserInfoRequest request;
    request.set_uid (uid);
    GetUserInfoResponse response;
    try {
        client->sync_getUserInfo (response, request);
        jsonObj["Response"] = "OK";
        jsonObj["Name"] = response.get_name ();
        jsonObj["Day1"] = response.get_isRsvpDay1 ();
        jsonObj["Day2"] = response.get_isRsvpDay2 ();
        retval = jsonObj.toStyledString ();
    } catch (const Sigsegv::Personalsite::ServiceException& e) {
        jsonObj["Response"] = "ERROR";
        jsonObj["Message"] = e.get_errorMessage ();
        jsonObj["Code"] = errorCodeToString (e.get_errorCode ());
        jsonObj["Type"] = "ServerError";
        retval = jsonObj.toStyledString ();
    } catch (const Sigsegv::Personalsite::ClientException& e) {
        jsonObj["Response"] = "ERROR";
        jsonObj["Message"] = e.get_errorMessage ();
        jsonObj["Code"] = errorCodeToString (e.get_errorCode ());
        jsonObj["Type"] = "ClientError";
        retval = jsonObj.toStyledString ();
    }
    return retval;
}

std::string updateUserInfo (const std::string& uid, const std::string& pwd, const std::string& name, const std::string& rsvp1, const std::string& rsvp2, const std::string& ip, const std::string& cert) {
    EventBase eb;
    uint16_t port = 9090;
    Json::Value jsonObj;
    std::string retval{ "" };

    // Create ssl context
    std::shared_ptr<folly::SSLContext> sslContext = std::make_shared<folly::SSLContext> ();
    sslContext->loadCertificate (cert.c_str ());
    sslContext->setAdvertisedNextProtocols ({ "h2", "http" });

    // Create socket, transport, etc
    apache::thrift::async::TAsyncTransport::UniquePtr transport;
    auto socket = new apache::thrift::async::TAsyncSocket (&eb, { ip, port });
    auto sslSocket = new apache::thrift::async::TAsyncSSLSocket (sslContext, &eb, socket->detachFd (), false);
    sslSocket->sslConn (nullptr);
    transport.reset (sslSocket);
    auto channel = apache::thrift::HeaderClientChannel::newChannel (std::move (transport));
    auto client = std::make_unique<FrontendAsyncClient> (std::move (channel));

    // Make the request
    UpdateUserInfoRequest request;
    request.set_uid (uid);
    request.set_name (name);
    request.set_pwd (pwd);
    request.set_isRsvpDay1 (rsvp1.compare ("True") == 0);
    request.set_isRsvpDay2 (rsvp2.compare ("True") == 0);
    UpdateUserInfoResponse response;
    try {
        client->sync_updateUserInfo (response, request);
        jsonObj["Response"] = "OK";
        retval = jsonObj.toStyledString ();
    } catch (const Sigsegv::Personalsite::ServiceException& e) {
        jsonObj["Response"] = "ERROR";
        jsonObj["Message"] = e.get_errorMessage ();
        jsonObj["Code"] = errorCodeToString (e.get_errorCode ());
        jsonObj["Type"] = "ServerError";
        retval = jsonObj.toStyledString ();
    } catch (const Sigsegv::Personalsite::ClientException& e) {
        jsonObj["Response"] = "ERROR";
        jsonObj["Message"] = e.get_errorMessage ();
        jsonObj["Code"] = errorCodeToString (e.get_errorCode ());
        jsonObj["Type"] = "ServerError";
        retval = jsonObj.toStyledString ();
    }
    return retval;
}

BOOST_PYTHON_MODULE (libClient) {
    using namespace boost::python;
    def ("getUidFromToken", getUidFromToken);
    def ("getUserInfo", getUserInfo);
    def ("updateUserInfo", updateUserInfo);
}

int main (int argc, char* argv[]) {
}
