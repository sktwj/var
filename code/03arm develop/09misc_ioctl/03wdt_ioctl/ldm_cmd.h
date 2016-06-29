#pragma once

//查看arm-generic/ioctl.h，获得ioctl命令码的详细说明

#define CMD_ID 'X'

#define WDT_ON _IO(CMD_ID, 1)
#define WDT_OFF _IO(CMD_ID, 2)
#define WDT_CFG_INT _IO(CMD_ID, 3)
#define WDT_CFG_RESET _IO(CMD_ID, 4)
#define WDT_SET_CYCLE _IOW(CMD_ID, 5, unsigned long)

