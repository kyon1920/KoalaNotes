#include "Koala.h"

const int port1 = 9001;

class Signup
{
private:
	// 网络链接
	int i, j, n, maxi;
	NetLink netlink = NetLink(port1);
	struct sockaddr_in addr;
	int sfd, cfd, maxfd;
	fd_set rset, allset;
	socklen_t caddr_len;
	int nready, client[FD_SETSIZE];
	char buf[BUFSIZ], str[INET_ADDRSTRLEN];

	// 数据库操作
	SqlOperator sqlOperator;

	// 日志操作
	std::ofstream log;

public:
	Signup();
	~Signup();
	void initSignin();
	void selectWork();
};

Signup::Signup()
{
	maxi = -1;
	log.open("register.log", std::ios::out | std::ios::app);
}

Signup::~Signup()
{
	log.close();
}

void Signup::initSignin()
{
	netlink.initNetLink();
	sqlOperator.initSql();
}

void Signup::selectWork()
{
	sfd = netlink.getFd();
	maxfd = sfd;

	for (i = 0; i < FD_SETSIZE; ++i)
		client[i] = -1;

	FD_ZERO(&allset);
	FD_SET(sfd, &allset);

	while (1)
	{
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (nready < 0)
		{
			log << "Select error!" << std::endl;
			exit(1);
		}
		if (FD_ISSET(sfd, &rset))
		{
			caddr_len = sizeof(addr);
			cfd = accept(sfd, (struct sockaddr*) & addr,
				&caddr_len);
			log << "Received from IP " << inet_ntop(AF_INET, &addr.sin_addr.s_addr, str, sizeof(str)) << " PORT " << ntohs(addr.sin_port) << std::endl;
			for (i = 0; i < FD_SETSIZE; ++i)
			{
				if (client[i] < 0)
				{
					client[i] = cfd;
					break;
				}
			}
			if (i == FD_SETSIZE)
			{
				log << "Too many clients, " << stderr << std::endl;
				exit(1);
			}

			FD_SET(cfd, &allset);
			if (cfd > maxfd)
			{
				maxfd = cfd;
			}

			if (i > maxi)
			{
				maxi = i;

			}
			if (--nready == 0)
			{
				continue;
			}
		}
		for (i = 0; i <= maxi; ++i)
		{
			int sockfd;
			if ((sockfd = client[i]) < 0)
			{
				continue;
			}
			if (FD_ISSET(sockfd, &rset))
			{
				if ((n = read(sockfd, buf, sizeof(buf))) == 0)
				{
					close(sockfd);
					log << "The client " << inet_ntop(AF_INET, &addr.sin_addr.s_addr, str, sizeof(str)) << " closed!" << std::endl;
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
				else if (n > 0)
				{
					int len = strlen(buf);
					int x, y = 0, z = 0;
					std::string userdata[3];

					for (x = 0; x <= len; ++x)
					{
						if (buf[x] == '|' || x == len)
						{
							userdata[y] = std::string(buf, z, x - z);
							y++;
							z = x + 1;
						}
					}
                    
					std::string psql = "insert into userdata(username, passwd, hdno) values('" + userdata[0] + "', '" + userdata[1] + "', '" + userdata[2] + "');";
					int result = 0;
                    result = sqlOperator.SqlInsert(psql.c_str());
                    
					if (result == 1)
					{
						psql = "CREATE TABLE " + userdata[0] + "_follow (username varchar(32) PRIMARY KEY, hdno char(1));";
						result = sqlOperator.SqlInsert(psql.c_str());
						if(result != 1)
							goto FAILED;
						psql = "CREATE TABLE " + userdata[0] + "_notes (title varchar(256), dates varchar(64), private char(1), content varchar(20480));";
						result = sqlOperator.SqlInsert(psql.c_str());
						if(result != 1)
							goto FAILED;
						psql = "CREATE TABLE " + userdata[0] + "_share (username varchar(32), hdno char(1), myshared char(1), text varchar(20480), title varchar(256));";
						result = sqlOperator.SqlInsert(psql.c_str());
						if(result != 1)
							goto FAILED;
						
						psql = "insert into " + userdata[0] + "_follow values('Koala', '1')";
						result = sqlOperator.SqlInsert(psql.c_str());
						
						psql = "insert into " + userdata[0] + "_share values('Koala', '1', '0', 'Welcome to the sharing function ~', 'Welcome to the sharing function!');";
						result = sqlOperator.SqlInsert(psql.c_str());
					
						log << str << "注册成功！" << std::endl;
						const char* data = "1";
						if (-1 == write(sockfd, data, strlen(data)))
						{
							log << "Sockfd write failed!" << std::endl;
						}
						else
						{
							log << "Sockfd write success!" << std::endl;
						}
					}
                    else
                    {
						FAILED:
                        const char* data = "0";
                        if (-1 == write(sockfd, data, strlen(data)))
                        {
                            log << "Sockfd write failed!" << std::endl;
                        }
                        else
                        {
                            log << "Sockfd write success!" << std::endl;
                        }
                    }
                    
					if (--nready == 0)
					{
						break;
					}
				}
			}
		}
	}
}
