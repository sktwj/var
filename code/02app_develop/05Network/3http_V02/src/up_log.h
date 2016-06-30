#pragma once

#define LOG_BUFFER_SIZE   128

void mylog(const char* format, ... );
int log_init(const char *logfile);
int log_destroy();