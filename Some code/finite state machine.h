#ifndef ROBOTDELIVERY_FINITE_STATE_MACHINE_H
#define ROBOTDELIVERY_FINITE_STATE_MACHINE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mosquitto.h>
#include <string.h>
#include <json-c/json.h>

using namespace std;

enum State {EXPECTATION,SEARCHMOVEMENT,MOVING,ROTATING};
class MosquittoPub
{
private:
    int THREAD = 10;
    int NUM_MESSAGES = 30;
    string MQTT_SERVER = "localhost";
    int KEEP_ALIVE = 60;
    int MQTT_PORT = 1883;
    string MQTT_PUB_TOPIC = "/funmqqt";
    int MQTT_QOS_LEVEL = 2;
    int MSG_MAX_SIZE = 512;

public:
    void SendToServer (const char* data);
    void *Publish (const char* message);
};

void sendtoserver (const char* data) {
    bool clean_session = true;
    struct mosquitto *mosq = NULL;
    mosq = mosquitto_new( NULL, clean_session, NULL );
    mosquitto_connect(mosq, MQTT_SERVER, MQTT_PORT, KEEP_ALIVE);
    mosquitto_publish( mosq, NULL, MQTT_PUB_TOPIC, strlen(data), data, 0, 0 );
    mosquitto_destroy( mosq );
}

int main(){
    State cur_st = EXPECTATION;
    bool user_command = false;
    bool delivery_drink = false;
    bool position = false;
    bool rotation = false;
    while (!delivery_drink){
        switch (cur_st) {
            case (EXPECTATION):
                if (user_command) {
                    cur_st = SEARCHMOVEMENT;
                    position = false;
                }
                break;
            case (SEARCHMOVEMENT):
                if (position) {
                    cur_st = ROTATING;
                    rotation = false;
                }
                break;
            case (ROTATING):
                if (rotation) {
                    cur_st = MOVING;
                }
                break;
            case (MOVING):
                if (delivery_drink) {
                    cur_st = EXPECTATION;
                }
        }
    }
    if (cur_st == EXPECTATION){
        cout << "Enter the command to deliver the drink"<<endl;
        cin >> user_command;
    }
    if(cur_st == SEARCHMOVEMENT && !position){
        position = true;
    }
    if(cur_st == MOVING && !delivery_drink){
        delivery_drink = true;
    }
    if(cur_st == ROTATING && !rotation) {
        rotation = true;
    }
};
#endif //ROBOTDELIVERY_FINITE_STATE_MACHINE_H
