//
//  GraphicObject2D.cpp
//  Week 05 - Prog 1
//
//  Created by Jean-Yves Hervé on 2024-09-19.
//

#include "glPlatform.h"
#include "GraphicObject2D.h"

using namespace std;
using namespace earshooter;

unsigned int GraphicObject2D::count_ = 0;
unsigned int GraphicObject2D::liveCount_ = 0;

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Constructors
//--------------------------------------
#endif

GraphicObject2D::GraphicObject2D(float cx, float cy, float angle,
						float r, float g, float b, bool drawContour,
						float vx, float vy, float spin)
	:	cx_(cx),
		cy_(cy),
		angle_(angle),
		r_(r),
		g_(g),
		b_(b),
		drawContour_(drawContour),
		vx_(vx),
		vy_(vy),
		spin_(spin),
		relativeBox_(nullptr),
		absoluteBox_(nullptr),
		index_(count_++)
{
	liveCount_++;
}

GraphicObject2D::GraphicObject2D(const WorldPoint& pt, float angle,
						float r, float g, float b, bool drawContour,
						float vx, float vy, float spin)
	:	GraphicObject2D(pt.x, pt.y, angle,
						r, g, b, drawContour,
						vx, vy, spin)
{
	liveCount_++;
}

GraphicObject2D::GraphicObject2D(float cx, float cy, float angle,
								 ColorIndex fillColor, bool drawContour,
								 float vx, float vy, float spin)
	:	GraphicObject2D(cx, cy, angle,
						COLOR[static_cast<int>(fillColor)][0],
						COLOR[static_cast<int>(fillColor)][1],
						COLOR[static_cast<int>(fillColor)][2],
						drawContour,
						vx, vy, spin)
{
}

GraphicObject2D::GraphicObject2D(const WorldPoint& pt, float angle,
								 ColorIndex fillColor, bool drawContour,
								 float vx, float vy, float spin)
	:	GraphicObject2D(pt.x, pt.y, angle,
						COLOR[static_cast<int>(fillColor)][0],
						COLOR[static_cast<int>(fillColor)][1],
						COLOR[static_cast<int>(fillColor)][2],
						drawContour,
						vx, vy, spin)
{
}


GraphicObject2D::~GraphicObject2D()
{
	liveCount_--;
}


#if 0
//--------------------------------------
#pragma mark -
#pragma mark Rendering and animation
//--------------------------------------
#endif

UpdateStatus GraphicObject2D::update(float dt)
{
	UpdateStatus status = UpdateStatus::NORMAL;
	cx_ += vx_*dt;
	cy_ += vy_*dt;
	angle_ += spin_*dt;
	
	//	different behaviors based on World2D::worldType
	switch(World2D::worldType)
	{
		case WorldType::WINDOW_WORLD:
			//	I add (a lot of) "padding" to my out of bounds test
			//
			if (cx_ > World2D::X_MAX + 0.5f*World2D::WIDTH ||
				cx_ < World2D::X_MIN - 0.5f*World2D::WIDTH ||
				cy_ > World2D::Y_MAX + 0.5f*World2D::HEIGHT ||
				cy_ < World2D::Y_MIN - 0.5f*World2D::HEIGHT)
			{
				status = UpdateStatus::DEAD;
			}
		break;

		case WorldType::BOX_WORLD:
			//	hit the right edge
			if (getX() >= World2D::X_MAX) {
				cx_ = World2D::X_MAX;
				vx_ = -vx_;
				status = UpdateStatus::BOUNCE;
			}
			//	hit the left edge
			if (getX() <= World2D::X_MIN) {
				cx_ = World2D::X_MIN;
				vx_ = -vx_;
				status = UpdateStatus::BOUNCE;
			}
			//	hit the top edge
			if (getY() >= World2D::Y_MAX) {
				cy_ = World2D::Y_MAX;
				vy_ = -vy_;
				status = UpdateStatus::BOUNCE;
			}
			//	hit the bottom edge
			if (getY() <= World2D::Y_MIN) {
				cy_ = World2D::Y_MIN;
				vy_ = -vy_;
				status = UpdateStatus::BOUNCE;
			}
		break;

		case WorldType::CYLINDER_WORLD:
			//	hit the right edge
			if (getX() >= World2D::X_MAX) {
				cx_ -= World2D::WIDTH;
				status = UpdateStatus::WRAPAROUND;
			}
			//	hit the left edge
			if (getX() <= World2D::X_MIN) {
				cx_ += World2D::WIDTH;
				status = UpdateStatus::WRAPAROUND;
			}
			//	hit the top edge
			if (getY() >= World2D::Y_MAX) {
				cy_ = World2D::Y_MAX;
				vy_ = -vy_;
				status = UpdateStatus::BOUNCE;
			}
			//	hit the bottom edge
			if (getY() <= World2D::Y_MIN) {
				cy_ = World2D::Y_MIN;
				vy_ = -vy_;
				status = UpdateStatus::BOUNCE;
			}
		break;

		case WorldType::SPHERE_WORLD:
		break;
	
		default:
			break;
	}
	
	//	Update the bounding boxes (if they exist)
	//	Simple (i.e. not complex, not made up of parts) objects' relative bounding
	//	box doesn't change
	if (absoluteBox_ != nullptr)
		updateAbsoluteBox_();

	return status;
}


void GraphicObject2D::draw() const
{
	glPushMatrix();
	glTranslatef(cx_, cy_, 0.f);
	glRotatef(angle_, 0, 0, 1);
	
	//	call the object's private drawing function
	draw_();
	
	if (BoundingBox::relativeBoxesAreDrawn() && relativeBox_ != nullptr)
	{
		relativeBox_->draw();
	}
	else if (BoundingBox::absoluteBoxesAreDrawn() && absoluteBox_ != nullptr)
	{
		glPushMatrix();
		glLoadIdentity();

		absoluteBox_->draw();
		glPopMatrix();
	}
	
	if (BoundingBox::relativeBoxesAreDrawn() && !partRelativeBox_.empty()) {
		for (const auto& box : partRelativeBox_) {
			if (box) {
				box->draw();
			}
		}
	}
	else if (BoundingBox::absoluteBoxesAreDrawn() && !partAbsoluteBox_.empty())
	{
		for (const auto& box : partAbsoluteBox_) {
			if (box) {
				glPushMatrix();
				glLoadIdentity();

				box->draw();
				glPopMatrix();
			}
		}
	}

	if (World2D::drawReferenceFrames)
		drawReferenceFrame();

	glPopMatrix();
}

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Bounding boxes
//--------------------------------------
#endif

void GraphicObject2D::setAbsoluteBoundingBox(float xmin, float xmax, float ymin, float ymax)
{
	if (absoluteBox_ != nullptr)
		absoluteBox_->setDimensions(xmin, xmax, ymin, ymax);
	else
		absoluteBox_ = make_unique<BoundingBox>(xmin, xmax, ymin, ymax, ColorIndex::RED);
		
}
void GraphicObject2D::setAbsoluteBoundingBox(const WorldPoint& ul, const WorldPoint& lr)
{
	if (absoluteBox_ != nullptr)
		absoluteBox_->setDimensions(ul, lr);
	else
		absoluteBox_ = make_unique<BoundingBox>(ul, lr, ColorIndex::RED);
		
}
void GraphicObject2D::setRelativeBoundingBox(float xmin, float xmax, float ymin, float ymax)
{
	if (relativeBox_ != nullptr)
		relativeBox_->setDimensions(xmin, xmax, ymin, ymax);
	else
		 relativeBox_ = make_unique<BoundingBox>(xmin, xmax, ymin, ymax, ColorIndex::GREEN);
		
}

void GraphicObject2D::setRelativeBoundingBox(const WorldPoint& ul, const WorldPoint& lr)
{
	if (relativeBox_ != nullptr)
		relativeBox_->setDimensions(ul, lr);
	else
		relativeBox_ = make_unique<BoundingBox>(ul, lr, ColorIndex::GREEN);
		
}

const BoundingBox& GraphicObject2D::getAbsoluteBoundingBox() const
{
	if (absoluteBox_ != nullptr)
		return *absoluteBox_;
	else
		return BoundingBox::NULL_BOX;
		
}
const BoundingBox& GraphicObject2D::getRelativeBoundingBox() const
{
	if (relativeBox_ != nullptr)
		return *relativeBox_;
	else
		return BoundingBox::NULL_BOX;
		
}

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Setters and getters
//--------------------------------------
#endif
		
void GraphicObject2D::setPosition(float x, float y)
{
	cx_ = x;
	cy_ = y;
}
void GraphicObject2D::setPosition(const WorldPoint& pt)
{
	cx_ = pt.x;
	cy_ = pt.y;
}

void GraphicObject2D::setAngle(float angle)
{
	angle_ = angle;
}

void GraphicObject2D::setColor(float r, float g, float b)
{
	r_ = r;
	g_ = g;
	b_ = b;
}

void GraphicObject2D::setDrawContour(bool drawContour)
{
	drawContour_ = drawContour;
}

unsigned int GraphicObject2D::getBaseCount(void)
{
	return count_;
}

unsigned int GraphicObject2D::getBaseLiveCount(void)
{
	return liveCount_;
}
