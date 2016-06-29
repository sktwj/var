#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "protocol.h"
#include <stddef.h>
#include <time.h>
#include "list.h"

#define  SRV_PORT  6666
#define  SRV_ID 	0

#define  MAX_NAME_SIZE	24

enum {
	S_OnLine,
	S_OffLine,
};

struct cli_list {
	struct list_head list;
	uint8_t uid;
	char    name[MAX_NAME_SIZE];
	struct  sockaddr_in cli_addr;
	uint8_t status; // Online OffLine  
};

struct server_info {
	struct list_head list;
	pid_t pid;
	int sock_fd;
	int count;//cli nums
} server;

int user_add(const char *username, struct sockaddr_in *cli_addr)
{
	struct cli_list *cli_node = (struct cli_list *)
			malloc(sizeof(struct cli_list));
	if (cli_node == NULL) {
		perror("malloc");
		return -1;
	}
	cli_node->uid = ++server.count;
	strncpy(cli_node->name, username, MAX_NAME_SIZE);
	memcpy(&cli_node->cli_addr, cli_addr, sizeof(cli_addr));
	cli_node->status = S_OnLine;

	//同步机制,确保链表操作有效 TODO
	list_add_tail(&server.list, &cli_node->list);

	printf("通知其他客户端，有新人加入\n");
	//通知其他客户端有新人加入
	struct list_head *cur;
	struct msg_info msg = {0}; 
	msg.mtype = M_INFO;
	time(&msg.mtime);
	msg.mtime = htonl(msg.mtime);

	list_for_each(cur, &server.list) {
		#if 0
		//不给自己发消息
		if (cur == &cli_node->list) {
			continue;
		}
		#endif
		struct cli_list *pcli = 
			list_entry(cur, struct cli_list, list);

		msg.dst_uid = pcli->uid;
		msg.src_uid = SRV_ID; //服务器端发出
		snprintf(msg.buf, BUF_SIZE,
		"new user %s:%d(%s) has join us\n,"
		"the num of online users is %d\n",
		 cli_node->name, cli_node->uid, 
		 inet_ntoa(cli_node->cli_addr.sin_addr),
		 server.count);
		//调试语句
		printf("%s\n", msg.buf);
		int n;
		n = sendto(server.sock_fd, &msg, offsetof(struct msg_info, buf) + strlen(msg.buf), 0, 
				(struct sockaddr *)&(pcli->cli_addr), sizeof(pcli->cli_addr));
		if (n == -1) {
			perror("sendto");
		}
	}


	return 0;
}

int cli_handler(const struct sockaddr_in *cli_addr, const struct msg_info *pmesg)
{
	switch (pmesg->mtype) {
		case M_REG:
				printf("注册请求\n");
				time_t t = ntohl(pmesg->mtime);
				printf("time:[%s]\n", ctime(&t));
				printf("buf:[%s]\n", pmesg->buf);
				//构建一个链表，分配uid给对应的客户端，
				//向所有客户端广播，通知有新人加入
				user_add(pmesg->buf, cli_addr);
				break;
		case M_ERR:
				fprintf(stderr,"[Error]:错误\n");
				break;
		case M_CHAT:
				printf("聊天\n");
				break;
		case M_LIST:
				printf("列表\n");
				break;
		default:
				fprintf(stderr, "[Error]:错误类型\n");
				break;
	}

}

int server_init()
{
	list_head_init(&server.list);
	server.pid = getpid();

	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1) {
		perror("socket");
		return -1;
	}

	struct sockaddr_in srv_addr, cli_addr;
	socklen_t addrlen = sizeof(srv_addr);
	bzero(&srv_addr, addrlen);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	inet_aton("0.0.0.0", &srv_addr.sin_addr);

	int iret = bind(sock_fd, (struct sockaddr *)&srv_addr, addrlen);
	server.sock_fd = sock_fd;
	server.count = 0; //客户端数量

	return sock_fd;
}
int main(int argc, char *argv[])
{
	int sock_fd = server_init();

	ssize_t n;
	#define BUF_SIZE   64*1024
	struct msg_info buf = {0};
	printf("server is running listen on udp:%d\n", SRV_PORT);
	struct sockaddr_in cli_addr;
	socklen_t addrlen =  sizeof(cli_addr);
	while (1) {
		n = recvfrom(sock_fd, &buf, offsetof(struct msg_info, buf) + BUF_SIZE, 0, (struct sockaddr *)&cli_addr, &addrlen);
		if (n == -1){
			perror("recvfrom");
			break;
		}
		printf("recv from (%s:%d)[%zd]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), n);
		
		cli_handler(&cli_addr, &buf);
	}

	close(sock_fd);

	return 0;
}
