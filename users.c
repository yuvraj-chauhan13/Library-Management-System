#include "users.h"
#include "storage.h"
#include "security.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#define USERS_FILE "data/users.txt"

static void trim(char *str) {
    if (str == NULL) return;

    int start = 0;

    while (isspace((unsigned char)str[start])) {
        start++;
    }

    if (start > 0) {
        memmove(str, str + start, strlen(str + start) + 1);
    }

    int end = strlen(str) - 1;

    while (end >= 0 && isspace((unsigned char)str[end])) {
        str[end] = '\0';
        end--;
    }
}

static const char *role_to_string(int role)
{
    switch (role)
    {
        case 1:
            return "Student";

        case 2:
            return "Librarian";

        case 3:
            return "Admin";

        default:
            return "Unknown";
    }
}

static UserStatus validate_username(const char *username)
{
    if (username == NULL)
        return USERNAME_EMPTY;

    char temp[USERNAME_MAX];

    strncpy(temp, username, USERNAME_MAX - 1);
    temp[USERNAME_MAX - 1] = '\0';

    trim(temp);

    if (strlen(temp) == 0)
        return USERNAME_EMPTY;

    if (strlen(username) >= USERNAME_MAX)
        return USERNAME_TOO_LONG;

    if (strchr(temp, '|') != NULL)
        return USERNAME_INVALID;

    if (strchr(temp, '\n') != NULL)
        return USERNAME_INVALID;

    if (strchr(temp, '\r') != NULL)
        return USERNAME_INVALID;

    return USER_SUCCESS;
}

static int contains_special(const char *password)
{
    while (*password)
    {
        if (!isalnum((unsigned char)*password))
            return 1;

        password++;
    }

    return 0;
}

static UserStatus validate_password(const char *password)
{
    if (password == NULL)
        return PASSWORD_EMPTY;

    if (strlen(password) == 0)
        return PASSWORD_EMPTY;

    if (strlen(password) < MIN_PASSWORD_LENGTH)
        return PASSWORD_TOO_SHORT;

    int hasUpper = 0;
    int hasLower = 0;
    int hasDigit = 0;

    for (int i = 0; password[i] != '\0'; i++)
    {
        if (isupper((unsigned char)password[i]))
            hasUpper = 1;

        else if (islower((unsigned char)password[i]))
            hasLower = 1;

        else if (isdigit((unsigned char)password[i]))
            hasDigit = 1;
    }

    if (!hasUpper)
        return PASSWORD_NO_UPPERCASE;

    if (!hasLower)
        return PASSWORD_NO_LOWERCASE;

    if (!hasDigit)
        return PASSWORD_NO_DIGIT;

    if (!contains_special(password))
        return PASSWORD_NO_SPECIAL;

    return USER_SUCCESS;
}

static int parse_user(const char *line, User *user)
{
    if (line == NULL || user == NULL)
        return 0;

    char username[USERNAME_MAX];
    char password[PASSWORD_MAX];
    int role;

    if (sscanf(line,
               "%49[^|]|%127[^|]|%d",
               username,
               password,
               &role) != 3)
    {
        return 0;
    }

    strncpy(user->username, username, USERNAME_MAX - 1);
    user->username[USERNAME_MAX - 1] = '\0';

    strncpy(user->password, password, PASSWORD_MAX - 1);
    user->password[PASSWORD_MAX - 1] = '\0';
    user->role = role;

    return 1;
}

static int username_exists(const char *username)
{
    if (username == NULL)
        return 0;

    char usernameCopy[USERNAME_MAX];

    strncpy(usernameCopy, username, USERNAME_MAX - 1);
    usernameCopy[USERNAME_MAX - 1] = '\0';

    trim(usernameCopy);

    char *content = read_file(USERS_FILE);

    if (content == NULL)
        return 0;

    User user;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_user(line, &user))
        {
            if (strcasecmp(user.username, usernameCopy) == 0)
            {
                free(save);
                return 1;
            }
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    return 0;
}

static int save_user(const User *user)
{
    if (user == NULL)
        return 0;

    char line[USERNAME_MAX + PASSWORD_MAX + 20];

    snprintf(line,
             sizeof(line),
             "%s|%s|%d",
             user->username,
             user->password,
             user->role);

    return append_line(USERS_FILE, line) == 0;
}

// authenticate: users.txt lines -> username|password|role
int authenticate_user(const char *username, const char *password)
{
    if (username == NULL || password == NULL)
        return 0;

    char usernameCopy[USERNAME_MAX];

    strncpy(usernameCopy, username, USERNAME_MAX - 1);
    usernameCopy[USERNAME_MAX - 1] = '\0';

    trim(usernameCopy);

    char *content = read_file(USERS_FILE);
    if (content == NULL)
        return 0;

    User user;
    int role = 0;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_user(line, &user))
        {
            if (strcasecmp(user.username, usernameCopy) == 0 &&
                verify_password(password, user.password))
            {
                role = user.role;
                break;
            }
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    return role;
}

UserStatus register_user(const char *username,
                         const char *password,
                         int role)
{
    UserStatus userStatus = validate_username(username);

    if (userStatus != USER_SUCCESS)
        return userStatus;

    UserStatus passStatus = validate_password(password);

    if (passStatus != USER_SUCCESS)
        return passStatus;

    if (username_exists(username))
        return USERNAME_EXISTS;

    if (role < 1 || role > 3)
        return INVALID_ROLE;

    User user;

    strncpy(user.username, username, USERNAME_MAX - 1);
    user.username[USERNAME_MAX - 1] = '\0';
    trim(user.username);

    hash_password(password, user.password);

    user.role = role;

    if (!save_user(&user))
        return STORAGE_ERROR;

    return USER_SUCCESS;
}

const char *user_status_message(UserStatus status)
{
    switch (status)
    {
        case USER_SUCCESS:
            return "Registration successful.";

        case USERNAME_EMPTY:
            return "Username cannot be empty.";

        case USERNAME_EXISTS:
            return "Username already exists.";

        case USERNAME_TOO_LONG:
            return "Username is too long.";

        case USERNAME_INVALID:
            return "Username contains invalid characters.";

        case PASSWORD_EMPTY:
            return "Password cannot be empty.";

        case PASSWORD_TOO_SHORT:
            return "Password must be at least 8 characters long.";

        case PASSWORD_NO_UPPERCASE:
            return "Password must contain at least one uppercase letter.";

        case PASSWORD_NO_LOWERCASE:
            return "Password must contain at least one lowercase letter.";

        case PASSWORD_NO_DIGIT:
            return "Password must contain at least one digit.";

        case PASSWORD_NO_SPECIAL:
            return "Password must contain at least one special character.";

        case INVALID_ROLE:
            return "Invalid role selected.";

        case STORAGE_ERROR:
            return "Unable to save user. Please try again.";

        default:
            return "Unknown error.";
    }
}

char *list_users()
{
    char *content = read_file(USERS_FILE);

    if (content == NULL)
        return strdup("No users found.\n");

    char *result = malloc(4096);

    if (result == NULL)
    {
        free(content);
        return strdup("Memory allocation failed.\n");
    }

    result[0] = '\0';

    strcat(result,
           "=========================================\n");
    strcat(result,
           "               USER LIST\n");
    strcat(result,
           "=========================================\n");
    strcat(result,
           "Username                  Role\n");
    strcat(result,
           "-----------------------------------------\n");

    User user;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_user(line, &user))
        {
            char row[128];

            snprintf(row,
                     sizeof(row),
                     "%-25s %s\n",
                     user.username,
                     role_to_string(user.role));

            strcat(result, row);
        }

        line = strtok(NULL, "\n");
    }

    strcat(result,
           "=========================================\n");

    free(save);

    return result;
}

