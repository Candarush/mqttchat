# Имя компилятора
CC=g++

# Стандарт
STD = -std=c++11

.PHONY: all

all: exec clear
 
exec: MqttChatClient.o main.o 
	$(CC) -o exec MqttChatClient.o main.o -lmosquitto -lmosquittopp
main.o: main.cpp
	$(CC) $(STD) -Wno-deprecated-declarations $(OPENCVINCLUDES) -c main.cpp -o main.o
MqttChatClient.o: MqttChatClient.hpp MqttChatClient.cpp
	$(CC) $(STD) -Wno-deprecated-declarations -c MqttChatClient.cpp -o MqttChatClient.o
clear:
	rm -rf *.o
