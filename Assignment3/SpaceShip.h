//
//  SpaceShip.h
//  Week 05 - Prog 1
//
//  Created by Harry Grenier on 2024-11-3.
//

#ifndef SpaceShip_H
#define SpaceShip_H

#include "GraphicObject2D.h"
#include <chrono>
#include <list>

namespace earshooter
{
	/**
	 * @class SpaceShip
	 * @brief Represents a 2D spaceship object with various properties and methods for handling movement,
	 *        collision, drawing, and projectile firing.
	 */
	class SpaceShip : public GraphicObject2D
	{
	private:
		/** List of objects for collision detection */
		static const std::list<std::shared_ptr<GraphicObject2D>>* objList_;

		/** Radius of the isosceles spaceship */
		float radius_;

		/** Angular velocity in degrees per second */
		float angularVelocity_;

		/** Unique creation index for each SpaceShip instance */
		unsigned int index_;

		/** Array of coordinates representing an isosceles SpaceShip */
		static float xy_[3][2];

		/** Counter of the number of SpaceShip objects created */
		static unsigned int count_;

		/** Counter of the number of SpaceShip objects still "alive" */
		static unsigned int liveCount_;

		/** Private rendering function for this class. Translation and rotation
		 * have already been applied by the base class, so this function only applies
		 * scaling prior to rendering.
		 */
		void draw_() const override;

		/** Health value for the spaceship */
		int health_;

		/** Constants representing parts of the spaceship */
		static const int BODY = 0;
		static const int BOTTOM_WING = 1;
		static const int TOP_WING = 2;
		static const int ENGINE = 3;

		/** Updates the relative bounding box of the spaceship */
		void updateRelativeBox_();

		/** Updates the absolute bounding box of the spaceship */
		void updateAbsoluteBox_() override;

		/** Fire rate in projectiles per second */
		float fireRate_;

		/** Time of the last fired projectile */
		std::chrono::high_resolution_clock::time_point lastFireTime_;

	public:
		/** @return The object type of the spaceship */
		ObjectType getObjectType() const override { return ObjectType::SpaceShip; }

		/** Sets the angular velocity of the spaceship */
		void setAngularVelocity(float angularVelocity) { angularVelocity_ = angularVelocity; }

		/** @return The current angular velocity of the spaceship */
		float getAngularVelocity() const { return angularVelocity_; }

		/** Sets the object list for collision detection */
		static void setObjectList(const std::list<std::shared_ptr<GraphicObject2D>>* objListPtr);

		/** @return True if the spaceship is alive, based on health */
		bool isAlive() const { return health_ > 0; }

		/** @return True if the spaceship can still move, based on health */
		bool cantmove() const { return health_ > 25; }

		/** Fires a projectile from the spaceship */
		void fireProjectile();

		/** Decreases the spaceship's health by a specified amount */
		void decreaseHealth(int amount);

		/** @return The current health of the spaceship */
		int getHealth() const;

		/**
		 * Constructor to create a SpaceShip object with specified parameters.
		 * @param x X-coordinate of the spaceship's origin
		 * @param y Y-coordinate of the spaceship's origin
		 * @param angle Orientation of the spaceship in degrees
		 * @param radius Radius of the spaceship
		 * @param r Red component of the spaceship's color
		 * @param g Green component of the spaceship's color
		 * @param b Blue component of the spaceship's color
		 * @param drawContour Flag to indicate if contour should be drawn
		 * @param vx X component of the velocity vector
		 * @param vy Y component of the velocity vector
		 * @param spin Angular velocity of the spaceship
		 */
		SpaceShip(float x, float y, float angle, float radius,
			float r, float g, float b, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/**
		 * Alternative constructor with a WorldPoint for position.
		 * @param pt Coordinates of the spaceship's origin
		 * @param angle Orientation of the spaceship in degrees
		 * @param radius Radius of the spaceship
		 * @param r Red component of the spaceship's color
		 * @param g Green component of the spaceship's color
		 * @param b Blue component of the spaceship's color
		 * @param drawContour Flag to indicate if contour should be drawn
		 * @param vx X component of the velocity vector
		 * @param vy Y component of the velocity vector
		 * @param spin Angular velocity of the spaceship
		 */
		SpaceShip(const WorldPoint& pt, float angle, float radius,
			float r, float g, float b, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/**
		 * Constructor to create a SpaceShip object with a color index.
		 * @param x X-coordinate of the spaceship's origin
		 * @param y Y-coordinate of the spaceship's origin
		 * @param angle Orientation of the spaceship in degrees
		 * @param radius Radius of the spaceship
		 * @param fillColor Color index for the spaceship's color
		 * @param drawContour Flag to indicate if contour should be drawn
		 * @param vx X component of the velocity vector
		 * @param vy Y component of the velocity vector
		 * @param spin Angular velocity of the spaceship
		 */
		SpaceShip(float x, float y, float angle, float radius,
			ColorIndex fillColor, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/**
		 * Alternative constructor with WorldPoint and color index.
		 * @param pt Coordinates of the spaceship's origin
		 * @param angle Orientation of the spaceship in degrees
		 * @param radius Radius of the spaceship
		 * @param fillColor Color index for the spaceship's color
		 * @param drawContour Flag to indicate if contour should be drawn
		 * @param vx X component of the velocity vector
		 * @param vy Y component of the velocity vector
		 * @param spin Angular velocity of the spaceship
		 */
		SpaceShip(const WorldPoint& pt, float angle, float radius,
			ColorIndex fillColor, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Destructor */
		~SpaceShip();

		/**
		 * Checks if a point is inside the spaceship.
		 * @param x X coordinate of the point to test
		 * @param y Y coordinate of the point to test
		 * @return True if the point lies inside the spaceship
		 */
		bool isInside(float x, float y) const override;

		/**
		 * @return The radius of the spaceship
		 */
		inline float getRadius() const { return radius_; }

		/**
		 * Updates the state of the spaceship.
		 * @param dt Time elapsed since the last update call
		 * @return Status value indicating the result of the update
		 */
		UpdateStatus update(float dt) override;

		/**
		 * @return The unique index of this spaceship
		 */
		unsigned int getIndex() const override;

		/**
		 * @return The total number of SpaceShip objects created
		 */
		static unsigned int getCount();

		/**
		 * @return The number of SpaceShip objects still "alive"
		 */
		static unsigned int getLiveCount();

		// Deleted constructors and operators
		SpaceShip() = delete;
		SpaceShip(const SpaceShip&) = delete;
		SpaceShip(SpaceShip&&) = delete;
		SpaceShip& operator =(const SpaceShip&) = delete;
		SpaceShip& operator =(SpaceShip&&) = delete;
	};
}

#endif // SpaceShip_H
