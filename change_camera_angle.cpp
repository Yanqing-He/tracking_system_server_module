#include<iostream>
#include<math.h>
#include<stdio.h>

#define PI 3.14159265358979323846
using namespace std;

int change_camera_angle(double a, double b, double c, double x, double y, double z, double *alpha_s, double *beta_s){

	if ((b - y) < 0){
		*alpha_s = atan2(b - y, a - x) * 180 / PI + 360;
	}
	else {
		*alpha_s = atan2(b - y, a - x) * 180 / PI;
	}

	*beta_s = atan2(c - z, sqrt((a - x) * (a - x) + (b - y)* (b - y))) * 180 / PI;

	return 0;
}

int main() //Object(a,b,c) Camera(x,y,z)
{
	double a, b, c, x, y, z;
	cout << "Please input parameter in Object(a,b,c) Camera(x,y,z)" << endl;
	cin >> a >> b >> c >> x >> y >> z;
	double alpha, beta;
	change_camera_angle(a, b, c, x, y, z, &alpha, &beta);
	cout << "Ahlpa=" << alpha << ", Beta=" << beta << "." << endl;
	return 0;


}