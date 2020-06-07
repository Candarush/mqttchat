#pragma once

#include "/usr/local/include/mosquittopp.h"
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

namespace MQTTW
{
    class Group
    {
        public:
        string name;
        vector<string> users;
        vector<string> messages;
    };

    class MqttChatHost : public mosqpp::mosquittopp
    {
        string nickname;
        string topic;
        vector<Group> groups;
        public:
        MqttChatHost(char const  *id, const char *host, int port);

        void on_message(const struct mosquitto_message *message);
        bool send_message(const  char * message);
        
        void hostsubscribetopic(string topic, string nickname);
        void msgtohistory(string group, string msg);
        void processcommand(string command);
    };

    vector<string> parse_command(string command);
}
