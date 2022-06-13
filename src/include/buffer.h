// Name: buffer.h
// Copyright 2022 SH Inc. All rights reserved.
// License:
// Author: Sean (eppesh@163.com)
// IDE:
// Time: 05/05/2022
// Description: Buffer ��, ÿ�����ӵĶ�д������

#ifndef BUFFER_H_    // NOLINT
#define BUFFER_H_    // NOLINT

#include "macros.h"

#include <iostream>
#include <string>

class Buffer
{
public:
    Buffer()  = default;
    ~Buffer() = default;

    DISALLOW_COPY_AND_MOVE(Buffer);

    void Append(const char *str, int size);
    ssize_t Size();
    const char *ToStr();
    void Clear();
    void Getline();    // ��������
    void SetBuffer(const char *buffer);

private:
    std::string buffer_;
};
#endif    // NOLINT