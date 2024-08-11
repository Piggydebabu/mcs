/**
 * @file test_socket.cc
 * @brief 测试Socket类，tcp服务器
 * @version 0.1
 * @date 2021-09-18
 */
#include <mcs/mcs.h>

static mcs::Logger::ptr g_logger = mcs_LOG_ROOT();

void test_tcp_server() {
    int ret;

    auto addr = mcs::Address::LookupAnyIPAddress("0.0.0.0:12345");
    mcs_ASSERT(addr);

    auto socket = mcs::Socket::CreateTCPSocket();
    mcs_ASSERT(socket);

    ret = socket->bind(addr);
    mcs_ASSERT(ret);
    
    mcs_LOG_INFO(g_logger) << "bind success";

    ret = socket->listen();
    mcs_ASSERT(ret);

    mcs_LOG_INFO(g_logger) << socket->toString() ;
    mcs_LOG_INFO(g_logger) << "listening...";

    while(1) {
        auto client = socket->accept();
        mcs_ASSERT(client);
        mcs_LOG_INFO(g_logger) << "new client: " << client->toString();
        client->send("hello world", strlen("hello world"));
        client->close();
    }
}

int main(int argc, char *argv[]) {
    mcs::EnvMgr::GetInstance()->init(argc, argv);
    mcs::Config::LoadFromConfDir(mcs::EnvMgr::GetInstance()->getConfigPath());

    mcs::IOManager iom(2);
    iom.schedule(&test_tcp_server);

    return 0;
}