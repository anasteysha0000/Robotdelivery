#ifndef MAI_SPOT_H
#define MAI_SPOT_H

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Color.h"
using namespace cv;
using namespace std;


class Spot{
private:
    Color color;
    pair<int, int> center;

public:

    Spot(const string &color) : color(Color(color)), center(-1, -1) {}

    void setColor(const Color &color) {
        Spot::color = color;
    }

    const Color &getColor() {
        return color;
    }

    void setCenter(const pair<int, int> &centre) {
        Spot::center = centre;
    }

    pair <int, int> getCenter() const {
        return center;
    }
};
#endif //MAI_SPOT_H
