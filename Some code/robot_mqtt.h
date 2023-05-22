
#ifndef ROBOTDELIVERY_ROBOT_MQTT_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mosquitto.h>
#include <string.h>
#define NUM_MESSAGES 30
#define THREAD 10
#define KEEP_ALIVE 60
#define MQTT_PUB_TOPIC "Hellow, my name is Tolya, Im here here)"
#define MQTT_QOS_LEVEL 2
#define MSG_MAX_SIZE 512

class RobotControl {
private:
    int port;
    char* server = nullptr;

public:
    RobotControl(char *s, int port): port(port), server(s){}

    void send_server(const char* data){
        bool clean_session = true;
        struct mosquitto *mosq = NULL;
        mosq = mosquitto_new( NULL, clean_session, NULL );
        mosquitto_connect(mosq, server, port, KEEP_ALIVE);

        mosquitto_publish( mosq, NULL,
                           MQTT_PUB_TOPIC,
                           strlen(data),
                           data,
                           0,
                           0);

        mosquitto_destroy(mosq);
    }


    void move_forward(){
        std::cout << "Forward";
        send_server("{ \"cmd\":\"forward\", \"val\": 0.1, \"spd\": 0.3}");
    }
    void move_left(){
        std::cout << "Left";
        send_server("{ \"cmd\":\"left\", \"val\": 0.1, \"spd\": 0.3}");

    }
    void move_right(){
        std::cout << "Right"; //
        send_server("{ \"cmd\":\"right\", \"val\": 0.1, \"spd\": 0.3}");
    }
};
#define ROBOTDELIVERY_ROBOT_MQTT_H

#endif //ROBOTDELIVERY_ROBOT_MQTT_H
