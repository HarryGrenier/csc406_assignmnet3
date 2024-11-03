#include "Projectile.h"
#include <algorithm>
#include "glPlatform.h"
using namespace earshooter;

// Initialize the static vector of projectiles
std::vector<std::shared_ptr<Projectile>> Projectile::projectiles_;

Projectile::Projectile(float x, float y, float angle, float vx, float vy, float lifetime)
    : Ellipse2D(x, y, angle, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, false, vx, vy, 0.0f),
    lifetime_(lifetime) {}

void Projectile::createProjectile(float x, float y, float angle, float vx, float vy, float lifetime) {
    auto projectile = std::make_shared<Projectile>(x, y, angle, vx, vy, lifetime);
    projectiles_.push_back(projectile);
}

void Projectile::updateProjectiles(float dt) {
    for (auto& projectile : projectiles_) {
        projectile->update(dt);
        projectile->lifetime_ -= dt;
    }
    removeExpiredProjectiles();
}


void Projectile::removeExpiredProjectiles() {
    projectiles_.erase(
        std::remove_if(projectiles_.begin(), projectiles_.end(),
            [](const std::shared_ptr<Projectile>& projectile) {
                return projectile->lifetime_ <= 0;
            }),
        projectiles_.end());
}

void Projectile::drawProjectiles() {
    for (const auto& projectile : projectiles_) {
        projectile->draw();
    }
}
