#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


char *read_file(const char *filename){
    FILE *f = fopen(filename, "r");
    if(!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(sz + 1);
    if(!buf){ fclose(f); return NULL; }
    size_t r = fread(buf, 1, sz, f);
    buf[r] = '\0';
    fclose(f);
    return buf;
}

int append_line(const char *filename, const char *line){
    FILE *f = fopen(filename, "a");
    if(!f) return -1;
    if(fprintf(f, "%s\n", line) < 0){ fclose(f); return -1; }
    fclose(f);
    return 0;
}

int rewrite_file_atomic(const char *filename, const char *content){
    char tmp[512];
    snprintf(tmp, sizeof(tmp), "%s.tmp", filename);
    FILE *f = fopen(tmp, "w");
    if(!f) return -1;
    if(fputs(content, f) == EOF){ fclose(f); unlink(tmp); return -1; }
    fclose(f);
    if(rename(tmp, filename) != 0) { unlink(tmp); return -1; }
    return 0;
}

