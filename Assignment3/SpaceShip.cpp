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
#include "Projectile.h"

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
	angularVelocity_(0.0f),
	radius_(radius),
	index_(count_++),
	fireRate_(5.0f),  // Set fire rate to 5 projectiles per second
	lastFireTime_(std::chrono::high_resolution_clock::now()) // Initialize lastFireTime
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

	// Main body of the spaceship
	glBegin(GL_POLYGON);
	glVertex2f(1.0f, 0.0f);             // Nose (now pointing right)
	glVertex2f(-0.8f, 0.6f);            // Top rear
	glVertex2f(-0.5f, 0.0f);            // Center rear
	glVertex2f(-0.8f, -0.6f);           // Bottom rear
	glEnd();

	// Draw wings
	glColor3f(r * 0.7f, g * 0.7f, b * 0.7f); // Slightly darker color for contrast
	glBegin(GL_TRIANGLES);
	// Top wing
	glVertex2f(-0.5f, 0.4f);
	glVertex2f(-1.2f, 1.0f);
	glVertex2f(-1.0f, 0.4f);

	// Bottom wing
	glVertex2f(-0.5f, -0.4f);
	glVertex2f(-1.2f, -1.0f);
	glVertex2f(-1.0f, -0.4f);
	glEnd();

	// Draw engine
	glColor3f(0.2f, 0.2f, 0.2f); // Dark color for the engine
	glBegin(GL_POLYGON);
	glVertex2f(-0.8f, -0.2f);
	glVertex2f(-1.0f, -0.2f);
	glVertex2f(-1.0f, 0.2f);
	glVertex2f(-0.8f, 0.2f);
	glEnd();

	// Draw cockpit window
	glColor3f(1.0f, 1.0f, 1.0f); // White color for the cockpit window
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.6f, 0.0f); // Center of the cockpit (aligned near nose)
	for (int i = 0; i <= 20; ++i) {
		float angle = 2 * M_PI * i / 20;
		glVertex2f(0.6f + 0.1f * cos(angle), 0.1f * sin(angle));
	}
	glEnd();

	// If drawContour is enabled
	if (getDrawContour()) {
		glColor3f(1.f - r, 1.f - g, 1.f - b); // Invert color for the contour
		glBegin(GL_LINE_LOOP);
		glVertex2f(1.0f, 0.0f);             // Nose
		glVertex2f(-0.8f, 0.6f);            // Top rear
		glVertex2f(-0.5f, 0.0f);            // Center rear
		glVertex2f(-0.8f, -0.6f);           // Bottom rear
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
	if (partRelativeBox_.empty()) {
		// Initialize bounding boxes for each part of the spaceship
		partRelativeBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::YELLOW)); // Body
		partRelativeBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::YELLOW)); // Top Wing
		partRelativeBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::YELLOW)); // Bottom Wing
		partRelativeBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::YELLOW)); // Engine
	}

	float scale = radius_;

	// Body bounding box (adjusted to new orientation)
	float bodyMinX = -0.8f * scale;
	float bodyMaxX = 1.0f * scale;
	float bodyMinY = -0.6f * scale;
	float bodyMaxY = 0.6f * scale;
	partRelativeBox_[BODY]->setDimensions(bodyMinX, bodyMaxX, bodyMinY, bodyMaxY);

	// Top Wing bounding box
	float topWingMinX = -1.2f * scale;
	float topWingMaxX = -0.4f * scale;
	float topWingMinY = 0.4f * scale;
	float topWingMaxY = 1.0f * scale;
	partRelativeBox_[TOP_WING]->setDimensions(topWingMinX, topWingMaxX, topWingMinY, topWingMaxY);

	// Bottom Wing bounding box
	float bottomWingMinX = -1.2f * scale;
	float bottomWingMaxX = -0.4f * scale;
	float bottomWingMinY = -1.0f * scale;
	float bottomWingMaxY = -0.4f * scale;
	partRelativeBox_[BOTTOM_WING]->setDimensions(bottomWingMinX, bottomWingMaxX, bottomWingMinY, bottomWingMaxY);

	// Engine bounding box
	float engineMinX = -1.0f * scale;
	float engineMaxX = -0.8f * scale;
	float engineMinY = -0.2f * scale;
	float engineMaxY = 0.2f * scale;
	partRelativeBox_[ENGINE]->setDimensions(engineMinX, engineMaxX, engineMinY, engineMaxY);

	// Calculate and set global bounding box dimensions without std::min and std::max
	float globalMinX = bodyMinX;
	if (topWingMinX < globalMinX) globalMinX = topWingMinX;
	if (bottomWingMinX < globalMinX) globalMinX = bottomWingMinX;
	if (engineMinX < globalMinX) globalMinX = engineMinX;

	float globalMaxX = bodyMaxX;
	if (topWingMaxX > globalMaxX) globalMaxX = topWingMaxX;
	if (bottomWingMaxX > globalMaxX) globalMaxX = bottomWingMaxX;
	if (engineMaxX > globalMaxX) globalMaxX = engineMaxX;

	float globalMinY = bodyMinY;
	if (topWingMinY < globalMinY) globalMinY = topWingMinY;
	if (bottomWingMinY < globalMinY) globalMinY = bottomWingMinY;
	if (engineMinY < globalMinY) globalMinY = engineMinY;

	float globalMaxY = bodyMaxY;
	if (topWingMaxY > globalMaxY) globalMaxY = topWingMaxY;
	if (bottomWingMaxY > globalMaxY) globalMaxY = bottomWingMaxY;
	if (engineMaxY > globalMaxY) globalMaxY = engineMaxY;

	// Set the global bounding box dimensions
	setRelativeBoundingBox(globalMinX, globalMaxX, globalMinY, globalMaxY);
}

void SpaceShip::fireProjectile() {
	// Get the current time
	auto currentTime = std::chrono::high_resolution_clock::now();

	// Calculate the time since the last projectile was fired
	float timeSinceLastFire = std::chrono::duration<float>(currentTime - lastFireTime_).count();

	// Check if enough time has passed since the last projectile was fired
	if (timeSinceLastFire >= 1.0f / fireRate_) {
		float radAngle = M_PI * getAngle() / 180.0f;
		float projectileSpeed = 10.0f;

		// Calculate the projectile’s velocity based on the spaceship's current heading
		float vx = projectileSpeed * cosf(radAngle) + getVX();
		float vy = projectileSpeed * sinf(radAngle) + getVY();

		// Create a new projectile with a lifetime, initial position, and velocity
		Projectile::createProjectile(getX(), getY(), getAngle(), vx, vy, 1.75f);

		// Update the lastFireTime to the current time
		lastFireTime_ = currentTime;
	}
}

void SpaceShip::updateAbsoluteBox_() {
	// Get current position, scale, and rotation
	float cx = getX();
	float cy = getY();
	float scale = radius_; // Scale factor for the spaceship
	float angleRad = M_PI * getAngle() / 180.0f; // Convert angle to radians
	float cosA = cosf(angleRad);
	float sinA = sinf(angleRad);

	if (partAbsoluteBox_.empty()) {
		// Initialize bounding boxes if they haven't been created yet
		partAbsoluteBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::ORANGE)); // Body
		partAbsoluteBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::ORANGE)); // Left Wing
		partAbsoluteBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::ORANGE)); // Right Wing
		partAbsoluteBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::ORANGE)); // Engine
	}

	// Body (with nose) bounding box, accounting for rotation
	float bodyTipX = cx + cosA * scale;
	float bodyTipY = cy + sinA * scale;
	float bodyBackX = cx - 0.6f * cosA * scale;
	float bodyBackY = cy - 0.8f * sinA * scale;

	// Manually calculate min and max for the body
	float bodyMinX = (bodyTipX < bodyBackX ? bodyTipX : bodyBackX) - 0.6f * scale;
	float bodyMaxX = (bodyTipX > bodyBackX ? bodyTipX : bodyBackX) + 0.6f * scale;
	float bodyMinY = (bodyTipY < bodyBackY ? bodyTipY : bodyBackY) - 0.8f * scale;
	float bodyMaxY = (bodyTipY > bodyBackY ? bodyTipY : bodyBackY) + 1.0f * scale;
	partAbsoluteBox_[BODY]->setDimensions(bodyMinX, bodyMaxX, bodyMinY, bodyMaxY);

	// Left wing bounding box with rotation and scale
	float leftWingX = cx + (-0.7f * cosA - (-1.0f) * sinA) * scale;
	float leftWingY = cy + (-0.7f * sinA + (-1.0f) * cosA) * scale;
	float leftWingMinX = leftWingX - 0.3f * scale;
	float leftWingMaxX = leftWingX + 0.3f * scale;
	float leftWingMinY = leftWingY - 0.4f * scale;
	float leftWingMaxY = leftWingY + 0.4f * scale;
	partAbsoluteBox_[BOTTOM_WING]->setDimensions(leftWingMinX, leftWingMaxX, leftWingMinY, leftWingMaxY);

	// Right wing bounding box with rotation and scale
	float rightWingX = cx + (0.7f * cosA - (-1.0f) * sinA) * scale;
	float rightWingY = cy + (0.7f * sinA + (-1.0f) * cosA) * scale;
	float rightWingMinX = rightWingX - 0.3f * scale;
	float rightWingMaxX = rightWingX + 0.3f * scale;
	float rightWingMinY = rightWingY - 0.4f * scale;
	float rightWingMaxY = rightWingY + 0.4f * scale;
	partAbsoluteBox_[TOP_WING]->setDimensions(rightWingMinX, rightWingMaxX, rightWingMinY, rightWingMaxY);

	// Engine bounding box with rotation and scale
	float engineX = cx + (0.0f * cosA - (-0.9f) * sinA) * scale;
	float engineY = cy + (0.0f * sinA + (-0.9f) * cosA) * scale;
	float engineMinX = engineX - 0.2f * scale;
	float engineMaxX = engineX + 0.2f * scale;
	float engineMinY = engineY - 0.2f * scale;
	float engineMaxY = engineY + 0.2f * scale;
	partAbsoluteBox_[ENGINE]->setDimensions(engineMinX, engineMaxX, engineMinY, engineMaxY);

	// Initialize global bounding box based on the body
	float globalMinX = bodyMinX;
	float globalMaxX = bodyMaxX;
	float globalMinY = bodyMinY;
	float globalMaxY = bodyMaxY;

	// Update global bounding box based on all parts
	if (leftWingMinX < globalMinX) globalMinX = leftWingMinX;
	if (rightWingMinX < globalMinX) globalMinX = rightWingMinX;
	if (engineMinX < globalMinX) globalMinX = engineMinX;

	if (leftWingMaxX > globalMaxX) globalMaxX = leftWingMaxX;
	if (rightWingMaxX > globalMaxX) globalMaxX = rightWingMaxX;
	if (engineMaxX > globalMaxX) globalMaxX = engineMaxX;

	if (leftWingMinY < globalMinY) globalMinY = leftWingMinY;
	if (rightWingMinY < globalMinY) globalMinY = rightWingMinY;
	if (engineMinY < globalMinY) globalMinY = engineMinY;

	if (leftWingMaxY > globalMaxY) globalMaxY = leftWingMaxY;
	if (rightWingMaxY > globalMaxY) globalMaxY = rightWingMaxY;
	if (engineMaxY > globalMaxY) globalMaxY = engineMaxY;

	// Set the global bounding box
	setAbsoluteBoundingBox(globalMinX, globalMaxX, globalMinY, globalMaxY);
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

