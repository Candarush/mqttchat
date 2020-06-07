#include "MqttChatHost.hpp"
#include <thread>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;
using namespace MQTTW;

int main()
{
    mosqpp::lib_init();
    string servername;
    cout<<"Введите имя сервера: ";
    cin >> servername;
    MqttChatHost* MyMQTT = new MqttChatHost(servername.c_str(), "localhost", 1883);
    MyMQTT->hostsubscribetopic(servername,"host");
    MyMQTT->loop_forever();
}
