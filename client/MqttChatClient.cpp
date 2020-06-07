#include "/usr/local/include/mosquittopp.h"
#include "MqttChatClient.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <locale>
#include <vector>
#include <string>
#include <unistd.h>

namespace MQTTW
{
    MqttChatClient::MqttChatClient(const char *id, const char *host, int port) : mosquittopp(id)
    {
        int keepalive = 60;
        connect(host, port, keepalive);
        cout<<"Подключение."<<endl;
    };

    void MqttChatClient::subscribetopic(string itopic, string inickname)
    {
        topic = itopic;
        nickname = inickname;
        subscribe(NULL, topic.c_str());
    }

    void MqttChatClient::joingroup(string itopic, string inickname)
    {
        string joinmsg = "@topic "+itopic+" "+nickname;
        send_message(joinmsg.c_str());
        sleep(3);
        subscribetopic(itopic,inickname);
    }

    void MqttChatClient::on_message(const struct mosquitto_message *message){
        setlocale(LC_CTYPE, "rus");
        char buffer[256];
        sprintf(buffer,"%s",message->payload);
        std::string msg = buffer;
        if (msg[0]=='@')
        {
            processcommand(msg.substr(1,msg.length()-1));
        }
        else if(!strcmp(message->topic, topic.c_str()) && strcmp(msg.substr(0,nickname.length()).c_str(), nickname.c_str())){
            std::cout<<msg<<std::endl;
        }
    };

    bool MqttChatClient::send_message(const  char * message)
    {
        if (message[0]!='@')
        {
            string namedmessage = nickname + ": " + message;
            message = namedmessage.c_str();
        }
        int ret = publish(NULL,topic.c_str(),(int)strlen(message),message,0,false);
        return ( ret == MOSQ_ERR_SUCCESS );
    }

    void MqttChatClient::gettopics()
    {
        cout<<"Доступные топики:"<<endl;
        string str = "@gettopics "+nickname;
        send_message(str.c_str());
        sleep(1);
    }

    void MqttChatClient::processcommand(string cmd)
    {
        vector<string> cmdv = parse_command(cmd);
        if (cmdv[0]=="history" && cmd.find(nickname)!=string::npos)
        {
            cout<<cmd.substr(cmdv[0].length()+cmdv[1].length()+2,cmd.length()-(cmdv[0].length()+cmdv[1].length()+2))<<endl;
        }
        else if (cmdv[0]=="topics" && cmd.find(nickname)!=string::npos)
        {
            cout<<cmdv[2].substr(topic.length()+1,cmdv[2].length()-(topic.length()+1))<<endl;
        }
    }

    vector<string> parse_command(string cmd)
    {
        vector<string> output;
        int stringstart = 0;
        for (int i = 0; i<=cmd.length(); i++)
        {
            if (cmd[i]==' ')
            {
                output.push_back(cmd.substr(stringstart,i-stringstart));
                stringstart=i+1;
            }
            else if (i == cmd.length())
            {
                output.push_back(cmd.substr(stringstart,cmd.length()-stringstart));
            }
        }
        return output;
    }
}
