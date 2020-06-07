#include "/usr/local/include/mosquittopp.h"
#include "MqttChatHost.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <locale>
#include <vector>
#include <string>

template<typename T>
void pop_front(vector<T>& vec)
{
    assert(!vec.empty());
    vec.erase(vec.begin());
}

namespace MQTTW
{

    MqttChatHost::MqttChatHost(const char *id, const char *host, int port) : mosquittopp(id)
    {
        int keepalive = 60;
        connect(host, port, keepalive);
        cout<<"Подключение."<<endl;
    };

    void MqttChatHost::hostsubscribetopic(string itopic, string inickname)
    {
        topic = itopic;
        string subscribetopic = itopic + "/#";
        nickname = inickname;
        subscribe(NULL, subscribetopic.c_str());
    }

    void MqttChatHost::on_message(const struct mosquitto_message *message){
        setlocale(LC_CTYPE, "rus");
        char buffer[256];
        sprintf(buffer,"%s",message->payload);
        std::string msg = buffer;
        if (msg[0]=='@')
        {
            processcommand(msg.substr(1,msg.length()-1));
        }
        else
        {
            msgtohistory(message->topic,msg);
        }
        std::cout<<message->topic<< "/ "+msg<<std::endl;
    };

    bool MqttChatHost::send_message(const  char * message)
    {
        cout<< "Отправляю: " << message << endl;
        int ret = publish(NULL,topic.c_str(),(int)strlen(message),message,0,false);
        return ( ret == MOSQ_ERR_SUCCESS );
    }

    void MqttChatHost::msgtohistory(string group, string msg)
    {
        for (int i = 0; i<groups.size(); i++)
        {
            if (groups[i].name==group)
            {
                cout<<"Записал: \""<<msg<<"\" в топик "<<group<<endl;
                groups[i].messages.push_back(msg);
                if (groups[i].messages.size()>20)
                {
                    pop_front(groups[i].messages);
                }
            }
        }
    }

    void MqttChatHost::processcommand(string cmd)
    {
        vector<string> cmdv = parse_command(cmd);
        if (cmdv[0]=="gettopics")
        {
            {
                cout<<"Количесво топиков: "<<groups.size()<<endl;
                for (int k = 0; k<groups.size();k++)
                {
                    string messagetopics = "@topics "+ cmdv[1] + " " + groups[k].name;
                    send_message(messagetopics.c_str());
                }
            }
        }
        else if (cmdv[0]=="topic")
        {
            for (int i = 0; i<groups.size(); i++)
            {
                if (groups[i].name==cmdv[1])
                {
                    groups[i].users.push_back(cmdv[2]);
                    string messagehistory;
                    for (int k = 0; k<groups[i].messages.size();k++)
                    {
                        messagehistory = "@history "+ cmdv[2] + " " + groups[i].messages[k];
                        send_message(messagehistory.c_str());
                    }
                    return;
                }
            }
            cout<<cmdv[1]<<endl;
            groups.push_back(Group());
            groups.back().name = cmdv[1];
            groups.back().users.push_back(cmdv[2]);
            
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
