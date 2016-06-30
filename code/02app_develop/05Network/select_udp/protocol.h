#pragma once

#ifndef BUF_SIZE
#define BUF_SIZE	1024	
#endif


enum msg_type {
	M_ERR = 0, //错误  
	M_REG, 	 //注册
	M_CHAT,  //聊天
	M_BROAD, //广播
	M_QUIT,  //退出
	M_LIST,  //列表
	M_INFO,  //服务器返回给客户端
};

struct msg_info {
	uint8_t mtype; //消息类型
	time_t mtime; //时间戳
	uint8_t src_uid; //发送者的ID
	uint8_t dst_uid; //接收者的ID
	char buf[BUF_SIZE]; //消息
};
