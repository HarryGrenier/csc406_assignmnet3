
#define _USE_MATH_DEFINES
#include "Projectile.h"
#include "BoundingBox.h"
#include "glPlatform.h"
#include <iostream> // For debugging output if needed
#include <cmath>

using namespace std;
using namespace earshooter;

unsigned int Projectile::count_ = 0;
unsigned int Projectile::liveCount_ = 0;
const std::list<std::shared_ptr<GraphicObject2D>>* Projectile::objList_ = nullptr;

Projectile::Projectile(float centerX, float centerY, float angle, float width, float height,
    float r, float g, float b, bool drawContour, float vx, float vy, float spin, float lifetime)
    : GraphicObject2D(centerX, centerY, angle, r, g, b, drawContour, vx, vy, spin),
    width_(width), height_(height), lifetime_(lifetime), index_(count_++) {
    updateRelativeBox_();
    updateAbsoluteBox_();
    liveCount_++;
}

Projectile::~Projectile() {
    liveCount_--;
}

void Projectile::draw_() const {
    glPushMatrix();
    glScalef(width_, height_, 1.f);
    glColor3f(getR(), getG(), getB());
    glBegin(GL_POLYGON);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(+0.5f, -0.5f);
    glVertex2f(+0.5f, +0.5f);
    glVertex2f(-0.5f, +0.5f);
    glEnd();
    glPopMatrix();
}

bool Projectile::isInside(float x, float y) const {
    float dx = x - getX();
    float dy = y - getY();
    float radAngle = M_PI * getAngle() / 180.f;
    float cosA = cosf(radAngle), sinA = sinf(radAngle);
    float localX = cosA * dx + sinA * dy;
    float localY = -sinA * dx + cosA * dy;
    return (fabs(localX) <= width_ / 2) && (fabs(localY) <= height_ / 2);
}

void Projectile::updateRelativeBox_() {
    setRelativeBoundingBox(-width_ / 2, width_ / 2, -height_ / 2, height_ / 2);
}

void Projectile::updateAbsoluteBox_() {
    // Calculate bounding box corners with rotation
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

UpdateStatus Projectile::update(float dt) {
    lifetime_ -= dt * 2.0f;  // Increase decay rate
    if (lifetime_ <= 0) {
        return UpdateStatus::DEAD;
    }

    // Collision detection logic
    for (const auto& obj : *objList_) {
        if (obj && obj.get() != this && obj->getObjectType() == ObjectType::Generic &&
            this->getAbsoluteBoundingBox().intersects(obj->getAbsoluteBoundingBox())) {

            obj->setDead(true);
            return UpdateStatus::DEAD;
        }
    }

    setPosition(getX() + getVX() * dt, getY() + getVY() * dt);
    return GraphicObject2D::update(dt);
}

void Projectile::setObjectList(const std::list<std::shared_ptr<GraphicObject2D>>* objListPtr) {
    objList_ = objListPtr;
}

void Projectile::createProjectile(float x, float y, float angle, float vx, float vy, float lifetime) {
    if (!objList_) return;
    auto projectile = std::make_shared<Projectile>(x, y, angle, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, false, vx, vy, 0.0f, lifetime);
    const_cast<std::list<std::shared_ptr<GraphicObject2D>>*>(objList_)->push_back(projectile);
}

unsigned int Projectile::getIndex() const {
    return index_;
}

unsigned int Projectile::getCount() {
    return count_;
}

unsigned int Projectile::getLiveCount() {
    return liveCount_;
}
