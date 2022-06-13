#include "include/util.h"

void ErrorIf(bool condition, const char *error_msg)
{
    if (condition)
    {
        perror(error_msg);
        exit(EXIT_FAILURE);
    }
}