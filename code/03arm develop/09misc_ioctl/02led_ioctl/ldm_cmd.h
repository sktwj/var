#pragma once

//查看arm-generic/ioctl.h，获得ioctl命令码的详细说明

#define CMD_ID 'X'

#define LED_ON _IOW(CMD_ID, 1, unsigned char)
#define LED_OFF _IO(CMD_ID, 2)
