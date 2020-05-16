#pragma once

#include <iostream>
#include <map>
#include <queue>
#include <thread>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>


struct ClientData
{
	int cfd;
	sockaddr_in caddr;
};

class Socket
{
private:
	std::string SERV_IP;
	int SERV_PORT;
	sockaddr_in saddr;
	int sfd;
	ClientData clientdata;
	socklen_t caddr_len;

public:
	Socket() = delete;  // 删除默认构造函数
	Socket(const std::string SERV_IP, const int SERV_PORT) :SERV_IP(SERV_IP), SERV_PORT(SERV_PORT)
	{
		bzero(&saddr, sizeof(saddr));
		saddr.sin_family = AF_INET;
		// saddr.sin_addr.s_addr = inet_addr(SERV_IP.c_str());
		saddr.sin_addr.s_addr = INADDR_ANY;
		// inet_pton(AF_INET, SERV_IP.c_str(), &saddr.sin_addr.s_addr);
		saddr.sin_port = htons(SERV_PORT);

		// 创建套接字
		sfd = socket(AF_INET, SOCK_STREAM, 0);
		if (-1 == sfd)
		{
			perror("Create socket failed!");
			exit(1);
		}

		// 端口复用
		int opt = 1;
		setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		// 绑定
		int ret = bind(sfd, (sockaddr*)&saddr, sizeof(saddr));
		if (-1 == ret)
		{
			perror("Socket bind failed!");
			exit(1);
		}

		// 监听
		ret = listen(sfd, 88);
		if (-1 == ret)
		{
			perror("Socket listen error!");
			exit(1);
		}
	}
	~Socket() { close(sfd); }

	int getServerFd() { return sfd; }

	ClientData Accept()
	{
		caddr_len = sizeof(clientdata.caddr);
	again:
		clientdata.cfd = accept(sfd, (struct sockaddr*) & clientdata.caddr, &caddr_len);
		if (0 > clientdata.cfd)
		{
			if ((errno == ECONNABORTED) || (errno == EINTR))  // 被信号和软件层中断则重新accept
				goto again;
			else
			{
				perror("socket accept error!");
				exit(1);
			}
		}
		return clientdata;
	}
};

/* 包裹函数 */
ssize_t Read(int fd, void* ptr, unsigned nbytes)
{
	ssize_t n;
again:
	if (-1 == (n = read(fd, ptr, nbytes)))
	{
		if (errno == EINTR)  // 如果被信号中断，不应该退出
			goto again;
		else
			return -1;
	}
	return n;
}

ssize_t Write(int fd, const void* ptr, unsigned nbytes)
{
	ssize_t n;
again:
	if (-1 == (n = write(fd, ptr, nbytes)))
	{
		if (errno == EINTR)
			goto again;
		else
			return -1;
	}
	return n;
}

int Close(int fd)
{
	int n;
	if (-1 == (n = close(fd)))
	{
		perror("close error!");
		exit(1);
	}
	return n;
}

// 参数三：应该读取固定的字节数数据
ssize_t Readn(int fd, void* vptr, int n)
{
	unsigned nleft;  // 剩余未读取的字节数
	int nread;       // 实际上读取的字节数

	char* ptr = (char*)vptr;
	nleft = n;

	while (nleft > 0)
	{
		if (0 > (nread = read(fd, ptr, nleft)))
		{
			if (errno == EINTR)
				nread = 0;
			else
				return -1;
		}
		else if (nread == 0)
			break;
		nleft -= nread;
		ptr += nread;
	}
	return n - nleft;
}

ssize_t Writen(int fd, const void* vptr, unsigned n)
{
	ssize_t nleft;
	ssize_t nwritten;
	const char* ptr = (char*)vptr;

	nleft = n;

	while (nleft > 0)
	{
		if (0 >= (nwritten = write(fd, ptr, nleft)))
		{
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

static ssize_t my_read(int fd, char* ptr)
{
	static int read_cnt;
	static char* read_ptr;
	static char read_buf[100];

	if (read_cnt <= 0)
	{
	again:
		if (0 > (read_cnt = read(fd, read_buf, sizeof(read_buf))))
		{
			if (errno == EINTR)
				goto again;
			return -1;
		}
		else if (0 == read_cnt)
			return 0;
		read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr++;

	return 1;
}

ssize_t Readline(int fd, void* vptr, unsigned maxlen)
{
	ssize_t n, rc;
	char c, * ptr = (char*)vptr;

	for (n = 1; n < maxlen; ++n)
	{
		if (1 == (rc = my_read(fd, &c)))
		{
			*ptr++ = c;
			if ('\n' == c)
				break;
		}
		else if (0 == rc)
		{
			*ptr = 0;
			return n - 1;
		}
		else
			return -1;
	}
	*ptr = 0;
	return n;
}

// 连接网络类
class NetLink
{
private:
	int sfd;
	struct sockaddr_in addr;
	int port; // 端口号
public:
	NetLink(int port) :port(port) {}
	~NetLink() { close(sfd); }
	int getFd() { return sfd; }

	void initNetLink()
	{
		sfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sfd == -1)
		{
			std::cerr << "Create socket failed!" << std::endl;
			exit(1);
		}

		// 设置端口复用
		int opt = 1;
		setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = INADDR_ANY;

		if (-1 == bind(sfd, (struct sockaddr*) & addr, sizeof(addr)))
		{
			std::cerr << "Socket bind failed!" << std::endl;
			exit(1);
		}

		if (-1 == listen(sfd, 1024))
		{
			std::cerr << "Socket listen failed!" << std::endl;
			exit(1);
		}
	}
};

// MySQL数据库操作类
class SqlOperator
{
private:
	int ret;  // 保存错误码
	MYSQL mysql, * connect;
	std::queue<std::string> resultQueue;
	unsigned int num_fields;
public:
	~SqlOperator() { mysql_close(&mysql); }

	void initSql()
	{
		// 初始化，建立句柄
		connect = mysql_init(&mysql);
		if (NULL == connect)
		{
			ret = mysql_errno(&mysql);
			std::cerr << "func mysql_init() err: " << ret << std::endl;
			exit(1);
		}
		// 连接数据库
		connect = mysql_real_connect(connect, "xxx.xx.xx.xxx", "Koala", "xxxxx", "koalanotes", 0, NULL, 0);
		if (NULL == connect)
		{
			ret = mysql_errno(&mysql);
			std::cerr << "func mysql_real_connect() err: " << ret << std::endl;
			exit(1);
		}
	}

	std::queue<std::string> SqlQuery(const char* psql)
	{
		while (!resultQueue.empty()) resultQueue.pop();
		ret = mysql_query(&mysql, "set names utf8");
		if (ret != 0)
		{
			std::cerr << "mysql_query to utf-8 error\n";
		}

		ret = mysql_query(connect, psql);
		if (0 != ret)
		{
			std::cerr << "func mysql_query() err: " << ret << std::endl;
		}
		else
		{
			// 获取结果集合
			MYSQL_RES* result = mysql_store_result(connect);
			if (result == NULL)
			{
				ret = mysql_errno(connect);
				std::cout << "func mysql_store_result() err: " << ret << std::endl;
			}
	
			// 解析结果集合，一行一行地检索数据
			MYSQL_ROW row = NULL;
			while (row = mysql_fetch_row(result))
			{
				for (int i = 0; i < mysql_num_fields(result); ++i)
				{
					resultQueue.push(row[i]);
				}
			}
			// 释放结果集
			mysql_free_result(result);
		}
		return resultQueue;
	}

	int SqlInsert(const char* psql)
	{
		ret = mysql_query(connect, psql);
		if (0 != ret)
		{
			std::cerr << "func mysql_query() err: " << ret << std::endl;
			return 2;
		}
		else
			return 1;
	}

	int SqlDelete(const char* psql)
	{
		ret = mysql_query(connect, psql);
		if (0 != ret)
		{
			std::cerr << "func mysql_query() err: " << ret << std::endl;
			return 2;
		}
		else
			return 1;
	}

	int SqlModify(const char* psql)
	{
		ret = mysql_query(connect, psql);
		if (0 != ret)
		{
			std::cerr << "func mysql_query() err: " << ret << std::endl;
			return 2;
		}
		else
			return 1;
	}
};