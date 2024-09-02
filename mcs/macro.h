/**
 * @file macro.h
 * @brief 常用宏的封装
 */
#ifndef __MCS_MACRO_H__
#define __MCS_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#define MCS_LIKELY(x) __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#define MCS_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define MCS_LIKELY(x) (x)
#define MCS_UNLIKELY(x) (x)
#endif

/// 断言宏封装
#define MCS_ASSERT(x)                                                                \
    if (MCS_UNLIKELY(!(x))) {                                                        \
        MCS_LOG_ERROR(MCS_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\nbacktrace:\n"                          \
                                          << mcs::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

/// 断言宏封装
#define MCS_ASSERT2(x, w)                                                            \
    if (MCS_UNLIKELY(!(x))) {                                                        \
        MCS_LOG_ERROR(MCS_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\n"                                      \
                                          << w                                         \
                                          << "\nbacktrace:\n"                          \
                                          << mcs::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

#endif
