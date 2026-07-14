#include "logs.h"
#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOG_FILE "data/logs.txt"

static void current_time(char *buffer, size_t size)
{
    time_t now = time(NULL);

    struct tm *tm_info = localtime(&now);

    strftime(buffer,
             size,
             "%Y-%m-%d %H:%M:%S",
             tm_info);
}

void write_log(const char *action,
               const char *details)
{
    char timestamp[32];

    current_time(timestamp, sizeof(timestamp));

    char line[512];

    snprintf(line,
             sizeof(line),
             "%s | %s | %s",
             timestamp,
             action,
             details);

    append_line(LOG_FILE, line);
}

char *list_logs(void)
{
    char *logs = read_file(LOG_FILE);

    if (logs == NULL)
        return strdup("No logs found.\n");

    return logs;
}