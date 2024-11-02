//
//  SmilingFace.cpp
//  Week 08 - Earshooter
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <algorithm>
#include "glPlatform.h"
#include "World2D.h"
#include "SmilingFace.h"
#include "Ellipse2D.h"
#include <iostream>

using namespace earshooter;
using namespace std;


unsigned int SmilingFace::count_ = 0;
unsigned int SmilingFace::liveCount_ = 0;

const int SmilingFace::LEFT_EAR = 0;
const int SmilingFace::RIGHT_EAR = 1;
const int SmilingFace::FACE = 2;
const int SmilingFace::HEAD_NUM_PARTS = 3;
const int SmilingFace::HEAD = SmilingFace::HEAD_NUM_PARTS;
const ColorIndex SmilingFace::PART_BOX_COLOR[] =
							{ColorIndex::GREEN,	//	LEFT_EAR
							 ColorIndex::GREEN,	//	RIGHT_EAR
							 ColorIndex::RED,	//	SmilingFace
							 ColorIndex::BLUE};	//	HEAD

const float SmilingFace::FACE_RADIUS = 1.0f;
const float SmilingFace::EAR_RADIUS = FACE_RADIUS/2.f;
const float SmilingFace::EYE_OUTER_RADIUS = FACE_RADIUS/4.f;
const float SmilingFace::EYE_INNER_RADIUS = FACE_RADIUS/6.f;
const float SmilingFace::MOUTH_V_DIAMETER = FACE_RADIUS/2.f;
const float SmilingFace::MOUTH_H_DIAMETER = FACE_RADIUS/1.5f;
//
const float SmilingFace::EAR_V_OFFSET = 0.6f*FACE_RADIUS;
const float SmilingFace::EAR_H_OFFSET = 0.9f*FACE_RADIUS;
const float SmilingFace::EYE_V_OFFSET = 0.4f*FACE_RADIUS;
const float SmilingFace::EYE_H_OFFSET = 0.4f*FACE_RADIUS;
const float SmilingFace::LEFT_EAR_X  = -EAR_H_OFFSET;
const float SmilingFace::LEFT_EAR_Y  = +EAR_V_OFFSET;
const float SmilingFace::RIGHT_EAR_X = +EAR_H_OFFSET;
const float SmilingFace::RIGHT_EAR_Y = +EAR_V_OFFSET;
//
const float SmilingFace::LEFT_EYE_X  = -EYE_H_OFFSET;
const float SmilingFace::LEFT_EYE_Y  = EYE_V_OFFSET;
const float SmilingFace::RIGHT_EYE_X = EYE_H_OFFSET;
const float SmilingFace::RIGHT_EYE_Y = EYE_V_OFFSET;
//
const float SmilingFace::MOUTH_H_OFFSET = 0.f;
const float SmilingFace::MOUTH_V_OFFSET = -SmilingFace::FACE_RADIUS*0.04f;


#if 0
//--------------------------------------
#pragma mark -
#pragma mark Constructors
//--------------------------------------
#endif

SmilingFace::SmilingFace(float x, float y, float angle, float size,
						float r, float g, float b, float vx, float vy, float spin)
	: 	GraphicObject2D(x, y, angle,
						r, g, b, false,
						vx, vy, spin),
		size_(size),
		index_(count_++)
{
	updateRelativeBox_();
	updateAbsoluteBox_();
	

	liveCount_++;
}

SmilingFace::SmilingFace(float x, float y, float angle, float size,
						ColorIndex fillColor, float vx, float vy, float spin)
	:	SmilingFace(x, y, angle, size,
					COLOR[static_cast<int>(fillColor)][0],
					COLOR[static_cast<int>(fillColor)][1],
					COLOR[static_cast<int>(fillColor)][2],
					vx, vy, spin)
{
}

SmilingFace::SmilingFace(const WorldPoint& pt, float angle, float size,
						float r, float g, float b, float vx, float vy, float spin)
	: 	SmilingFace(pt.x, pt.y, angle, size, r, g, b, vx, vy, spin)
{
}

SmilingFace::SmilingFace(const WorldPoint& pt, float angle, float size,
						ColorIndex fillColor, float vx, float vy, float spin)
	:	SmilingFace(pt.x, pt.y, angle, size,
					COLOR[static_cast<int>(fillColor)][0],
					COLOR[static_cast<int>(fillColor)][1],
					COLOR[static_cast<int>(fillColor)][2],
					vx, vy, spin)
{
}


SmilingFace::~SmilingFace(void)
{
	liveCount_--;
}

#if 0
//-----------------------------------------
#pragma mark -
#pragma mark Drawing, geometry, animation
//-----------------------------------------
#endif

void SmilingFace::draw_(void) const
{
	float r = getR(), g = getG(), b = getB();
	
	glPushMatrix();

	glScalef(size_, size_, 1.f);
	
	glColor4f(r, g, b, 1.f);
	glPushMatrix();
	glScalef(FACE_RADIUS, FACE_RADIUS, 1.f);
	drawDisk();
	glPopMatrix();

	//	draw the left ear
	glPushMatrix();
	glTranslatef(LEFT_EAR_X, LEFT_EAR_Y, 0.f);
	glScalef(EAR_RADIUS, EAR_RADIUS, 1.f);
	drawDisk();
	glPopMatrix();

	//	draw the right ear
	glPushMatrix();
	glTranslatef(RIGHT_EAR_X, RIGHT_EAR_Y, 0.f);
	glScalef(EAR_RADIUS, EAR_RADIUS, 1.f);
	drawDisk();
	glPopMatrix();

	//	draw the left and right eyes (I could have gone the Push&pop way here as
	//	well, and would if the eyes were more complex, but here they are simply
	//	ellipses, so no need to make it over-complex.
	//	White of eye
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glPushMatrix();
	glTranslatef(LEFT_EYE_X, LEFT_EYE_Y, 0.f);
	glScalef(EYE_OUTER_RADIUS, EYE_OUTER_RADIUS, 1.f);
	drawDisk();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(RIGHT_EYE_X, RIGHT_EYE_Y, 0.f);
	glScalef(EYE_OUTER_RADIUS, EYE_OUTER_RADIUS, 1.f);
	drawDisk();
	glPopMatrix();
	// pupils
	glColor4f(0.f, 0.f, 0.f, 1.f);
	glPushMatrix();
	glTranslatef(LEFT_EYE_X, LEFT_EYE_Y, 0.f);
	glScalef(EYE_INNER_RADIUS, EYE_INNER_RADIUS, 1.f);
	drawDisk();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(RIGHT_EYE_X, RIGHT_EYE_Y, 0.f);
	glScalef(EYE_INNER_RADIUS, EYE_INNER_RADIUS, 1.f);
	drawDisk();
	glPopMatrix();

	glTranslatef(MOUTH_H_OFFSET, MOUTH_V_OFFSET, 0.f);
	glScalef(MOUTH_H_DIAMETER, MOUTH_V_DIAMETER, 1.f);
	glColor3f(0.f, 0.f, 0.f);
	glLineWidth(3.f);
	drawArc(0.7f, 0.85f);
	glLineWidth(1.f);



	

	//	restore the original scale
	glPopMatrix();

}

bool SmilingFace::isInside(float x, float y) const
{
	bool inside = false;

	return inside;
}

void SmilingFace::updateRelativeBox_() {
	// Initialize partRelativeBox_ if empty
	if (partRelativeBox_.empty()) {
		partRelativeBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::YELLOW)); // Face
		partRelativeBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::YELLOW)); // Left Ear
		partRelativeBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::YELLOW)); // Right Ear
	}

	float scale = size_;

	// Relative bounding box for the face, centered at (0, 0)
	float faceMinX = -FACE_RADIUS * scale;
	float faceMaxX = FACE_RADIUS * scale;
	float faceMinY = -FACE_RADIUS * scale;
	float faceMaxY = FACE_RADIUS * scale;
	partRelativeBox_[FACE]->setDimensions(faceMinX, faceMaxX, faceMinY, faceMaxY);

	// Relative bounding box for the left ear, centered at (LEFT_EAR_X, LEFT_EAR_Y)
	float leftEarCenterX = LEFT_EAR_X * scale;
	float leftEarCenterY = LEFT_EAR_Y * scale;
	float leftEarMinX = leftEarCenterX - EAR_RADIUS * scale;
	float leftEarMaxX = leftEarCenterX + EAR_RADIUS * scale;
	float leftEarMinY = leftEarCenterY - EAR_RADIUS * scale;
	float leftEarMaxY = leftEarCenterY + EAR_RADIUS * scale;
	partRelativeBox_[LEFT_EAR]->setDimensions(leftEarMinX, leftEarMaxX, leftEarMinY, leftEarMaxY);

	// Relative bounding box for the right ear, centered at (RIGHT_EAR_X, RIGHT_EAR_Y)
	float rightEarCenterX = RIGHT_EAR_X * scale;
	float rightEarCenterY = RIGHT_EAR_Y * scale;
	float rightEarMinX = rightEarCenterX - EAR_RADIUS * scale;
	float rightEarMaxX = rightEarCenterX + EAR_RADIUS * scale;
	float rightEarMinY = rightEarCenterY - EAR_RADIUS * scale;
	float rightEarMaxY = rightEarCenterY + EAR_RADIUS * scale;
	partRelativeBox_[RIGHT_EAR]->setDimensions(rightEarMinX, rightEarMaxX, rightEarMinY, rightEarMaxY);

	// Set initial values for the global bounding box based on the face's bounding box
	float globalMinX = faceMinX;
	float globalMaxX = faceMaxX;
	float globalMinY = faceMinY;
	float globalMaxY = faceMaxY;

	// Update global bounding box based on left ear
	if (leftEarMinX < globalMinX) globalMinX = leftEarMinX;
	if (leftEarMaxX > globalMaxX) globalMaxX = leftEarMaxX;
	if (leftEarMinY < globalMinY) globalMinY = leftEarMinY;
	if (leftEarMaxY > globalMaxY) globalMaxY = leftEarMaxY;

	// Update global bounding box based on right ear
	if (rightEarMinX < globalMinX) globalMinX = rightEarMinX;
	if (rightEarMaxX > globalMaxX) globalMaxX = rightEarMaxX;
	if (rightEarMinY < globalMinY) globalMinY = rightEarMinY;
	if (rightEarMaxY > globalMaxY) globalMaxY = rightEarMaxY;

	// Set the global bounding box dimensions
	setRelativeBoundingBox(globalMinX, globalMaxX, globalMinY, globalMaxY);
}


void SmilingFace::updateAbsoluteBox_() {
	// Get current position, scale, and rotation
	float cx = getX();
	float cy = getY();
	float scale = size_;
	float angleRad = M_PI * getAngle() / 180.f; // Convert angle to radians for rotation
	float cosA = cosf(angleRad);
	float sinA = sinf(angleRad);

	if (partAbsoluteBox_.empty()) {
		// Initialize bounding boxes if they haven't been created yet

		// Index 0 for face, 1 for left ear, and 2 for right ear
		partAbsoluteBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::ORANGE)); // Face
		partAbsoluteBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::ORANGE)); // Left Ear
		partAbsoluteBox_.emplace_back(std::make_unique<BoundingBox>(0, 0, 0, 0, ColorIndex::ORANGE)); // Right Ear
	}

	// Face bounding box with transformation
	float faceMinX = cx - FACE_RADIUS * scale;
	float faceMaxX = cx + FACE_RADIUS * scale;
	float faceMinY = cy - FACE_RADIUS * scale;
	float faceMaxY = cy + FACE_RADIUS * scale;
	partAbsoluteBox_[FACE]->setDimensions(faceMinX, faceMaxX, faceMinY, faceMaxY);

	// Left ear bounding box with rotation and scale
	float leftEarX = cx + (LEFT_EAR_X * cosA - LEFT_EAR_Y * sinA) * scale;
	float leftEarY = cy + (LEFT_EAR_X * sinA + LEFT_EAR_Y * cosA) * scale;
	float leftEarMinX = leftEarX - EAR_RADIUS * scale;
	float leftEarMaxX = leftEarX + EAR_RADIUS * scale;
	float leftEarMinY = leftEarY - EAR_RADIUS * scale;
	float leftEarMaxY = leftEarY + EAR_RADIUS * scale;
	partAbsoluteBox_[LEFT_EAR]->setDimensions(leftEarMinX, leftEarMaxX, leftEarMinY, leftEarMaxY);

	// Right ear bounding box with rotation and scale
	float rightEarX = cx + (RIGHT_EAR_X * cosA - RIGHT_EAR_Y * sinA) * scale;
	float rightEarY = cy + (RIGHT_EAR_X * sinA + RIGHT_EAR_Y * cosA) * scale;
	float rightEarMinX = rightEarX - EAR_RADIUS * scale;
	float rightEarMaxX = rightEarX + EAR_RADIUS * scale;
	float rightEarMinY = rightEarY - EAR_RADIUS * scale;
	float rightEarMaxY = rightEarY + EAR_RADIUS * scale;
	partAbsoluteBox_[RIGHT_EAR]->setDimensions(rightEarMinX, rightEarMaxX, rightEarMinY, rightEarMaxY);


	float globalMinX = faceMinX;
	float globalMaxX = faceMaxX;
	float globalMinY = faceMinY;
	float globalMaxY = faceMaxY;

	// Compare each coordinate to determine the global min/max
	if (leftEarMinX < globalMinX) globalMinX = leftEarMinX;
	if (rightEarMinX < globalMinX) globalMinX = rightEarMinX;

	if (leftEarMaxX > globalMaxX) globalMaxX = leftEarMaxX;
	if (rightEarMaxX > globalMaxX) globalMaxX = rightEarMaxX;

	if (leftEarMinY < globalMinY) globalMinY = leftEarMinY;
	if (rightEarMinY < globalMinY) globalMinY = rightEarMinY;

	if (leftEarMaxY > globalMaxY) globalMaxY = leftEarMaxY;
	if (rightEarMaxY > globalMaxY) globalMaxY = rightEarMaxY;

	// Set the global bounding box
	setAbsoluteBoundingBox(globalMinX, globalMaxX, globalMinY, globalMaxY);
}



#if 0
//--------------------------------------
#pragma mark -
#pragma mark Setters and getters
//--------------------------------------
#endif

unsigned int SmilingFace::getIndex(void) const
{
	return index_;
}

unsigned int SmilingFace::getCount(void)
{
	return count_;
}
unsigned int SmilingFace::getLiveCount(void)
{
	return liveCount_;
}
