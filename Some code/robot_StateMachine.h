
#ifndef ROBOTDELIVERY_ROBOT_STATEMACHINE_H
#include <memory>
#include "Camera.h"
#include <unistd.h>
#include "robot_mqtt.h"

using namespace std;
bool order_exists = true;

enum State {Wait, Move ,MoveToTarget, RotateLeft, RotateRight, Forward, Finish};

class StateMachine{

    Camera cam;
    RobotControl robot_moving;
    State state;
    int iter = 0;

    void processEvents();

public:

    StateMachine(Camera d);

    void run();

};



StateMachine::StateMachine(Camera camera){
    this->cam = camera;
    state = Wait;
}


void StateMachine::run() {
    while (1){
        this->cam.define_all();

        iter++;
        this->processEvents();


        char c=(char)waitKey(25);
        if(c==27)
            break;
    }
}


void StateMachine::processEvents() {
    switch (this->state) {
        case Wait:
            if (order_exists) {
                this->state = MoveToTarget;
            }
            break;

        case Move:
            this->state = MoveToTarget;
            break;

        case MoveToTarget:
            if(cam.has_angle_to_target(5)){
                if (cam.getAngleTarget() > 0)
                    this->state = RotateLeft;
                else
                    this->state = RotateRight;
            }

            if(cam.has_distance_to_target(160))
                this->state = Forward;
            else
                this->state = Finish;
            break;

        case RotateLeft:
            // robot_moving->move_left();
            this->state = Move;
            break;

        case RotateRight:
            // robot_moving->move_right();
            this->state = Move;
            break;

        case Forward:
            // robot_moving->move_forward();
            this->state = Move;
            break;

        case Finish:
            cout << "Finish!!!" << '\n';
            sleep(30);
            this->state = Wait;
            break;
    }
}

#define ROBOTDELIVERY_ROBOT_STATEMACHINE_H

#endif //ROBOTDELIVERY_ROBOT_STATEMACHINE_H
