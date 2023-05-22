
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
    Mat final_frame;
    VideoCapture cap;

    pair <double, double> center_a_bot;

    double angle_bot;
    double angle_target;
    double distance_target;

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
            cap.read(frame);
            cap.read(final_frame);
            final_frame = detect_spot_on_image(front) + detect_spot_on_image(back) + detect_spot_on_image(target);
            if (haveRobot()) {
                line(frame, Point(center_a_contrast_spot(front).first, center_a_contrast_spot(front).second),
                     Point(center_a_contrast_spot(back).first, center_a_contrast_spot(back).second), Scalar(0, 0, 255));
            }
            imshow("General picture", final_frame);
            imshow("Camera", frame);

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

    pair <double, double> center_a_contrast_spot(Spot part) {
            Moments oMoments = moments(detect_spot_on_image(part));
            double dM01 = oMoments.m01;
            double dM10 = oMoments.m10;
            double dArea = oMoments.m00;

            if (dArea > 10000)
                part.setCenter(make_pair(dM10 / dArea, dM01 / dArea));

            return part.getCenter();
        };

    pair <double, double> center_a_robot(pair <int, int> front, pair <int, int> back) {
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

    void define_angle_bot() {
        pair <double, double> p_a = center_a_contrast_spot(front);
        pair <double, double> p_b = center_a_contrast_spot(back);

        this->center_a_bot = center_a_robot(p_a, p_b);
        this->angle_bot = angle_need_to_point(p_b, p_a);

    }

    void define_angle_to_target() {
        pair <double, double> point_target = center_a_contrast_spot(target);
        int centre_to_target = angle_need_to_point(this->center_a_bot, point_target);
        this->angle_target = centre_to_target - this->angle_bot;

    }

    void define_distance_to_target() {
        pair <double, double> point_target = center_a_contrast_spot(target);
        this->distance_target = distance_beetwen_points(this->center_a_bot, point_target);
    }

    const pair<double, double> &getCenterABot() const {
        return center_a_bot;
    }

    double getAngleBot() const {
        return angle_bot;
    }

    double getAngleTarget() const {
        return angle_target;
    }

    double getDistanceTarget() const {
        return distance_target;
    }

    void define_all() {
        cap >> frame;
        final_frame = frame;

        detectRobot();

        this->define_angle_bot();
        this->define_angle_to_target();
        this->define_distance_to_target();

        cout << this->getDistanceTarget() << '\n';

    }

    bool has_angle_to_target(int delta) {
        if (abs(this->angle_target) > delta)
            return true;
        else
            return false;
    }

    bool has_distance_to_target(int distance) const {
        if (this->distance_target < distance)
            return false;
        else
            return true;

    }
};
#endif //MAI_ROBOT_H
