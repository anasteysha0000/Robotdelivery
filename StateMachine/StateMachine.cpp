#include <iostream>
using namespace std;

enum State { STANDBY, SEARCH_MODE, MOVE_MODE };

int main() {
    State currentState = STANDBY;
    bool userCommand = false;
    bool positionDetected = false;
    bool coffeeDelivered = false;

    while (!coffeeDelivered) {
        switch (currentState) {
            case STANDBY:
                if (userCommand) {
                    currentState = SEARCH_MODE;
                    positionDetected = false;
                }
                break;

            case SEARCH_MODE:
                if (positionDetected) {
                    currentState = MOVE_MODE;
                }
                break;

            case MOVE_MODE:
                if (coffeeDelivered) {
                    currentState = STANDBY;
                    userCommand = false;
                }
                break;
        }


        if (currentState == SEARCH_MODE && !positionDetected) {
            positionDetected = true;
        }

        if (currentState == MOVE_MODE && !coffeeDelivered) {
            coffeeDelivered = true;
        }


        if (currentState == STANDBY) {
            cout << "Enter a command to pour coffee: ";
            cin >> userCommand;
        }
    }

    cout << "Coffee delivered." << endl;

    return 0;
}