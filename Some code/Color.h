#ifndef MAI_COLOR_H
#define MAI_COLOR_H

#include <string>

using namespace std;

class Color{
public:
    int R;
    int R1;
    int G;
    int G1;
    int B;
    int B1;

    Color(const string &name) {
        if (name == "red") {
            R = 161; R1 = 179; G = 135; G1 = 255; B = 199; B1 = 255;
        }
        if (name == "blue") {
            R = 75; R1 = 130; G = 150; G1 = 255; B = 60; B1 = 255;
        }
        if (name == "yellow") {
            R = 22; R1 = 38; G = 150; G1 = 255; B = 60; B1 = 255;
        }
    }

    virtual ~Color() {}
};
#endif //MAI_COLOR_H
