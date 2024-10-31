//
//  Triangle.cpp
//  Week 05 - Prog 1
//
//  Created by Jean-Yves Hervé on 2024-09-10.
//

#include <iostream>
#include <cmath>

#include "glPlatform.h"
#include "World2D.h"
#include "Triangle.h"

using namespace std;
using namespace earshooter;

//	Workaround for that pesky M_PI lack on Windows
#ifndef M_PI
#define	M_PI 3.14159265f
#endif

unsigned int Triangle::count_ = 0;
unsigned int Triangle::liveCount_ = 0;

float Triangle::xy_[3][2] = {{1.f, 0.f},
							 {cosf(2*M_PI/3), sinf(2*M_PI/3)},
							 {cosf(2*M_PI/3), -sinf(2*M_PI/3)}};


#if 0
//--------------------------------------
#pragma mark -
#pragma mark Constructors
//--------------------------------------
#endif

Triangle::Triangle(float cx, float cy, float angle, float radius, float r,
			float g, float b, bool drawContour, float vx, float vy, float spin)
	:	GraphicObject2D(cx, cy, angle, r, g, b, drawContour, vx, vy, spin),
		//
		radius_(radius),
		index_(count_++)
{
	liveCount_++;
}

Triangle::Triangle(float cx, float cy, float angle, float radius,
				   ColorIndex fillColor, bool drawContour,
				   float vx, float vy, float spin)
	:	Triangle(cx, cy, angle, radius,
				 COLOR[static_cast<int>(fillColor)][0],
				 COLOR[static_cast<int>(fillColor)][1],
				 COLOR[static_cast<int>(fillColor)][2],
				 drawContour, vx, vy, spin)
{
}

Triangle::Triangle(const WorldPoint& pt, float angle, float radius,
				   float r, float g, float b, bool drawContour,
				   float vx, float vy, float spin)
	:	Triangle(pt.x, pt.y, angle, r, g, b, drawContour, vx, vy, spin)
{
}

Triangle::Triangle(const WorldPoint& pt, float angle, float radius,
				   ColorIndex fillColor, bool drawContour, float vx, float vy, float spin)
	:	Triangle(pt.x, pt.y, angle, radius,
				 COLOR[static_cast<int>(fillColor)][0],
				 COLOR[static_cast<int>(fillColor)][1],
				 COLOR[static_cast<int>(fillColor)][2],
				 drawContour, vx, vy, spin)
{
}


Triangle::~Triangle()
{
	liveCount_--;
}

#if 0
//-----------------------------------------
#pragma mark -
#pragma mark Drawing, geometry, animation
//-----------------------------------------
#endif


void Triangle::draw_() const
{
	float r = getR(), g = getG(), b = getB();
	
	//	save the initial reference frame
	glPushMatrix();
	
	glScalef(radius_, radius_, 1.f);
	
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		//	 vertex coordinates are relative to the center of the triangle
		glVertex2fv(xy_[0]);
		glVertex2fv(xy_[1]);
		glVertex2fv(xy_[2]);
	glEnd();

	if (getDrawContour())
	{
		// simply invert the filling color
		glColor3f(1.f-r, 1.f-g, 1.f-b);
		glBegin(GL_LINE_LOOP);
			//	 vertex coordinates are relative to the center of the triangle
			glVertex2fv(xy_[0]);
			glVertex2fv(xy_[1]);
			glVertex2fv(xy_[2]);
		glEnd();
	}
	
	//	restore the original scale
	glPopMatrix();
}

UpdateStatus Triangle::update(float dt)
{
	//	call function of the parent class
	UpdateStatus status = GraphicObject2D::update(dt);
	
//	// change color:  make more green as time passes
//	g_ += 0.3f*dt;
//	if (g_ > 1.f)
//		g_ = 1.f;

	return status;
}

bool Triangle::isInside(float x, float y) const
{
	return false;
}

void Triangle::updateAbsoluteBox_()
{

}

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Setters and getters
//--------------------------------------
#endif

unsigned int Triangle::getIndex(void) const
{
	return index_;
}

unsigned int Triangle::getLiveCount(void)
{
	return liveCount_;
}

unsigned int Triangle::getCount(void)
{
	return count_;
}

