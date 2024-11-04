#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GraphicObject2D.h"
#include <list>
#include <memory>

namespace earshooter {
    /**
     * @class Projectile
     * @brief Represents a projectile in a 2D space, with properties for rendering, collision detection, and lifetime.
     */
    class Projectile : public GraphicObject2D {
    private:
        /** Width of the projectile */
        float width_;

        /** Height of the projectile */
        float height_;

        /** Lifetime of the projectile */
        float lifetime_;

        /** Unique creation index of the projectile */
        unsigned int index_;

        /** Counter for the total number of projectiles created */
        static unsigned int count_;

        /** Counter for the number of active projectiles */
        static unsigned int liveCount_;

        /** Pointer to the list of all objects, used for collision detection */
        static const std::list<std::shared_ptr<GraphicObject2D>>* objList_;

        /** Private rendering function for the Projectile class.
         * Translation and rotation are applied by the root class,
         * so this function only applies scaling before rendering.
         */
        void draw_() const override;

        /** Updates the relative bounding box of the projectile */
        void updateRelativeBox_();

        /** Updates the absolute bounding box of the projectile */
        void updateAbsoluteBox_() override;

    public:
        /**
         * @brief Constructor to create a projectile with specified position, dimensions, velocity, spin, color, and lifetime.
         * @param x X-coordinate of the projectile's origin
         * @param y Y-coordinate of the projectile's origin
         * @param angle Orientation of the projectile (in degrees)
         * @param width Width of the projectile
         * @param height Height of the projectile
         * @param r Red component of the projectile's color
         * @param g Green component of the projectile's color
         * @param b Blue component of the projectile's color
         * @param drawContour Whether a contour should be drawn for the projectile
         * @param vx X component of the velocity vector
         * @param vy Y component of the velocity vector
         * @param spin Angular velocity of the projectile
         * @param lifetime Duration the projectile remains active (in seconds)
         */
        Projectile(float x, float y, float angle, float width, float height,
            float r, float g, float b, bool drawContour,
            float vx = 0.f, float vy = 0.f, float spin = 0.f, float lifetime = 1.5f);

        /**
         * @brief Alternative constructor with color index.
         * @param x X-coordinate of the projectile's origin
         * @param y Y-coordinate of the projectile's origin
         * @param angle Orientation of the projectile (in degrees)
         * @param width Width of the projectile
         * @param height Height of the projectile
         * @param fillColor Color index for the projectile's color
         * @param drawContour Whether a contour should be drawn for the projectile
         * @param vx X component of the velocity vector
         * @param vy Y component of the velocity vector
         * @param spin Angular velocity of the projectile
         * @param lifetime Duration the projectile remains active (in seconds)
         */
        Projectile(float x, float y, float angle, float width, float height,
            ColorIndex fillColor, bool drawContour,
            float vx = 0.f, float vy = 0.f, float spin = 0.f, float lifetime = 1.5f);

        /** Destructor */
        ~Projectile();

        /**
         * @brief Gets the unique creation index of the projectile.
         * @return Unique index of the projectile
         */
        unsigned int getIndex() const override;

        /**
         * @brief Retrieves the object type, identifying it as a projectile.
         * @return ObjectType::Projectile
         */
        ObjectType getObjectType() const override { return ObjectType::Projectile; }

        /**
         * @brief Checks if a point is within the bounds of the projectile.
         * @param x X-coordinate of the point
         * @param y Y-coordinate of the point
         * @return True if the point lies within the projectile's bounds, otherwise false
         */
        bool isInside(float x, float y) const override;

        /**
         * @brief Returns the total count of Projectile objects created.
         * @return Total count of created Projectile objects
         */
        static unsigned int getCount();

        /**
         * @brief Returns the count of active (alive) Projectile objects.
         * @return Count of active Projectile objects
         */
        static unsigned int getLiveCount();

        /**
         * @brief Updates the state of the projectile over a time interval.
         * @param dt Time elapsed since the last update (in seconds)
         * @return UpdateStatus indicating the projectile's current status
         */
        UpdateStatus update(float dt) override;

        /**
         * @brief Sets the global object list used for collision detection.
         * @param objListPtr Pointer to the global object list
         */
        static void setObjectList(const std::list<std::shared_ptr<GraphicObject2D>>* objListPtr);

        /**
         * @brief Creates a new projectile with given properties and adds it to the object list.
         * @param x X-coordinate of the new projectile
         * @param y Y-coordinate of the new projectile
         * @param angle Orientation angle of the new projectile
         * @param vx X component of the initial velocity vector
         * @param vy Y component of the initial velocity vector
         * @param lifetime Duration the projectile will remain active
         */
        static void createProjectile(float x, float y, float angle, float vx, float vy, float lifetime);
    };
}

#endif // PROJECTILE_H
