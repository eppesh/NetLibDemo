// Name: util.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:工具类,包含常用辅助函数;

#ifndef UTIL_H_    // NOLINT
#define UTIL_H_    // NOLINT

#include <cstdio>
#include <cstdlib>

// 条件condition为true时输出错误信息error_msg
void ErrorIf(bool condition, const char *error_msg);

#endif