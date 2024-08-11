/**
 * @file test_daemon.cc
 * @brief 守护进程测试
 * @version 0.1
 * @date 2021-12-09
 */

#include "mcs/mcs.h"

static mcs::Logger::ptr g_logger = mcs_LOG_ROOT();

mcs::Timer::ptr timer;
int server_main(int argc, char **argv) {
    mcs_LOG_INFO(g_logger) << mcs::ProcessInfoMgr::GetInstance()->toString();
    mcs::IOManager iom(1);
    timer = iom.addTimer(
        1000, []() {
            mcs_LOG_INFO(g_logger) << "onTimer";
            static int count = 0;
            if (++count > 10) {
                exit(1);
            }
        },
        true);
    return 0;
}

int main(int argc, char **argv) {
    return mcs::start_daemon(argc, argv, server_main, argc != 1);
}
