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
	updateRelativeBox_();
	updateAbsoluteBox_();
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

void Rectangle2D::updateRelativeBox_() {
	// Half dimensions of the rectangle
	float halfWidth = width_ / 2;
	float halfHeight = height_ / 2;

	// Set the relative bounding box using the local coordinates
	setRelativeBoundingBox(-halfWidth, halfWidth, -halfHeight, halfHeight);
}

void Rectangle2D::updateAbsoluteBox_()
{
	// Center position and angle in radians
	float cx = getX();
	float cy = getY();
	float radAngle = M_PI * getAngle() / 180.f;
	float cosA = cosf(radAngle);
	float sinA = sinf(radAngle);

	// Half-dimensions
	float halfWidth = width_ / 2;
	float halfHeight = height_ / 2;

	// Calculate rotated corner positions
	float x1 = cx + (-halfWidth * cosA - -halfHeight * sinA);
	float y1 = cy + (-halfWidth * sinA + -halfHeight * cosA);

	float x2 = cx + (halfWidth * cosA - -halfHeight * sinA);
	float y2 = cy + (halfWidth * sinA + -halfHeight * cosA);

	float x3 = cx + (halfWidth * cosA - halfHeight * sinA);
	float y3 = cy + (halfWidth * sinA + halfHeight * cosA);

	float x4 = cx + (-halfWidth * cosA - halfHeight * sinA);
	float y4 = cy + (-halfWidth * sinA + halfHeight * cosA);

	// Find min and max x and y values for bounding box
	float minX = x1, maxX = x1;
	float minY = y1, maxY = y1;

	// Compare with each corner
	if (x2 < minX) minX = x2; if (x2 > maxX) maxX = x2;
	if (y2 < minY) minY = y2; if (y2 > maxY) maxY = y2;

	if (x3 < minX) minX = x3; if (x3 > maxX) maxX = x3;
	if (y3 < minY) minY = y3; if (y3 > maxY) maxY = y3;

	if (x4 < minX) minX = x4; if (x4 > maxX) maxX = x4;
	if (y4 < minY) minY = y4; if (y4 > maxY) maxY = y4;

	// Set the absolute bounding box with the calculated min and max
	setAbsoluteBoundingBox(minX, maxX, minY, maxY);
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
