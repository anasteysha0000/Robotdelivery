
#ifndef MAI_ROBOT_H
#define MAI_ROBOT_H
#define _USE_MATH_DEFINES

#include <iostream>
#include <utility>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Color.h"
#include "Spot.h"
#include <math.h> // число pi

using namespace cv;
using namespace std;

class Camera {
private:

    Spot front;
    Spot back;
    Spot target; //the final object of the robot
    Mat frame;
    VideoCapture cap;
    
public:

    Camera(Spot front, Spot back, Spot target) : front(std::move(front)), back(std::move(back)), target(std::move(target)) {
        cap.open(0);
        if (!cap.isOpened())
            cout << "Error opening video stream or file" << endl;
        cap.read(frame);
    }

    virtual ~Camera() {
        cap.release();
        destroyAllWindows();
    }

    bool haveRobot() {
        Moments redMoments = moments(detect_spot_on_image(front));
        Moments blueMoments = moments(detect_spot_on_image(back));
        double redArea = redMoments.m00; //m00 is the number of all points that make up the spot
        double blueArea = blueMoments.m00;
        if (redArea > 10000 and blueArea > 10000) {
            return true;
        } else {
            return false;
        }
    }

    void detectRobot() {
        Mat resultWhiteBlack; // general picture
        while (true) {
            cap.read(frame);
            cap.read(resultWhiteBlack);
            Mat resultWhiteBlack = detect_spot_on_image(front) + detect_spot_on_image(back);
            if (haveRobot()) {
                line(frame, Point(center_a_contrast_spot(front).first, center_a_contrast_spot(front).second),
                     Point(center_a_contrast_spot(back).first, center_a_contrast_spot(back).second), Scalar(0, 0, 255));
            }
            imshow("General picture", resultWhiteBlack);
            imshow("Camera", frame);

            if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
            {
                cout << "Esc key is pressed by user" << endl;
                break;
            }
        }
    }

    Mat detect_spot_on_image(Spot part) {
            Mat imgHSV;
            Mat imgWhite;
            Mat imgOriginal;
            cap.read(imgOriginal);
            cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frameOriginal from BGR to HSV
            inRange(imgHSV, Scalar(part.getColor().R, part.getColor().G, part.getColor().B), Scalar(part.getColor().R1, part.getColor().G1, part.getColor().B1), imgWhite); //Threshold the image
            erode(imgWhite, imgWhite, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            dilate( imgWhite, imgWhite, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            dilate( imgWhite, imgWhite, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            erode(imgWhite, imgWhite, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            return imgWhite;
    }

    pair <int, int> center_a_contrast_spot(Spot part) {
            Moments oMoments = moments(detect_spot_on_image(part));
            double dM01 = oMoments.m01;
            double dM10 = oMoments.m10;
            double dArea = oMoments.m00;

            if (dArea > 10000)
                part.setCenter(make_pair(dM10 / dArea, dM01 / dArea));

            return part.getCenter();
        };

    pair <int, int> center_a_robot(pair <int, int> front, pair <int, int> back) {
        int x = (front.first + back.first) / 2;
        int y = (front.second + back.second) / 2;

        return make_pair(x, y);
    }

    double angle_need_to_point(pair <int, int> center_robot, pair <int, int> center_spot_target) {
        double hyp = pow((center_spot_target.first - center_robot.first) * (center_spot_target.first - center_robot.first) + (center_spot_target.second - center_robot.second) * (center_spot_target.second - center_robot.second), 2);
        asin(hyp / (center_spot_target.first - center_robot.first));

        double a = atan2((center_spot_target.first - center_robot.first), (center_spot_target.second - center_robot.second)) / M_PI * 180;

        if (a >= 0)
            return a;
        else
            return 360 + a;
    }

    double distance_beetwen_points(pair <int, int> center_robot, pair <int, int> center_spot_target){
        return sqrt((center_spot_target.first - center_robot.first)*(center_spot_target.first - center_robot.first) + (center_spot_target.second - center_robot.second)*(center_spot_target.second - center_robot.second));
    }
};
#endif //MAI_ROBOT_H
