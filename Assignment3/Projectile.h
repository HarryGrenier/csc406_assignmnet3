#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GraphicObject2D.h"
#include <list>
#include <memory>

namespace earshooter {
    class Projectile : public GraphicObject2D {
    private:
        float width_, height_;
        float lifetime_;  // Lifetime of the projectile

        /** creation index of the projectile */
        unsigned int index_;

        /** Counters for number of projectiles created and active */
        static unsigned int count_;
        static unsigned int liveCount_;

        /** Pointer to the list of all objects, for collision detection */
        static const std::list<std::shared_ptr<GraphicObject2D>>* objList_;

        void draw_() const override;
        void updateRelativeBox_();
        void updateAbsoluteBox_() override;

    public:
        Projectile(float x, float y, float angle, float width, float height,
            float r, float g, float b, bool drawContour,
            float vx = 0.f, float vy = 0.f, float spin = 0.f, float lifetime = 1.5f);

        Projectile(float x, float y, float angle, float width, float height,
            ColorIndex fillColor, bool drawContour,
            float vx = 0.f, float vy = 0.f, float spin = 0.f, float lifetime = 1.5f);

        ~Projectile();

        unsigned int getIndex() const override;
        bool isInside(float x, float y) const override;

        static unsigned int getCount();
        static unsigned int getLiveCount();

        UpdateStatus update(float dt) override;

        static void setObjectList(const std::list<std::shared_ptr<GraphicObject2D>>* objListPtr);
        static void createProjectile(float x, float y, float angle, float vx, float vy, float lifetime);
    };
}

#endif // PROJECTILE_H
