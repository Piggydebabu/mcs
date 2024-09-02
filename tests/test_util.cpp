/**
 * @file test_util.cpp
 * @brief util与macro测试
 * @version 0.1
 * @date 2021-06-12
 */
#include "mcs/mcs.h"

mcs::Logger::ptr g_logger = MCS_LOG_ROOT();

void test2() {
    std::cout << mcs::BacktraceToString() << std::endl;
}
void test1() {
    test2();
}

void test_backtrace() {
    test1();
}

int main() {
    MCS_LOG_INFO(g_logger) << mcs::GetCurrentMS();
    MCS_LOG_INFO(g_logger) << mcs::GetCurrentUS();
    MCS_LOG_INFO(g_logger) << mcs::ToUpper("hello");
    MCS_LOG_INFO(g_logger) << mcs::ToLower("HELLO");
    MCS_LOG_INFO(g_logger) << mcs::Time2Str();
    MCS_LOG_INFO(g_logger) << mcs::Str2Time("1970-01-01 00:00:00"); // -28800

    std::vector<std::string> files;
    mcs::FSUtil::ListAllFile(files, "./mcs", ".cpp");
    for (auto &i : files) {
        MCS_LOG_INFO(g_logger) << i;
    }

    // todo, more...

    test_backtrace();

    MCS_ASSERT2(false, "assert");
    return 0;
}