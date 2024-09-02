/**
 * @file test_http_connection.cc
 * @brief HTTP客户端类测试
 * @version 0.1
 * @date 2021-12-09
 */
#include "mcs/mcs.h"
#include <iostream>

static mcs::Logger::ptr g_logger = MCS_LOG_ROOT();

void test_pool() {
    mcs::http::HttpConnectionPool::ptr pool(new mcs::http::HttpConnectionPool(
        "www.midlane.top", "", 80, 10, 1000 * 30, 5));

    mcs::IOManager::GetThis()->addTimer(
        1000, [pool]() {
            auto r = pool->doGet("/", 300);
            std::cout << r->toString() << std::endl;
        },
        true);
}

void run() {
    mcs::Address::ptr addr = mcs::Address::LookupAnyIPAddress("www.midlane.top:80");
    if (!addr) {
        MCS_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    mcs::Socket::ptr sock = mcs::Socket::CreateTCP(addr);
    bool rt                 = sock->connect(addr);
    if (!rt) {
        MCS_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }

    mcs::http::HttpConnection::ptr conn(new mcs::http::HttpConnection(sock));
    mcs::http::HttpRequest::ptr req(new mcs::http::HttpRequest);
    req->setPath("/");
    req->setHeader("host", "www.midlane.top");
    // 小bug，如果设置了keep-alive，那么要在使用前先调用一次init
    req->setHeader("connection", "keep-alive");
    req->init();
    std::cout << "req:" << std::endl
              << *req << std::endl;

    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if (!rsp) {
        MCS_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    std::cout << "rsp:" << std::endl
              << *rsp << std::endl;

    std::cout << "=========================" << std::endl;

    auto r = mcs::http::HttpConnection::DoGet("http://www.midlane.top/wiki/", 300);
    std::cout << "result=" << r->result
              << " error=" << r->error
              << " rsp=" << (r->response ? r->response->toString() : "")
              << std::endl;

    std::cout << "=========================" << std::endl;
    test_pool();
}

int main(int argc, char **argv) {
    mcs::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
