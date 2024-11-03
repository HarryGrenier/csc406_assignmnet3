#include <vector>
#include <memory>
#include "GraphicObject2D.h"
#include "Ellipse2D.h"

namespace earshooter {
    class Projectile : public Ellipse2D {
    public:
        Projectile(float x, float y, float angle, float vx, float vy, float lifetime);

        static void createProjectile(float x, float y, float angle, float vx, float vy, float lifetime);
        static void updateProjectiles(float dt);
        static void drawProjectiles();
        static void removeExpiredProjectiles();

    private:
        float lifetime_;
        static std::vector<std::shared_ptr<Projectile>> projectiles_;
    };
}
