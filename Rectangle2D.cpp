//
//  Rectangle2D.cpp
//
//  Created by Jean-Yves Hervé on 2023-09-14.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "glPlatform.h"
#include "Rectangle2D.h"


using namespace std;
using namespace earshooter;

unsigned int Rectangle2D::count_ = 0;
unsigned int Rectangle2D::liveCount_ = 0;

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Constructors
//--------------------------------------
#endif

Rectangle2D::Rectangle2D(float centerX, float centerY, float angle, float width, float height,
						 float r, float g, float b, bool drawContour, float vx, float vy, float spin)
	:	GraphicObject2D(centerX, centerY, angle, r, g, b, drawContour, vx, vy, spin),
		//
		width_(width),
		height_(height),
		index_(count_++)
{
	liveCount_++;
}

Rectangle2D::Rectangle2D(float centerX, float centerY, float angle, float width, float height,
						 ColorIndex fillColor, bool drawContour, float vx, float vy, float spin)
	:	Rectangle2D(centerX, centerY, angle, width, height,
					COLOR[static_cast<int>(fillColor)][0],
					COLOR[static_cast<int>(fillColor)][1],
					COLOR[static_cast<int>(fillColor)][2],
					drawContour,
					vx, vy, spin)
{
}

Rectangle2D::Rectangle2D(const WorldPoint& pt, float angle, float width, float height,
						 float r, float g, float b, bool drawContour, float vx, float vy, float spin)
	:	Rectangle2D(pt.x, pt.y, angle, width, height, r, g, b, drawContour,
					vx, vy, spin)
{
}

Rectangle2D::Rectangle2D(const WorldPoint& pt, float angle, float width, float height,
						 ColorIndex fillColor, bool drawContour, float vx, float vy, float spin)
	:	Rectangle2D(pt.x, pt.y, angle, width, height,
					COLOR[static_cast<int>(fillColor)][0],
					COLOR[static_cast<int>(fillColor)][1],
					COLOR[static_cast<int>(fillColor)][2],
					drawContour,
					vx, vy, spin)
{
}

Rectangle2D::~Rectangle2D()
{
	liveCount_--;
}

#if 0
//-----------------------------------------
#pragma mark -
#pragma mark Drawing, geometry, animation
//-----------------------------------------
#endif

void Rectangle2D::draw_() const
{
	float r = getR(), g = getG(), b = getB();
	
	//	save the current coordinate system (origin, axes)
	glPushMatrix();
	
	glScalef(width_, height_, 1.f);
	
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(+0.5f, -0.5f);
		glVertex2f(+0.5f, +0.5f);
		glVertex2f(-0.5f, +0.5f);
	glEnd();
	
	if (getDrawContour())
	{
		glColor3f(1.f - r, 1.f - g, 1.f - b);
		glBegin(GL_LINE_LOOP);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(+0.5f, -0.5f);
			glVertex2f(+0.5f, +0.5f);
			glVertex2f(-0.5f, +0.5f);
		glEnd();
	}
	
	//	restore the original scale
	glPopMatrix();
}

bool Rectangle2D::isInside(float x, float y) const
{
	float dx = x - getX(), dy = y - getY();
	float radAngle = M_PI*getAngle()/180.f;
	float ct = cosf(radAngle), st = sinf(radAngle);
	float rdx = ct*dx + st*dy;
	float rdy = -st*dx + ct*dy;

	return (fabsf(rdx) <= width_/2) && (fabsf(rdy) <= height_/2);
}


void Rectangle2D::updateAbsoluteBox_()
{

}


#if 0
//--------------------------------------
#pragma mark -
#pragma mark Setters and getters
//--------------------------------------
#endif

unsigned int Rectangle2D::getIndex(void) const
{
	return index_;
}

unsigned int Rectangle2D::getCount(void)
{
	return count_;
}
unsigned int Rectangle2D::getLiveCount(void)
{
	return liveCount_;
}
