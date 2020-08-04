#pragma once
#include <QtWidgets/QMainWindow>
class Point
{

public:
	Point(int x, int y)
	{
		X = x;
		Y = y;
	};
	unsigned short int X;
	unsigned short int Y;
	bool Connected = false;
	//int Position = 0;
	Point* PreviousePoint = nullptr;
	Point* NextPoint = nullptr;

	void DrawConenctions(QPainter*, Point*);

};

