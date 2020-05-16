#include "main.h"
#include "register.h"
#include "signin.h"

int main()
{
	pid_t pid;
	int i;
	for(i = 0; i < 3; ++i)
	{
		pid = fork();
		if(pid == 0)
			break;
	}
	if(i == 0)
	{
		Signin signin;
		signin.initSignin();
		signin.selectWork();
	}
	if(i == 1)
	{
		Signup signup;
		signup.initSignin();
		signup.selectWork();
	}
	if(i == 2)
	{
		Main ma;
		ma.epollWork();
	}
	else
	{
		int res;
		for(int j = 0; j < 3; ++j)
			wait(&res);
	}
	return 0;
}

