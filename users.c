#include "users.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define USERS_FILE "data/users.txt"

// authenticate: users.txt lines -> username|password|role
int authenticate_user(const char *username, const char *password){
    char *s = read_file(USERS_FILE);
    if(!s) return 0;
    int role = 0;
    char *save = s;
    char *line = strtok(s, "\n");
    while(line){
        char u[128], p[128]; int r;
        if(sscanf(line, "%127[^|]|%127[^|]|%d", u, p, &r) == 3){
            if(strcmp(u, username) == 0 && strcmp(p, password) == 0){
                role = r;
                break;
            }
        }
        line = strtok(NULL, "\n");
    }
    free(save);
    return role;
}

int register_user(const char *username, const char *password, int role){
    // naive: no duplicate check (we can add if needed)
    char line[512];
    snprintf(line, sizeof(line), "%s|%s|%d", username, password, role);
    return append_line(USERS_FILE, line);
}

char *list_users(){
    char *s = read_file(USERS_FILE);
    if(!s) return strdup("No users.\n");
    return s;
}

