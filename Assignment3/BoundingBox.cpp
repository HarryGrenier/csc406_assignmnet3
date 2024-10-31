//
//  BoundingBox.cpp
//  Week 08 - Earshooter

#include <iostream>
#include <cfloat>
#include "glPlatform.h"
#include "BoundingBox.h"

using namespace earshooter;

bool BoundingBox::drawRelativeBoxes_ = false;
bool BoundingBox::drawAbsoluteBoxes_ = false;

const BoundingBox BoundingBox::NULL_BOX(0, 0, 0, 0);

BoundingBox::BoundingBox(float xmin, float xmax, float ymin, float ymax,
						 ColorIndex color)
	: 	xmin_(xmin),
		xmax_(xmax),
		ymin_(ymin),
		ymax_(ymax),
		color_(color)
{
}

BoundingBox::BoundingBox(const WorldPoint& cornerUL, const WorldPoint& cornerLR,
						ColorIndex color)
	: 	xmin_(cornerUL.x),
		xmax_(cornerLR.x),
		ymin_(cornerUL.y),
		ymax_(cornerLR.y),
		color_(color)
{
}

BoundingBox::BoundingBox(ColorIndex color)
	: 	xmin_(FLT_MAX),
		xmax_(FLT_MAX),
		ymin_(FLT_MAX),
		ymax_(FLT_MAX),
		color_(color)
	{
	}


void BoundingBox::setDimensions(float xmin, float xmax, float ymin, float ymax)
{
	xmin_ = xmin;
	xmax_ = xmax;
	ymin_ = ymin;
	ymax_ = ymax;
}

void BoundingBox::setDimensions(const WorldPoint& cornerUL, const WorldPoint& cornerLR)
{
	xmin_ = cornerUL.x;
	xmax_ = cornerLR.x;
	ymin_ = cornerLR.y;
	ymax_ = cornerUL.y;
}

void BoundingBox::draw(void) const
{
 	glPushMatrix();
	glLineWidth(1.f);
	glColor4fv(COLOR[static_cast<int>(color_)]);
	glBegin(GL_LINE_LOOP);
		glVertex2f(xmin_, ymin_);
		glVertex2f(xmax_, ymin_);
		glVertex2f(xmax_, ymax_);
		glVertex2f(xmin_, ymax_);
	glEnd();
	glPopMatrix();
}

WorldPoint BoundingBox::getCornerUL(void) const
{
	return WorldPoint{xmin_, ymax_};
}
		
WorldPoint BoundingBox::getCornerLR(void) const
{
	return WorldPoint{xmax_, ymin_};
}

bool BoundingBox::boxesAreDrawn()
{
	return drawRelativeBoxes_ || drawAbsoluteBoxes_;
}
bool BoundingBox::relativeBoxesAreDrawn()
{
	return drawRelativeBoxes_;
}
bool BoundingBox::absoluteBoxesAreDrawn()
{
	return drawAbsoluteBoxes_;
}

void BoundingBox::setDrawRelativeBoxes(bool draw)
{
	if (drawAbsoluteBoxes_ && draw)
		drawAbsoluteBoxes_ = false;

	drawRelativeBoxes_ = draw;
}
void BoundingBox::setDrawAbsoluteBoxes(bool draw)
{
	if (drawRelativeBoxes_ && draw)
		drawRelativeBoxes_ = false;
	
	drawAbsoluteBoxes_ = draw;
}
