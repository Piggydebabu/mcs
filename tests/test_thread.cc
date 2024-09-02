/**
 * @file test_thread.cc
 * @brief 线程模块测试
 * @version 0.1
 * @date 2021-06-15
 */
#include "mcs/mcs.h"

mcs::Logger::ptr g_logger = MCS_LOG_ROOT();

int count = 0;
mcs::Mutex s_mutex;

void func1(void *arg) {
    MCS_LOG_INFO(g_logger) << "name:" << mcs::Thread::GetName()
        << " this.name:" << mcs::Thread::GetThis()->getName()
        << " thread name:" << mcs::GetThreadName()
        << " id:" << mcs::GetThreadId()
        << " this.id:" << mcs::Thread::GetThis()->getId();
    MCS_LOG_INFO(g_logger) << "arg: " << *(int*)arg;
    for(int i = 0; i < 10000; i++) {
        mcs::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

int main(int argc, char *argv[]) {
    mcs::EnvMgr::GetInstance()->init(argc, argv);
    mcs::Config::LoadFromConfDir(mcs::EnvMgr::GetInstance()->getConfigPath());

    std::vector<mcs::Thread::ptr> thrs;
    int arg = 123456;
    for(int i = 0; i < 3; i++) {
        // 带参数的线程用std::bind进行参数绑定
        mcs::Thread::ptr thr(new mcs::Thread(std::bind(func1, &arg), "thread_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for(int i = 0; i < 3; i++) {
        thrs[i]->join();
    }
    
    MCS_LOG_INFO(g_logger) << "count = " << count;
    return 0;
}

