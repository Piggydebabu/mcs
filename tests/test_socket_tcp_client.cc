/**
 * @file test_socket_tcp_client.cc
 * @brief 测试Socket类，tcp客户端
 * @version 0.1
 * @date 2021-09-18
 */
#include<mcs/mcs.h>

static mcs::Logger::ptr g_logger = mcs_LOG_ROOT();

void test_tcp_client() {
    int ret;

    auto socket = mcs::Socket::CreateTCPSocket();
    mcs_ASSERT(socket);

    auto addr = mcs::Address::LookupAnyIPAddress("0.0.0.0:12345");
    mcs_ASSERT(addr);

    ret = socket->connect(addr);
    mcs_ASSERT(ret);

    mcs_LOG_INFO(g_logger) << "connect success, peer address: " << socket->getRemoteAddress()->toString();

    std::string buffer;
    buffer.resize(1024);
    socket->recv(&buffer[0], buffer.size());
    mcs_LOG_INFO(g_logger) << "recv: " << buffer;
    socket->close();

    return;
}

int main(int argc, char *argv[]) {
    mcs::EnvMgr::GetInstance()->init(argc, argv);
    mcs::Config::LoadFromConfDir(mcs::EnvMgr::GetInstance()->getConfigPath());

    mcs::IOManager iom;
    iom.schedule(&test_tcp_client);

    return 0;
}