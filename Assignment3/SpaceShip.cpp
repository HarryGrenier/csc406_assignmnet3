//
//  SpaceShip.cpp
//  Week 05 - Prog 1
//
//  Created by Jean-Yves Hervé on 2024-09-10.
//

#include <iostream>
#include <cmath>

#include "glPlatform.h"
#include "World2D.h"
#include "SpaceShip.h"

using namespace std;
using namespace earshooter;

//	Workaround for that pesky M_PI lack on Windows
#ifndef M_PI
#define	M_PI 3.14159265f
#endif

unsigned int SpaceShip::count_ = 0;
unsigned int SpaceShip::liveCount_ = 0;

float SpaceShip::xy_[3][2] = { {1.f, 0.f},
							 {cosf(2 * M_PI / 3), sinf(2 * M_PI / 3)},
							 {cosf(2 * M_PI / 3), -sinf(2 * M_PI / 3)} };


#if 0
//--------------------------------------
#pragma mark -
#pragma mark Constructors
//--------------------------------------
#endif

SpaceShip::SpaceShip(float cx, float cy, float angle, float radius, float r,
	float g, float b, bool drawContour, float vx, float vy, float spin)
	: GraphicObject2D(cx, cy, angle, r, g, b, drawContour, vx, vy, spin),
	//
	angularVelocity_(0.0f),
	radius_(radius),
	index_(count_++)
{
	liveCount_++;
	updateRelativeBox_();
	updateAbsoluteBox_();
}

SpaceShip::SpaceShip(float cx, float cy, float angle, float radius,
	ColorIndex fillColor, bool drawContour,
	float vx, float vy, float spin)
	: SpaceShip(cx, cy, angle, radius,
		COLOR[static_cast<int>(fillColor)][0],
		COLOR[static_cast<int>(fillColor)][1],
		COLOR[static_cast<int>(fillColor)][2],
		drawContour, vx, vy, spin)
{
}

SpaceShip::SpaceShip(const WorldPoint& pt, float angle, float radius,
	float r, float g, float b, bool drawContour,
	float vx, float vy, float spin)
	: SpaceShip(pt.x, pt.y, angle, r, g, b, drawContour, vx, vy, spin)
{
}

SpaceShip::SpaceShip(const WorldPoint& pt, float angle, float radius,
	ColorIndex fillColor, bool drawContour, float vx, float vy, float spin)
	: SpaceShip(pt.x, pt.y, angle, radius,
		COLOR[static_cast<int>(fillColor)][0],
		COLOR[static_cast<int>(fillColor)][1],
		COLOR[static_cast<int>(fillColor)][2],
		drawContour, vx, vy, spin)
{
}


SpaceShip::~SpaceShip()
{
	liveCount_--;
}

#if 0
//-----------------------------------------
#pragma mark -
#pragma mark Drawing, geometry, animation
//-----------------------------------------
#endif


void SpaceShip::draw_() const
{
	float r = getR(), g = getG(), b = getB();

	// Save the initial reference frame
	glPushMatrix();

	// Scale based on radius
	glScalef(radius_, radius_, 1.f);

	// Set the spaceship color
	glColor3f(r, g, b);

	// main body of the spaceship
	glBegin(GL_POLYGON);
	glVertex2f(0.0f, 1.0f);             // Nose
	glVertex2f(-0.6f, -0.8f);           // Left rear
	glVertex2f(0.0f, -0.5f);            // Center rear
	glVertex2f(0.6f, -0.8f);            // Right rear
	glEnd();

	// Draw wings
	glColor3f(r * 0.7f, g * 0.7f, b * 0.7f); // Slightly darker color for contrast
	glBegin(GL_TRIANGLES);
	// Left wing
	glVertex2f(-0.4f, -0.5f);
	glVertex2f(-1.0f, -1.2f);
	glVertex2f(-0.4f, -1.0f);

	// Right wing
	glVertex2f(0.4f, -0.5f);
	glVertex2f(1.0f, -1.2f);
	glVertex2f(0.4f, -1.0f);
	glEnd();

	// Draw engine
	glColor3f(0.2f, 0.2f, 0.2f); // Dark color for the engine
	glBegin(GL_POLYGON);
	glVertex2f(-0.2f, -0.8f);
	glVertex2f(0.2f, -0.8f);
	glVertex2f(0.2f, -1.0f);
	glVertex2f(-0.2f, -1.0f);
	glEnd();

	// Draw cockpit window
	glColor3f(1.0f, 1.0f, 1.0f); // White color for the cockpit window
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.6f); // Center of the cockpit
	for (int i = 0; i <= 20; ++i) {
		float angle = 2 * M_PI * i / 20;
		glVertex2f(0.1f * cos(angle), 0.6f + 0.1f * sin(angle));
	}
	glEnd();

	// If drawContour is enabled
	if (getDrawContour()) {
		glColor3f(1.f - r, 1.f - g, 1.f - b); // Invert color for the contour
		glBegin(GL_LINE_LOOP);
		glVertex2f(0.0f, 1.0f);
		glVertex2f(-0.6f, -0.8f);
		glVertex2f(0.0f, -0.5f);
		glVertex2f(0.6f, -0.8f);
		glEnd();
	}

	// Restore the original scale
	glPopMatrix();
}

UpdateStatus SpaceShip::update(float dt)
{

	setAngle(getAngle() + angularVelocity_ * dt);
	//	call function of the parent class
	UpdateStatus status = GraphicObject2D::update(dt);

	//	// change color:  make more green as time passes
	//	g_ += 0.3f*dt;
	//	if (g_ > 1.f)
	//		g_ = 1.f;

	return status;
}

bool SpaceShip::isInside(float x, float y) const
{
	return false;
}

void SpaceShip::updateRelativeBox_() {
	// Calculate relative bounding box based on SpaceShip vertices in local coordinates
	float minX = xy_[0][0] * radius_;
	float maxX = xy_[0][0] * radius_;
	float minY = xy_[0][1] * radius_;
	float maxY = xy_[0][1] * radius_;

	for (int i = 1; i < 3; ++i) {
		float x = xy_[i][0] * radius_;
		float y = xy_[i][1] * radius_;

		if (x < minX) minX = x;
		if (x > maxX) maxX = x;
		if (y < minY) minY = y;
		if (y > maxY) maxY = y;
	}

	// Set the relative bounding box
	setRelativeBoundingBox(minX, maxX, minY, maxY);
}

void SpaceShip::updateAbsoluteBox_() {
	// Retrieve the SpaceShip's center position using the getX() and getY() methods
	float cx = getX();
	float cy = getY();

	// Angle of the SpaceShip in radians
	float angle = M_PI * getAngle() / 180.f;
	float cA = cosf(angle);
	float sA = sinf(angle);

	// Define the SpaceShip vertices based on radius and angle
	// Assuming radius defines the distance from the center to each vertex
	float x1 = cx + (xy_[0][0] * cA - xy_[0][1] * sA) * radius_;
	float y1 = cy + (xy_[0][0] * sA + xy_[0][1] * cA) * radius_;

	float x2 = cx + (xy_[1][0] * cA - xy_[1][1] * sA) * radius_;
	float y2 = cy + (xy_[1][0] * sA + xy_[1][1] * cA) * radius_;

	float x3 = cx + (xy_[2][0] * cA - xy_[2][1] * sA) * radius_;
	float y3 = cy + (xy_[2][0] * sA + xy_[2][1] * cA) * radius_;

	// Calculate min and max for the bounding box
	float minX = x1;
	if (x2 < minX) minX = x2;
	if (x3 < minX) minX = x3;

	float maxX = x1;
	if (x2 > maxX) maxX = x2;
	if (x3 > maxX) maxX = x3;

	float minY = y1;
	if (y2 < minY) minY = y2;
	if (y3 < minY) minY = y3;

	float maxY = y1;
	if (y2 > maxY) maxY = y2;
	if (y3 > maxY) maxY = y3;

	// Set the absolute bounding box using the calculated min and max values
	setAbsoluteBoundingBox(minX, maxX, minY, maxY);
}


#if 0
//--------------------------------------
#pragma mark -
#pragma mark Setters and getters
//--------------------------------------
#endif

unsigned int SpaceShip::getIndex(void) const
{
	return index_;
}

unsigned int SpaceShip::getLiveCount(void)
{
	return liveCount_;
}

unsigned int SpaceShip::getCount(void)
{
	return count_;
}

