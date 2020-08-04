#pragma once
#include "Point.h"
#include "qpainter.h"
#include <vector>
class QuadTree
{
	public:
		QuadTree(short int x, short int y, unsigned short int width, unsigned short int height)
		{
			X = x;
			Y = y;
			Width = width;
			Height = height;
		};
		bool Insert(Point*);
		void DrawRect(QPainter*);
		std::vector<QuadTree> SubTree; //0=NE, 1=NW, 2=SW, 3=SE
		void ContainArea(std::vector<Point*>*, short int, short int, unsigned short int, unsigned short int);

	private:
		Point* AssignedPoint = nullptr;
		short int X;
		short int Y;
		unsigned short int Width;
		unsigned short int Height;
		bool Divided = false;

		bool ContainPoint(Point*);
		



};

