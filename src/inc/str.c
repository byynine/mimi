int strequal(const char *str_0, const char *str_1)
{
    while (*str_0 && *str_1)
    {
        if (*str_0 != *str_1) return 0;
        str_0++;
        str_1++;
    }

    return *str_0 == *str_1;
}
