#include "Koala.h"

const int port = 9000;

class Signin
{
private:
	// 网络链接
	int i, j, n, maxi;
	NetLink netlink = NetLink(port);
	struct sockaddr_in addr;
	int sfd, cfd, maxfd;
	fd_set rset, allset;
	socklen_t caddr_len;
	int nready, client[FD_SETSIZE];
	char buf[BUFSIZ], str[INET_ADDRSTRLEN];

	// 数据库操作
	std::queue<std::string> resultQueue;
	SqlOperator sqlOperator;
    
    // 日志操作
    std::ofstream log;

public:
	Signin();
    ~Signin();
	void initSignin();
	void selectWork();
};

Signin::Signin()
{
	maxi = -1;
    log.open("signin.log", std::ios::out | std::ios::app);
}

Signin::~Signin()
{
    log.close();
}

void Signin::initSignin()
{
	netlink.initNetLink();
	sqlOperator.initSql();
}

void Signin::selectWork()
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
            log << "Received from IP " << inet_ntop(AF_INET, &addr.sin_addr.s_addr, str, sizeof(str)) << " PORT "<< ntohs(addr.sin_port) << std::endl;
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
					int index;
                    int len = strlen(buf);
					for (int k = 0; k < len; ++k)
					{
						if (buf[k] == '|')
						{
							index = k;
							break;
						}
					}
                    std::string password(buf, index + 1, len - index - 1);
					std::string str(buf, index);
					std::string psql = "select passwd, username, hdno from userdata where username = '" + str + "'";
					resultQueue = sqlOperator.SqlQuery(psql.c_str());
                    if(resultQueue.empty() != true)
                    {
                        if (password == resultQueue.front())
                        {
                            log << str << "成功登录！" << std::endl;
                            resultQueue.pop();
                            std::string username = resultQueue.front();
							resultQueue.pop();
							std::string hdno = resultQueue.front();
							std::string senddata = username + "|" + hdno;
							const char* data = senddata.c_str();
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
