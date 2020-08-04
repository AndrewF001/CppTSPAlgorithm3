#pragma once
class MyVector
{
	public:
		MyVector(int X1,int Y1, int GradX, int GradY)
		{
			StationaryCoords[0] = X1;
			StationaryCoords[1] = Y1;
			GradiantCoords[0] = GradX;
			GradiantCoords[0] = GradY;
		}
		double DistanceToPoint(MyVector*);

	private:
		int StationaryCoords[2] = { 0,0 };
		double GradiantCoords[2] = { 0,0 };
		double DotProduct(MyVector*);
		void GradFactor(double);
		double Modulus();


};

