# Имя компилятора
CC=g++

# Стандарт
STD = -std=c++11

.PHONY: all

all: exec clear
 
exec: MqttChatHost.o main.o 
	$(CC) -o exec MqttChatHost.o main.o -lmosquitto -lmosquittopp
main.o: main.cpp
	$(CC) $(STD) -Wno-deprecated-declarations -c main.cpp -o main.o
MqttChatHost.o: MqttChatHost.hpp MqttChatHost.cpp
	$(CC) $(STD) -Wno-deprecated-declarations -c MqttChatHost.cpp -o MqttChatHost.o
clear:
	rm -rf *.o
