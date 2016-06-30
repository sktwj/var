#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUF_SIZE	1024

#define   XPRINTF(level, fmt, args...) \
		fprintf(stderr, "[%s](%s:%d:%s)"fmt, level, __FILE__, __LINE__, __FUNCTION__, ##args)


#ifdef _DEBUG
#define  DBG_PRINTF(fmt, args...)	\
		XPRINTF("DEBUG", fmt, ##args)
#else
#define  DBG_PRINTF(fmt, args...)  
#endif


#define  ERR_PRINTF(fmt, args...)	\
		XPRINTF("ERR", fmt, ##args)

#define  MAX(a, b)	((a) > (b)? (a) : (b))
