#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "server.h"
#include "up_log.h"

FILE *log_pf = NULL;

void mylog(const char* format, ... )
{
    time_t tmp = time( NULL );
    struct tm* cur_time = localtime( &tmp );
    if ( ! cur_time )
    {
        return;
    }

    char arg_buffer[ LOG_BUFFER_SIZE ];
    memset( arg_buffer, '\0', LOG_BUFFER_SIZE );
    strftime( arg_buffer, LOG_BUFFER_SIZE - 1, "[ %x %X ] ", cur_time );
    int n = strlen(arg_buffer);

    va_list arg_list;
    va_start( arg_list, format );
    vsnprintf( arg_buffer + n, LOG_BUFFER_SIZE - 1 - n, format, arg_list);
    fprintf(stderr, "%s", arg_buffer );
    va_end(arg_list);
}

int log_init(const char *logfile)
{
    log_pf = fopen(logfile, "w+");
    if (log_pf == NULL) {
        ERR_PRINTF("open log file %s failed.\n", logfile);
        return -1;
    }

    return 0;
}

int log_destroy()
{
   return fclose(log_pf);
}