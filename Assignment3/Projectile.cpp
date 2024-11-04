#define _USE_MATH_DEFINES
#include "Projectile.h"
#include "BoundingBox.h"
#include "glPlatform.h"
#include <iostream> // For debugging output if needed
#include <cmath>

using namespace std;
using namespace earshooter;

// Initialize static counters and object list pointer
unsigned int Projectile::count_ = 0;
unsigned int Projectile::liveCount_ = 0;
const std::list<std::shared_ptr<GraphicObject2D>>* Projectile::objList_ = nullptr;

// Constructor for creating a projectile with specified parameters
Projectile::Projectile(float centerX, float centerY, float angle, float width, float height,
    float r, float g, float b, bool drawContour, float vx, float vy, float spin, float lifetime)
    : GraphicObject2D(centerX, centerY, angle, r, g, b, drawContour, vx, vy, spin),
    width_(width), height_(height), lifetime_(lifetime), index_(count_++) {
    updateRelativeBox_();
    updateAbsoluteBox_();
    liveCount_++;
}

// Destructor for projectile, decrements the live count
Projectile::~Projectile() {
    liveCount_--;
}

// Draw the projectile as a scaled square
void Projectile::draw_() const {
    glPushMatrix();
    glScalef(width_, height_, 1.f); // Scale based on width and height
    glColor3f(getR(), getG(), getB()); // Set color
    glBegin(GL_POLYGON); // Draw square
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(+0.5f, -0.5f);
    glVertex2f(+0.5f, +0.5f);
    glVertex2f(-0.5f, +0.5f);
    glEnd();
    glPopMatrix();
}

// Check if a point (x, y) is inside the projectile's bounds
bool Projectile::isInside(float x, float y) const {
    float dx = x - getX();
    float dy = y - getY();
    float radAngle = M_PI * getAngle() / 180.f; // Convert angle to radians
    float cosA = cosf(radAngle), sinA = sinf(radAngle);
    float localX = cosA * dx + sinA * dy; // Rotate point to local coordinates
    float localY = -sinA * dx + cosA * dy;
    return (fabs(localX) <= width_ / 2) && (fabs(localY) <= height_ / 2); // Check if within bounds
}

// Update the relative bounding box dimensions
void Projectile::updateRelativeBox_() {
    setRelativeBoundingBox(-width_ / 2, width_ / 2, -height_ / 2, height_ / 2);
}

// Update the absolute bounding box with rotation applied
void Projectile::updateAbsoluteBox_() {
    float halfWidth = width_ / 2;
    float halfHeight = height_ / 2;
    float cosA = cosf(M_PI * getAngle() / 180.f);
    float sinA = sinf(M_PI * getAngle() / 180.f);

    float xMin = getX() - (halfWidth * fabs(cosA) + halfHeight * fabs(sinA));
    float xMax = getX() + (halfWidth * fabs(cosA) + halfHeight * fabs(sinA));
    float yMin = getY() - (halfHeight * fabs(cosA) + halfWidth * fabs(sinA));
    float yMax = getY() + (halfHeight * fabs(cosA) + halfWidth * fabs(sinA));

    setAbsoluteBoundingBox(xMin, xMax, yMin, yMax);
}

// Update the projectile state and check for collisions
UpdateStatus Projectile::update(float dt) {
    lifetime_ -= dt * 2.0f;  // Decrease lifetime at a set rate
    if (lifetime_ <= 0) {
        return UpdateStatus::DEAD;
    }

    // Check for collisions with generic objects in the object list
    for (const auto& obj : *objList_) {
        if (obj && obj.get() != this && obj->getObjectType() == ObjectType::Generic &&
            this->getAbsoluteBoundingBox().intersects(obj->getAbsoluteBoundingBox())) {
            obj->setDead(true); // Mark the object as dead on collision
            return UpdateStatus::DEAD;
        }
    }

    // Update position based on velocity
    setPosition(getX() + getVX() * dt, getY() + getVY() * dt);
    return GraphicObject2D::update(dt);
}

// Set the object list pointer for collision detection
void Projectile::setObjectList(const std::list<std::shared_ptr<GraphicObject2D>>* objListPtr) {
    objList_ = objListPtr;
}

// Static function to create and add a new projectile to the object list
void Projectile::createProjectile(float x, float y, float angle, float vx, float vy, float lifetime) {
    if (!objList_) return; // Ensure object list is set
    auto projectile = std::make_shared<Projectile>(x, y, angle, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, false, vx, vy, 0.0f, lifetime);
    const_cast<std::list<std::shared_ptr<GraphicObject2D>>*>(objList_)->push_back(projectile); // Add to list
}

// Get the unique index of the projectile
unsigned int Projectile::getIndex() const {
    return index_;
}

// Get the total count of projectiles created
unsigned int Projectile::getCount() {
    return count_;
}

// Get the current count of live projectiles
unsigned int Projectile::getLiveCount() {
    return liveCount_;
}
