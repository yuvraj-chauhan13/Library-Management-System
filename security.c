#include "security.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void hash_password(const char *password,
                   char hash[PASSWORD_HASH_SIZE])
{
    uint64_t value = 5381;

    while (*password)
    {
        value = ((value << 5) + value) + (unsigned char)(*password);
        password++;
    }

    snprintf(hash,
             PASSWORD_HASH_SIZE,
             "%016llX",
             (unsigned long long)value);
}

int verify_password(const char *password,
                    const char *storedHash)
{
    char hash[PASSWORD_HASH_SIZE];

    hash_password(password, hash);

    return strcmp(hash, storedHash) == 0;
}