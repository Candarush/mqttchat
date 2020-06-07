#include "MqttChatClient.hpp"
#include <thread>
#include <iostream>
#include <string>

using namespace std;
using namespace MQTTW;

void LoopReadMessages(MqttChatClient* MQTT)
{
    MQTT->loop_forever();
}

int main()
{
    mosqpp::lib_init();
    string str,name,server;
    cout<<"Введите имя сервера: ";
    cin >> server;
    cout<<"Введите имя: ";
    cin >> name;
    MqttChatClient* MyMQTT = new MqttChatClient(name.c_str(), "localhost", 1883);
    MyMQTT->subscribetopic(server,name);
    std::thread t = std::thread(LoopReadMessages, MyMQTT);
    t.detach();
    MyMQTT->gettopics();
    cout<<"Введите топик:"<<endl;
    cin >> str;
    str = server+"/"+str;
    MyMQTT->joingroup(str, name);

    while(1)
    {
        if (getline(cin, str) && str.length()>0 && str[0]!='@')
        {
            if (!MyMQTT->send_message(str.c_str()))
            {
                cout<<"Ошибка: Сообщение не отправлено!"<<endl;
            }
        }
    }
}
