#include "QuadTree.h"
#include "CppTSPalgorithm.h"

bool QuadTree::Insert(Point* Dot)
{
	if (ContainPoint(Dot))
	{
		if (AssignedPoint == nullptr)
		{
			AssignedPoint = Dot;
			return true;
		}
		else if (!Divided)
		{
			int WCarry = 0;
			int HCarry = 0;
			if (Width % 2 == 1)
				WCarry = 1;
			if (Height % 2 == 1)
				HCarry = 1;
			SubTree.reserve(4);
			SubTree.push_back(QuadTree(X, Y, Width / 2 +WCarry, Height / 2 +HCarry)); //NE
			SubTree.push_back(QuadTree(X + Width / 2+WCarry, Y, Width / 2, Height / 2 + HCarry)); //NW
			SubTree.push_back(QuadTree(X + Width / 2+WCarry, Y + Height / 2+HCarry, Width / 2, Height / 2)); //SW
			SubTree.push_back(QuadTree(X, Y + Height / 2+HCarry, Width / 2+WCarry, Height / 2)); //SE
			Divided = true;	
		}
		//Make this into a loop somehow just to look cleaner but this should work.
		if (SubTree[0].Insert(Dot))
			return true;
		else if (SubTree[1].Insert(Dot))
			return true;
		else if (SubTree[2].Insert(Dot))
			return true;
		else if (SubTree[3].Insert(Dot))
			return true;
		else
		{
			return false;
		}
	}
	return false;
}
bool QuadTree::ContainPoint(Point* Dot)
{
	if (Dot->X > X && Dot->X<=X + Width && Dot->Y>Y && Dot->Y <= Y + Height)
		return true;
	return false;
}

void QuadTree::DrawRect(QPainter* painter)
{	
	painter->drawRect(X+15, Y+85, Width, Height);
	if (Divided)
	{
		for (size_t i = 0; i < 4; i++)
		{
			SubTree[i].DrawRect(painter);
		}
	}
}

//not the cleanest code
void QuadTree::ContainArea(std::vector<Point*>* List, short int Rx, short int Ry, unsigned short int Rwidth, unsigned short int Rheight)
{
	//is the quadtree intersecting the area
	if (Rx>X-Rwidth&&Rx<X+Width && Ry>Y-Rheight && Ry<Y+Height)
	{
		//is the point in the area
		if (AssignedPoint != nullptr)
		{
			if (AssignedPoint->X > Rx && AssignedPoint->X<Rx + Rwidth && AssignedPoint->Y>Ry && AssignedPoint->Y < Ry + Rheight)
			{
				List->push_back(AssignedPoint);
			}

			if (Divided)
			{
				for (size_t i = 0; i < 4; i++)
				{
					SubTree[i].ContainArea(List, Rx, Ry, Rwidth, Rheight);
				}
			}
		}
	}
}
