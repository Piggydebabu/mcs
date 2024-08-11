/**
 * @file macro.h
 * @brief 常用宏的封装
 */
#ifndef __mcs_MACRO_H__
#define __mcs_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#define mcs_LIKELY(x) __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#define mcs_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define mcs_LIKELY(x) (x)
#define mcs_UNLIKELY(x) (x)
#endif

/// 断言宏封装
#define mcs_ASSERT(x)                                                                \
    if (mcs_UNLIKELY(!(x))) {                                                        \
        mcs_LOG_ERROR(mcs_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\nbacktrace:\n"                          \
                                          << mcs::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

/// 断言宏封装
#define mcs_ASSERT2(x, w)                                                            \
    if (mcs_UNLIKELY(!(x))) {                                                        \
        mcs_LOG_ERROR(mcs_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\n"                                      \
                                          << w                                         \
                                          << "\nbacktrace:\n"                          \
                                          << mcs::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

#endif
