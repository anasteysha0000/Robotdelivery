#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Spot.h"
#include "Camera.h"


using namespace cv;
using namespace std;


int main()
{
    Spot front("red");
    Spot back("blue");
    Spot target("yellow");
    Camera laptop(front, back, target);
    laptop.detectRobot();

    return 0;
}