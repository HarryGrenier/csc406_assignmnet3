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

void SmilingFace::updateAbsoluteBox_() {
	// Center position and scale
	float cx = getX();
	float cy = getY();
	float scale = size_;
	float angleRad = M_PI * getAngle() / 180.f;
	float cosA = cosf(angleRad);
	float sinA = sinf(angleRad);

	// Rotate and translate each component relative to the face’s center
	// Face itself
	float faceMinX = cx - FACE_RADIUS * scale;
	float faceMaxX = cx + FACE_RADIUS * scale;
	float faceMinY = cy - FACE_RADIUS * scale;
	float faceMaxY = cy + FACE_RADIUS * scale;

	// Left ear
	float leftEarX = cx + (LEFT_EAR_X * cosA - LEFT_EAR_Y * sinA) * scale;
	float leftEarY = cy + (LEFT_EAR_X * sinA + LEFT_EAR_Y * cosA) * scale;
	float leftEarMinX = leftEarX - EAR_RADIUS * scale;
	float leftEarMaxX = leftEarX + EAR_RADIUS * scale;
	float leftEarMinY = leftEarY - EAR_RADIUS * scale;
	float leftEarMaxY = leftEarY + EAR_RADIUS * scale;

	// Right ear
	float rightEarX = cx + (RIGHT_EAR_X * cosA - RIGHT_EAR_Y * sinA) * scale;
	float rightEarY = cy + (RIGHT_EAR_X * sinA + RIGHT_EAR_Y * cosA) * scale;
	float rightEarMinX = rightEarX - EAR_RADIUS * scale;
	float rightEarMaxX = rightEarX + EAR_RADIUS * scale;
	float rightEarMinY = rightEarY - EAR_RADIUS * scale;
	float rightEarMaxY = rightEarY + EAR_RADIUS * scale;

	// Left eye
	float leftEyeX = cx + (LEFT_EYE_X * cosA - LEFT_EYE_Y * sinA) * scale;
	float leftEyeY = cy + (LEFT_EYE_X * sinA + LEFT_EYE_Y * cosA) * scale;
	float leftEyeMinX = leftEyeX - EYE_OUTER_RADIUS * scale;
	float leftEyeMaxX = leftEyeX + EYE_OUTER_RADIUS * scale;
	float leftEyeMinY = leftEyeY - EYE_OUTER_RADIUS * scale;
	float leftEyeMaxY = leftEyeY + EYE_OUTER_RADIUS * scale;

	// Right eye
	float rightEyeX = cx + (RIGHT_EYE_X * cosA - RIGHT_EYE_Y * sinA) * scale;
	float rightEyeY = cy + (RIGHT_EYE_X * sinA + RIGHT_EYE_Y * cosA) * scale;
	float rightEyeMinX = rightEyeX - EYE_OUTER_RADIUS * scale;
	float rightEyeMaxX = rightEyeX + EYE_OUTER_RADIUS * scale;
	float rightEyeMinY = rightEyeY - EYE_OUTER_RADIUS * scale;
	float rightEyeMaxY = rightEyeY + EYE_OUTER_RADIUS * scale;

	// Mouth
	float mouthX = cx + (MOUTH_H_OFFSET * cosA - MOUTH_V_OFFSET * sinA) * scale;
	float mouthY = cy + (MOUTH_H_OFFSET * sinA + MOUTH_V_OFFSET * cosA) * scale;
	float mouthMinX = mouthX - MOUTH_H_DIAMETER * scale / 2;
	float mouthMaxX = mouthX + MOUTH_H_DIAMETER * scale / 2;
	float mouthMinY = mouthY - MOUTH_V_DIAMETER * scale / 2;
	float mouthMaxY = mouthY + MOUTH_V_DIAMETER * scale / 2;

	// Determine the overall min and max values for the bounding box
	float minX = faceMinX;
	float maxX = faceMaxX;
	float minY = faceMinY;
	float maxY = faceMaxY;

	// Compare with each component's bounds
	// Manually calculate the overall min and max values for x and y
	if (leftEarMinX < minX) minX = leftEarMinX;
	if (rightEarMinX < minX) minX = rightEarMinX;
	if (leftEyeMinX < minX) minX = leftEyeMinX;
	if (rightEyeMinX < minX) minX = rightEyeMinX;
	if (mouthMinX < minX) minX = mouthMinX;

	if (leftEarMaxX > maxX) maxX = leftEarMaxX;
	if (rightEarMaxX > maxX) maxX = rightEarMaxX;
	if (leftEyeMaxX > maxX) maxX = leftEyeMaxX;
	if (rightEyeMaxX > maxX) maxX = rightEyeMaxX;
	if (mouthMaxX > maxX) maxX = mouthMaxX;

	if (leftEarMinY < minY) minY = leftEarMinY;
	if (rightEarMinY < minY) minY = rightEarMinY;
	if (leftEyeMinY < minY) minY = leftEyeMinY;
	if (rightEyeMinY < minY) minY = rightEyeMinY;
	if (mouthMinY < minY) minY = mouthMinY;

	if (leftEarMaxY > maxY) maxY = leftEarMaxY;
	if (rightEarMaxY > maxY) maxY = rightEarMaxY;
	if (leftEyeMaxY > maxY) maxY = leftEyeMaxY;
	if (rightEyeMaxY > maxY) maxY = rightEyeMaxY;
	if (mouthMaxY > maxY) maxY = mouthMaxY;

	// Set the absolute bounding box with the calculated values
	setAbsoluteBoundingBox(minX, maxX, minY, maxY);
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
