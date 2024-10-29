//
//  Ellipse2D.cpp
//
//  Created by Jean-Yves Hervé on 2023-09-14.  Revised 2024-10-03
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "glPlatform.h"
#include "Ellipse2D.h"

using namespace std;
using namespace earshooter;

//	Workaround for that pesky M_PI lack on Windows
#ifndef M_PI
#define	M_PI 3.14159265f
#endif

//	Prototypes for "file-level private" functions
const int Ellipse2D::numCirclePts_ = 24;
float** Ellipse2D::circlePts_;
unsigned int Ellipse2D::count_ = 0;
unsigned int Ellipse2D::liveCount_ = 0;

//	Ensures that the vertices defining ellipses' contours are initialized
//	before action starts
bool ellipseInitialized = initEllipseFunc();

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Constructors
//--------------------------------------
#endif

Ellipse2D::Ellipse2D(float centerX, float centerY, float angle, float radiusX, float radiusY,
				float r, float g, float b, bool drawContour, float vx, float vy, float spin)
	:	GraphicObject2D(centerX, centerY, angle, r, g, b, drawContour, vx, vy, spin),
		//
		radiusX_(radiusX),
		radiusY_(radiusY),
		index_(count_++)
{
	liveCount_++;
	
	//	set the bounding boxes
	updateAbsoluteBox_();
	setRelativeBoundingBox(-radiusX, +radiusX, -radiusY, +radiusY);
}

Ellipse2D::Ellipse2D(const WorldPoint& pt, float angle, float radiusX, float radiusY,
				float r, float g, float b, bool drawContour, float vx, float vy, float spin)
	:	Ellipse2D(pt.x, pt.y, angle, radiusX, radiusY, r, g, b, drawContour, vx, vy, spin)
{
}


Ellipse2D::Ellipse2D(float centerX, float centerY, float angle, float radiusX, float radiusY,
					 ColorIndex fillColor, bool drawContour, float vx, float vy, float spin)
	:	Ellipse2D(centerX, centerY, angle, radiusX, radiusY,
					COLOR[static_cast<int>(fillColor)][0],
					COLOR[static_cast<int>(fillColor)][1],
					COLOR[static_cast<int>(fillColor)][2],
					drawContour,
					vx, vy, spin)
{
}

Ellipse2D::Ellipse2D(const WorldPoint& pt, float angle, float radiusX, float radiusY,
				ColorIndex fillColor, bool drawContour, float vx, float vy, float spin)
	:	Ellipse2D(pt.x, pt.y, angle, radiusX, radiusY,
					COLOR[static_cast<int>(fillColor)][0],
					COLOR[static_cast<int>(fillColor)][1],
					COLOR[static_cast<int>(fillColor)][2],
					drawContour,
					vx, vy, spin)
{
}


Ellipse2D::~Ellipse2D()
{
	liveCount_--;
}

#if 0
//-----------------------------------------
#pragma mark -
#pragma mark Drawing, geometry, animation
//-----------------------------------------
#endif

void Ellipse2D::draw_() const
{
	float r = getR(), g = getG(), b = getB();
	
	//	save the current coordinate system (origin, axes)
	glPushMatrix();
	
	//	apply the radius as a scale
	glScalef(radiusX_, radiusY_, 1.f);
	
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		for (int k=0; k<numCirclePts_; k++)
			glVertex2f(circlePts_[k][0],
						circlePts_[k][1]);
	glEnd();
	
	if (getDrawContour())
	{
		glColor3f(1.f - r, 1.f - g, 1.f - b);
		glBegin(GL_LINE_LOOP);
		for (int k=0; k<numCirclePts_; k++)
			glVertex2f(circlePts_[k][0],
						circlePts_[k][1]);
		glEnd();
	}

	//	restore the original scale
	glPopMatrix();
}


bool Ellipse2D::isInside(float x, float y) const
{
	float dx = x - getX(), dy = y-getY();
	float radAngle = M_PI*getAngle()/180.f;
	float ct = cosf(radAngle), st = sinf(radAngle);
	//	scaled rotated coordinates
	float srdx = (ct*dx + st*dy)/radiusX_,
		  srdy = (-st*dx + ct*dy)/radiusY_;
	return srdx*srdx + srdy*srdy <= 1.f;
}


void Ellipse2D::updateAbsoluteBox_()
{
	//	could definitely be optimized
	float angle = M_PI*getAngle()/180.f;
	float cx = getX(), cy = getY();
	float cA = cosf(angle), sA = sinf(angle);
	//	parametric equation of the elipse in global reference frame
	//		x(t) = cx + radiusX*cos(angle)*cos(t) - radiusY*sin(angle)*sin(t)
	//		y(t) = cy + radiusX*cos(angle)*cos(t) - radiusY*sin(angle)*sin(t)
	//	The x extrema are reached for +/-
	float tx = atan2f(radiusY_*sA, -radiusX_*cA);
	float ctx = cosf(tx), stx = sinf(tx);
	//	The y extrema are reached for +/-
	float ty = atan2f(-radiusY_*cA, -radiusX_*sA);
	float cty = cosf(ty), sty = sinf(ty);
	//	Now we can compute the extremal displacements from the center
	float dx = fabsf(radiusX_*cA*ctx - radiusY_*sA*stx),
		  dy = fabsf(radiusX_*sA*cty + radiusY_*cA*sty);
	//	And compute the min and max
	setAbsoluteBoundingBox(cx - dx, cx + dx, cy - dy, cy + dy);
}



#if 0
//--------------------------------------
#pragma mark -
#pragma mark Setters and getters
//--------------------------------------
#endif

unsigned int Ellipse2D::getIndex(void) const
{
	return index_;
}

unsigned int Ellipse2D::getCount(void)
{
	return count_;
}
unsigned int Ellipse2D::getLiveCount(void)
{
	return liveCount_;
}

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Free functions
//--------------------------------------
#endif

// I want this code to run only once
bool earshooter::initEllipseFunc()
{
	Ellipse2D::circlePts_ = new float*[Ellipse2D::numCirclePts_];
	for (int k=0; k<Ellipse2D::numCirclePts_; k++)
		Ellipse2D::circlePts_[k] = new float[2];
		
	//	Initialize the array of coordinates of the disk or radius 1 centered at (0, 0)
	float angleStep = 2.f*M_PI/Ellipse2D::numCirclePts_;
	for (int k=0; k<Ellipse2D::numCirclePts_; k++)
	{
		float theta = k*angleStep;
		Ellipse2D::circlePts_[k][0] = cosf(theta);
		Ellipse2D::circlePts_[k][1] = sinf(theta);
	}
	return true;
}

void earshooter::drawDisk()
{
	glBegin(GL_POLYGON);
			for (int k=0; k<Ellipse2D::numCirclePts_; k++)
				glVertex2fv(Ellipse2D::circlePts_[k]);
	glEnd();
}

void earshooter::drawArc(float startFrac, float endFrac)
{
	if ((startFrac < endFrac) && (startFrac > -100) && (endFrac < +100))
	{
		int startIndex = static_cast<int>(roundf(startFrac*(Ellipse2D::numCirclePts_-1)));
		int endIndex = static_cast<int>(roundf(endFrac*(Ellipse2D::numCirclePts_-1)));
		
		glBegin(GL_LINE_STRIP);
			for (int k=startIndex; k<=endIndex; k++)
			{
				int index = k < 0 ? k + Ellipse2D::numCirclePts_ : k;
				glVertex2fv(Ellipse2D::circlePts_[index]);
			}
		glEnd();
	}
}
