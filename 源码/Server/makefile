src=$(wildcard ./*.cpp)
target=$(patsubst ./%.cpp,./%,$(src))

object:$(target)

%:%.cpp
	g++ $< -o $@ -I /www/server/mysql/include/ -L /www/server/mysql/lib/ -l mysqlclient -ldl -lpthread -lm -lrt -lstdc++ -Wall -std=c++11 -O2
