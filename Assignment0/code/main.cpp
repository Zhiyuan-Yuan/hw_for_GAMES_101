#include<cmath>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<iostream>

using namespace std;
#define PI 3.1415926

int main()
{
    Eigen::Vector3d P;
    P << 2.0, 1.0, 0.0;
    Eigen::Matrix3d T;
    T.block(0, 0, 2, 2) << cos(0.25*PI), -sin(0.25*PI), sin(0.25*PI), cos(0.25*PI);
    T.block(0, 2, 3, 1) << 1, 2, 1;
    T.block(2, 0, 1, 2) << 0, 0;

    cout << T * P << endl;
    return 0;
}