#ifndef STORAGE_H
#define STORAGE_H

// Simple file helpers
char *read_file(const char *filename);       // returns malloc'ed string or NULL
int append_line(const char *filename, const char *line); // append line + newline
int rewrite_file_atomic(const char *filename, const char *content); // overwrite atomically

#endif // STORAGE_H

