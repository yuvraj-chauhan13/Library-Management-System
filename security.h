#ifndef SECURITY_H
#define SECURITY_H

#define PASSWORD_HASH_SIZE 17

void hash_password(const char *password,
                   char hash[PASSWORD_HASH_SIZE]);

int verify_password(const char *password,
                    const char *storedHash);

#endif