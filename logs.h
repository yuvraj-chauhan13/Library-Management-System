#ifndef LOGS_H
#define LOGS_H

void write_log(const char *action,
               const char *details);

char *list_logs(void);

#endif