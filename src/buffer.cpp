#include "include/buffer.h"

void Buffer::Append(const char *str, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (str[i] == '\0')
        {
            break;
        }
        buffer_.push_back(str[i]);
    }
}

ssize_t Buffer::Size()
{
    return buffer_.size();
}

const char *Buffer::ToStr()
{
    return buffer_.c_str();
}

void Buffer::Clear()
{
    buffer_.clear();
}

void Buffer::Getline()
{
    buffer_.clear();
    std::getline(std::cin, buffer_);
}

void Buffer::SetBuffer(const char *buffer)
{
    buffer_.clear();
    buffer_.append(buffer);
}