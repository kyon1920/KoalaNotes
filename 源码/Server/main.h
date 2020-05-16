#include "Koala.h"

const std::string IP = "xxx.xx.xx.xxx";
const int PORT = 9002;
const int MAXLINE = 1024;
const int USER_MAX = 5000;
int CLIENT_NUM = 0;
int epfd;
epoll_event tep, ep[USER_MAX];
void dowork(int& fd);
void sql0Operator(char* data, int& fd, int& len);
void sql1Operator(char* data, int& fd, int& len);
void sql2Operator(char* data, int& fd, int& len);
void sql3Operator(char* data, int& fd, int& len);
void sql4Operator(char* data, int& fd, int& len);
void sql5Operator(char* data, int& fd, int& len);
void sql6Operator(char* data, int& fd, int& len);
void sql7Operator(char* data, int& fd, int& len);
void sql8Operator(char* data, int& fd, int& len);
void sql9Operator(char* data, int& fd, int& len);
void otherOperator(char* data, int& fd, int& len);

// std::map<int, std::thread> thmap;
int dfd[USER_MAX / 10];

// 数据库操作
SqlOperator sqlOperator;
std::queue<std::string> resultQueue;

class Main
{
private:
	Socket* ssock;
	int sfd, ret, nready;

public:
	Main();
	~Main();
	void epollWork();
};

inline Main::Main()
{
	for (int i = 0; i < USER_MAX / 10; ++i)
		dfd[i] = -1;
	ssock = new Socket(IP, PORT);
	sfd = ssock->getServerFd();
	sqlOperator.initSql();
}

Main::~Main()
{
	delete ssock;
}

void Main::epollWork()
{
	epfd = epoll_create(USER_MAX);
	if (-1 == epfd)
	{
		perror("epoll_create failed!");
		exit(1);
	}

	tep.events = EPOLLIN;
	tep.data.fd = sfd;

	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &tep);
	if (-1 == ret)
	{
		perror("62 epoll_ctl failed!");
		exit(1);
	}

Again:
	while (1)
	{
		nready = epoll_wait(epfd, ep, USER_MAX, -1);
		if (-1 == nready)
		{
			perror("epoll_wait failed!");
			exit(1);
		}
		for (int i = 0; i < nready; ++i)
		{
			if (!(ep[i].events & EPOLLIN))
			{
				continue;
			}

			if (ep[i].data.fd == sfd)
			{
				CLIENT_NUM++;
				if (CLIENT_NUM > USER_MAX)
				{
					std::cout << "Too many clients!" << std::endl;
					goto Again;
				}
				ClientData cd;
				char cdbuff[INET_ADDRSTRLEN];
				cd = ssock->Accept();
				std::cout << "Received from IP " << inet_ntop(AF_INET, &cd.caddr.sin_addr.s_addr, cdbuff, sizeof(cdbuff))
					<< " and PORT " << ntohs(cd.caddr.sin_port) << std::endl;
				
				tep.events = EPOLLIN;
				tep.data.fd = cd.cfd;
				ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cd.cfd, &tep);
				if (-1 == ret)
				{
					perror("101 epoll_ctl failed!");
					exit(1);
				}

				/*int cfd = ep[i].data.fd;
				thmap.insert(make_pair(cfd, std::thread(dowork, std::ref(cfd))));*/
			}
			else
			{
				int cfd = ep[i].data.fd;
				dowork(cfd);
			}
		}
		/*for (int i = 0; i < USER_MAX / 10; ++i)
		{
			if (dfd[i] > 0)
			{
				auto it = thmap.find(dfd[i]);
				it->second.join();
				thmap.erase(it);
				dfd[i] = -1;
			}
		}*/
	}
}

void dowork(int& fd)
{
	char buf[MAXLINE];
	int n, ret;

	n = Readn(fd, buf, 100);
	std::cout << buf << std::endl;
	if (n <= 0)
	{
		CLOSE:
		for (int i = 0; i < USER_MAX / 10; ++i)
			if (dfd[i] == -1)
			{
				dfd[i] = fd;
				break;
			}

		ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
		if (-1 == ret)
		{
			perror("148 epoll_ctl failed!");
			exit(1);
		}
		close(fd);
	}
	else if (n > 0)
	{
		std::string strr;
		for (int i = 0; i < 5, buf[i] != '*'; ++i)
			strr += buf[i];
		int len = atoi(strr.c_str());
		char* data = new char[len];
		n = Readn(fd, data, len);
		std::cout << data << std::endl;
		
		if (n <= 0)
			goto CLOSE;
		int swith_n = atoi(std::string(data, 0, 1).c_str());
		std::cout << swith_n << std::endl;
		switch (swith_n)
		{
		case 0:
			sql0Operator(data, fd, len);
			break;
		case 1:
			sql1Operator(data, fd, len);
			break;
		case 2:
			sql2Operator(data, fd, len);
			break;
		case 3:
			sql3Operator(data, fd, len);
			break;
		case 4:
			sql4Operator(data, fd, len);
			break;
		case 5:
			sql5Operator(data, fd, len);
		case 6:
			sql6Operator(data, fd, len);
			break;
		case 7:
			sql7Operator(data, fd, len);
			break;
		case 8:
			sql8Operator(data, fd, len);
			break;
		case 9:
			sql9Operator(data, fd, len);
			break;
		default:
			// otherOperator(data, fd, len);
			break;
		}
		
		std::string psql;
		std::string n(data, 1);
		if(n == "d")
		{
			std::string str;
			for(int i = 2; i < len; ++i)
				str += data[i];
			psql = "DELETE FROM userdata WHERE username = '" + str + "';";
			int re = sqlOperator.SqlInsert(psql.c_str());
			std::cout << psql << std::endl;
			psql = "DROP TABLE " + str + "_notes;";
			std::cout << psql << std::endl;
			int ret = sqlOperator.SqlInsert(psql.c_str());
			psql = "DROP TABLE " + str + "_follow;";
			std::cout << psql << std::endl;
			int ret1 = sqlOperator.SqlInsert(psql.c_str());
			psql = "DROP TABLE " + str + "_share;";
			std::cout << psql << std::endl;
			int ret2 = sqlOperator.SqlInsert(psql.c_str());
			if(1 == ret && ret1 == 1 && 1 == ret2 && re == 1)
				std::cout << "注销成功！" << std::endl;
		}
	}
}

void sql0Operator(char* data, int& fd, int& len)
{
	std::string result;
	std::string psql;
	std::string str;
	
	for(int i = 2; i < len; ++i)
		str += data[i];
	psql = "SELECT title, dates FROM " + str + "_notes;";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		result = "0|" + resultQueue.front();
		resultQueue.pop();
	}
	
	while (resultQueue.empty() != true)
	{
		result.append("|" + resultQueue.front());
		resultQueue.pop();
	}
	std::cout << result << std::endl;
	const char* sendData = result.c_str();
	send(fd, sendData, strlen(sendData), 0);
}

void sql1Operator(char* data, int& fd, int& len)
{
	std::string result;
	std::string psql;
	std::string str[3];
	int i;
	int l = 2;
	int j = 0;
	for(i = 2; i < len; ++i)
	{
		if(data[i] == '|')
		{
			str[j] = std::string(data+l, i - l);
			l = i + 1;
			j++;
		}
	}
	str[j] = std::string(data+l, i - l);
	
	psql = "SELECT content FROM " + str[0] + "_notes where title = '" + str[1] + "' and dates = '" + str[2] +"';";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		result = "1|" + resultQueue.front();
		resultQueue.pop();
	}
	
	while (resultQueue.empty() != true)
	{
		result.append("|" + resultQueue.front());
		resultQueue.pop();
	}
	std::cout << result << std::endl;
	const char* sendData = result.c_str();
	send(fd, sendData, strlen(sendData), 0);
}

void sql2Operator(char* data, int& fd, int& len)
{
	std::string psql;
	std::string str[5];
	int i;
	int l = 2;
	int j = 0;
	for(i = 2; i < len; ++i)
	{
		if(data[i] == '|')
		{
			str[j] = std::string(data+l, i - l);
			l = i + 1;
			j++;
		}
	}
	str[j] = std::string(data+l, i - l);
	
	psql = "INSERT INTO " + str[0] + "_notes values ('" + str[1] + "','" + str[2] + "','" + str[3] + "','" + str[4] + "');";
	std::cout << psql << std::endl;
	int ret = sqlOperator.SqlInsert(psql.c_str());
	if (ret == 1)
		send(fd, "2|1", strlen("2|1"), 0);
	else
		send(fd, "2|0", strlen("2|0"), 0);
}

void sql3Operator(char* data, int& fd, int& len)
{
	std::string psql;
	std::string str[7];
	int i;
	int l = 2;
	int j = 0;
	for(i = 2; i < len; ++i)
	{
		if(data[i] == '|')
		{
			str[j] = std::string(data+l, i - l);
			l = i + 1;
			j++;
		}
	}
	str[j] = std::string(data+l, i - l);
	
	psql = "UPDATE " + str[0] + "_notes SET title = '" + str[1] + "', dates = '" + str[2] + "', private =  '" + str[3] + "', content = '" + str[4] + "' WHERE title = '" + str[5] + "' AND dates = '" + str[6] + "';";
	std::cout << psql << std::endl;
	int ret = sqlOperator.SqlModify(psql.c_str());
	if (ret == 1)
		send(fd, "3|1", strlen("3|1"), 0);
	else
		send(fd, "3|0", strlen("3|0"), 0);
}

void sql4Operator(char* data, int& fd, int& len)
{
	std::string result;
	std::string psql;
	std::string str;
	
	for(int i = 2; i < len; ++i)
		str += data[i];
	psql = "SELECT username, hdno FROM " + str + "_follow;";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		result = "4|" + resultQueue.front();
		resultQueue.pop();
	}
	
	while (resultQueue.empty() != true)
	{
		result.append("|" + resultQueue.front());
		resultQueue.pop();
	}
	std::cout << result << std::endl;
	const char* sendData = result.c_str();
	send(fd, sendData, strlen(sendData), 0);
}

void sql5Operator(char* data, int& fd, int& len)
{
	std::string result;
	std::string psql;
	std::string str;
	
	for(int i = 2; i < len; ++i)
		str += data[i];
	psql = "SELECT title, dates FROM " + str + "_notes WHERE private = '0';";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		result = "5|" + resultQueue.front();
		resultQueue.pop();
	}
	
	while (resultQueue.empty() != true)
	{
		result.append("|" + resultQueue.front());
		resultQueue.pop();
	}
	std::cout << result << std::endl;
	const char* sendData = result.c_str();
	send(fd, sendData, strlen(sendData), 0);
}

void sql6Operator(char* data, int& fd, int& len)
{
	std::string result;
	std::string psql;
	std::string str[3];
	int i;
	int l = 2;
	int j = 0;
	for(i = 2; i < len; ++i)
	{
		if(data[i] == '|')
		{
			str[j] = std::string(data+l, i - l);
			l = i + 1;
			j++;
		}
	}
	str[j] = std::string(data+l, i - l);
	
	psql = "SELECT content FROM " + str[0] + "_notes where title = '" + str[1] + "' and dates = '" + str[2] +"';";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		result = "6|" + resultQueue.front();
		resultQueue.pop();
	}
	
	while (resultQueue.empty() != true)
	{
		result.append("|" + resultQueue.front());
		resultQueue.pop();
	}
	std::cout << result << std::endl;
	const char* sendData = result.c_str();
	send(fd, sendData, strlen(sendData), 0);
}

void sql7Operator(char* data, int& fd, int& len)
{
	int result;
	std::string username;
	std::string hdno;
	std::string psql;
	std::string str[2];
	int i;
	int l = 2;
	int j = 0;
	for(i = 2; i < len; ++i)
	{
		if(data[i] == '|')
		{
			str[j] = std::string(data+l, i - l);
			l = i + 1;
			j++;
		}
	}
	str[j] = std::string(data+l, i - l);
	
	psql = "SELECT username, hdno FROM userdata  where username = '" + str[1] + "';";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		result = 1;
		username = resultQueue.front();
		resultQueue.pop();
		hdno = resultQueue.front();
		resultQueue.pop();
	}
	
	psql = "INSERT INTO " + str[0] + "_follow values('" + str[1] + "','" + hdno + "');";
	int ret = sqlOperator.SqlInsert(psql.c_str());
	if (ret == 1 && result == 1)
		send(fd, "7|1", strlen("7|1"), 0);
	else
		send(fd, "7|0", strlen("7|0"), 0);
}

void sql8Operator(char* data, int& fd, int& len)
{
	int result, rets;
	std::string username;
	std::string hdno;
	std::string title;
	std::string text;
	std::string psql;
	std::string str[4];
	int i;
	int l = 2;
	int j = 0;
	for(i = 2; i < len; ++i)
	{
		if(data[i] == '|')
		{
			str[j] = std::string(data+l, i - l);
			l = i + 1;
			j++;
		}
	}
	str[j] = std::string(data+l, i - l);
	
	psql = "SELECT username, hdno FROM userdata  where username = '" + str[1] + "';";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		result = 1;
		username = resultQueue.front();
		resultQueue.pop();
		hdno = resultQueue.front();
		resultQueue.pop();
	}
	psql = "SELECT title, content FROM " + str[0] + "_notes where title = '" + str[2] + "' and dates = '" + str[3] +"';";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		rets = 1;
		title = resultQueue.front();
		resultQueue.pop();
		text = resultQueue.front();
		resultQueue.pop();
	}
	
	psql = "INSERT INTO " + str[0] + "_share values('" + str[1] + "','" + hdno + "', '1'" + ",'" + text + "','" + title + "');";
	std::cout << psql << std::endl;
	int ret = sqlOperator.SqlInsert(psql.c_str());
	
	psql = "INSERT INTO " + str[1] + "_share values('" + str[0] + "','" + hdno + "', '0'" + ",'" + text + "','" + title + "');";
	std::cout << psql << std::endl;
	int ret1 = sqlOperator.SqlInsert(psql.c_str());
	
	if (ret == 1 && result == 1 && rets == 1 && ret1 == 1)
		send(fd, "8|1", strlen("8|1"), 0);
	else
		send(fd, "8|0", strlen("8|0"), 0);
}

void sql9Operator(char* data, int& fd, int& len)
{
	std::string result;
	std::string psql;
	std::string str;
	for(int i = 2; i < len; ++i)
		str += data[i];
	psql = "SELECT myshared, username, hdno, title, text FROM " + str + "_share;";
	std::cout << psql << std::endl;
	resultQueue = sqlOperator.SqlQuery(psql.c_str());
	if (resultQueue.empty() != true)
	{
		result = "9|" + resultQueue.front();
		resultQueue.pop();
	}
	
	while (resultQueue.empty() != true)
	{
		result.append("|" + resultQueue.front());
		resultQueue.pop();
	}
	std::cout << result << std::endl;
	const char* sendData = result.c_str();
	send(fd, sendData, strlen(sendData), 0);
}

void otherOperator(char* data, int& fd, int& len)
{
}