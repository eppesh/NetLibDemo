// Name: util.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 04/26/2022
// Description:������,�������ø�������;

#ifndef UTIL_H_    // NOLINT
#define UTIL_H_    // NOLINT

#include <cstdio>
#include <cstdlib>

// ����conditionΪtrueʱ���������Ϣerror_msg
void ErrorIf(bool condition, const char *error_msg);

#endif