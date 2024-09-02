/**
 * @file test_fiber2.cc
 * @brief 协程测试，用于演示非对称协程
 * @version 0.1
 * @date 2021-06-15
 */
#include "mcs/fiber.h"
#include "mcs/mcs.h"
#include <string>
#include <vector>

mcs::Logger::ptr g_logger = MCS_LOG_ROOT();

void run_in_fiber2() {
    MCS_LOG_INFO(g_logger) << "run_in_fiber2 begin";
    MCS_LOG_INFO(g_logger) << "run_in_fiber2 end";
}

void run_in_fiber() {
    MCS_LOG_INFO(g_logger) << "run_in_fiber begin";

    /**
     * 非对称协程，子协程不能创建并运行新的子协程，下面的操作是有问题的，
     * 子协程再创建子协程，原来的主协程就跑飞了
     */
    mcs::Fiber::ptr fiber(new mcs::Fiber(run_in_fiber2, 0, false));
    fiber->resume();

    MCS_LOG_INFO(g_logger) << "run_in_fiber end";
}

int main(int argc, char *argv[]) {
    mcs::EnvMgr::GetInstance()->init(argc, argv);
    mcs::Config::LoadFromConfDir(mcs::EnvMgr::GetInstance()->getConfigPath());

    MCS_LOG_INFO(g_logger) << "main begin";

    mcs::Fiber::GetThis();

    mcs::Fiber::ptr fiber(new mcs::Fiber(run_in_fiber, 0, false));
    fiber->resume();

    MCS_LOG_INFO(g_logger) << "main end";
    return 0;
}