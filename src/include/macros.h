// Name: inet_address.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:常用的宏定义

#ifndef MACROS_H_    // NOLINT
#define MACROS_H_    // NOLINT

#include <cassert>
#include <stdexcept>

#define OS_LINUX

// macros to disable copying and moving
#define DISALLOW_COPY(cname)                                                                                           \
    cname(const cname &) = delete;                                                                                     \
    cname &operator=(const cname &) = delete;    // NOLINT

#define DISALLOW_MOVE(cname)                                                                                           \
    cname(cname &&) = delete;                                                                                          \
    cname &operator=(cname &&) = delete;    // NOLINT

#define DISALLOW_COPY_AND_MOVE(cname)                                                                                  \
    DISALLOW_COPY(cname);                                                                                              \
    DISALLOW_MOVE(cname);

#define ASSERT(expr, message) assert((expr) && (message))

#define UNREACHABLE(message) throw std::logic_error(message)
#endif    // NOLINT