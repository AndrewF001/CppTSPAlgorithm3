#include "Point.h"
#include "qpainter.h"
void Point::DrawConenctions(QPainter* painter, Point* StartNode)
{
	painter->drawLine(X + 15, Y + 85, NextPoint->X + 15, NextPoint->Y + 85);
	if (NextPoint == StartNode)
		return;
	NextPoint->DrawConenctions(painter, StartNode);
}