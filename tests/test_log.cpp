/**
 * @file test_log.cpp
 * @brief 日志类测试
 * @version 0.1
 * @date 2021-06-10
 */

#include "mcs/mcs.h"

#include <unistd.h>

mcs::Logger::ptr g_logger = MCS_LOG_ROOT(); // 默认INFO级别

int main(int argc, char *argv[]) {
    mcs::EnvMgr::GetInstance()->init(argc, argv);
    mcs::Config::LoadFromConfDir(mcs::EnvMgr::GetInstance()->getConfigPath());

    MCS_LOG_FATAL(g_logger) << "fatal msg";
    MCS_LOG_ERROR(g_logger) << "err msg";
    MCS_LOG_INFO(g_logger) << "info msg";
    MCS_LOG_DEBUG(g_logger) << "debug msg";

    MCS_LOG_FMT_FATAL(g_logger, "fatal %s:%d", __FILE__, __LINE__);
    MCS_LOG_FMT_ERROR(g_logger, "err %s:%d", __FILE__, __LINE__);
    MCS_LOG_FMT_INFO(g_logger, "info %s:%d", __FILE__, __LINE__);
    MCS_LOG_FMT_DEBUG(g_logger, "debug %s:%d", __FILE__, __LINE__);
   
    sleep(1);
    mcs::SetThreadName("brand_new_thread");

    g_logger->setLevel(mcs::LogLevel::WARN);
    MCS_LOG_FATAL(g_logger) << "fatal msg";
    MCS_LOG_ERROR(g_logger) << "err msg";
    MCS_LOG_INFO(g_logger) << "info msg"; // 不打印
    MCS_LOG_DEBUG(g_logger) << "debug msg"; // 不打印


    mcs::FileLogAppender::ptr fileAppender(new mcs::FileLogAppender("./log.txt"));
    g_logger->addAppender(fileAppender);
    MCS_LOG_FATAL(g_logger) << "fatal msg";
    MCS_LOG_ERROR(g_logger) << "err msg";
    MCS_LOG_INFO(g_logger) << "info msg"; // 不打印
    MCS_LOG_DEBUG(g_logger) << "debug msg"; // 不打印

    mcs::Logger::ptr test_logger = MCS_LOG_NAME("test_logger");
    mcs::StdoutLogAppender::ptr appender(new mcs::StdoutLogAppender);
    mcs::LogFormatter::ptr formatter(new mcs::LogFormatter("%d:%rms%T%p%T%c%T%f:%l %m%n")); // 时间：启动毫秒数 级别 日志名称 文件名：行号 消息 换行
    appender->setFormatter(formatter);
    test_logger->addAppender(appender);
    test_logger->setLevel(mcs::LogLevel::WARN);

    MCS_LOG_ERROR(test_logger) << "err msg";
    MCS_LOG_INFO(test_logger) << "info msg"; // 不打印

    // 输出全部日志器的配置
    g_logger->setLevel(mcs::LogLevel::INFO);
    MCS_LOG_INFO(g_logger) << "logger config:" << mcs::LoggerMgr::GetInstance()->toYamlString();

    return 0;
}