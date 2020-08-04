#include "MyVector.h"
#include <math.h>
#include <iostream>

double MyVector::DistanceToPoint(MyVector* Point)
{
	MyVector PtoM(StationaryCoords[0]-Point->StationaryCoords[0], StationaryCoords[1] - Point->StationaryCoords[1],GradiantCoords[0],GradiantCoords[1]);
	double Lynda = DotProduct(&PtoM);
	PtoM.GradFactor(Lynda);
	return PtoM.Modulus();
}
double MyVector::DotProduct(MyVector* Point)
{
	double StationarySum = GradiantCoords[0]*(StationaryCoords[0] - Point->StationaryCoords[0])+GradiantCoords[1]*(StationaryCoords[1] - Point->StationaryCoords[1]);
	double GradiantSum = std::pow(GradiantCoords[0],2)+std::pow(GradiantCoords[1],2);
	StationarySum = StationarySum * -1;
	return StationarySum / GradiantSum;
}
void MyVector::GradFactor(double Lynda)
{
	StationaryCoords[0] += GradiantCoords[0] * Lynda;
	StationaryCoords[1] += GradiantCoords[1] * Lynda;
}
double MyVector::Modulus()
{
	return std::sqrt(std::pow(StationaryCoords[0], 2) + std::pow(StationaryCoords[1], 2));
}