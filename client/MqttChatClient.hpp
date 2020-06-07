#pragma once

#include "/usr/local/include/mosquittopp.h"
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

namespace MQTTW
{
    class MqttChatClient : public mosqpp::mosquittopp
    {
        string topic;
        string nickname;
        vector<string> history;
        int messages;
        vector<string> topics;
        
        public:
        MqttChatClient(char const  *id, const char *host, int port);

        void on_message(const struct mosquitto_message *message);
        bool send_message(const  char * message);
        
        void subscribetopic(string topic, string nickname);
        void joingroup(string topic, string nickname);
        void gettopics();
        void processcommand(string command);
    };

    vector<string> parse_command(string command);
}
