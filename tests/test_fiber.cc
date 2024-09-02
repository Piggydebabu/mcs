/**
 * @file test_fiber.cc
 * @brief 协程测试
 * @version 0.1
 * @date 2021-06-15
 */
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

    MCS_LOG_INFO(g_logger) << "before run_in_fiber yield";
    mcs::Fiber::GetThis()->yield();
    MCS_LOG_INFO(g_logger) << "after run_in_fiber yield";

    MCS_LOG_INFO(g_logger) << "run_in_fiber end";
    // fiber结束之后会自动返回主协程运行
}

void test_fiber() {
    MCS_LOG_INFO(g_logger) << "test_fiber begin";

    // 初始化线程主协程
    mcs::Fiber::GetThis();

    mcs::Fiber::ptr fiber(new mcs::Fiber(run_in_fiber, 0, false));
    MCS_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1

    MCS_LOG_INFO(g_logger) << "before test_fiber resume";
    fiber->resume();
    MCS_LOG_INFO(g_logger) << "after test_fiber resume";

    /** 
     * 关于fiber智能指针的引用计数为3的说明：
     * 一份在当前函数的fiber指针，一份在MainFunc的cur指针
     * 还有一份在在run_in_fiber的GetThis()结果的临时变量里
     */
    MCS_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 3

    MCS_LOG_INFO(g_logger) << "fiber status: " << fiber->getState(); // READY

    MCS_LOG_INFO(g_logger) << "before test_fiber resume again";
    fiber->resume();
    MCS_LOG_INFO(g_logger) << "after test_fiber resume again";

    MCS_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1
    MCS_LOG_INFO(g_logger) << "fiber status: " << fiber->getState(); // TERM

    fiber->reset(run_in_fiber2); // 上一个协程结束之后，复用其栈空间再创建一个新协程
    fiber->resume();

    MCS_LOG_INFO(g_logger) << "use_count:" << fiber.use_count(); // 1
    MCS_LOG_INFO(g_logger) << "test_fiber end";
}

int main(int argc, char *argv[]) {
    mcs::EnvMgr::GetInstance()->init(argc, argv);
    mcs::Config::LoadFromConfDir(mcs::EnvMgr::GetInstance()->getConfigPath());

    mcs::SetThreadName("main_thread");
    MCS_LOG_INFO(g_logger) << "main begin";

    std::vector<mcs::Thread::ptr> thrs;
    for (int i = 0; i < 2; i++) {
        thrs.push_back(mcs::Thread::ptr(
            new mcs::Thread(&test_fiber, "thread_" + std::to_string(i))));
    }

    for (auto i : thrs) {
        i->join();
    }

    MCS_LOG_INFO(g_logger) << "main end";
    return 0;
}