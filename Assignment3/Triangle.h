//
//  Triangle.h
//  Week 05 - Prog 1
//
//  Created by Jean-Yves Hervé on 2024-09-10.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "GraphicObject2D.h"

namespace earshooter
{
	/**
	 * @class Triangle
	 * @brief Represents a 2D isosceles triangle object with properties for rendering, collision, and scaling.
	 */
	class Triangle : public GraphicObject2D
	{
	private:
		/** Radius of the isosceles triangle */
		float radius_;

		/** Unique creation index of the triangle */
		unsigned int index_;

		/** Array of coordinates for the vertices of an isosceles triangle */
		static float xy_[3][2];

		/** Counter for the number of Triangle objects created */
		static unsigned int count_;

		/** Counter for the number of Triangle objects still "alive" */
		static unsigned int liveCount_;

		/** Private rendering function for the Triangle class.
		 * Translation and rotation have already been applied by the root class,
		 * so this function only applies scaling before rendering.
		 */
		void draw_() const override;

		/** Updates the relative bounding box of the triangle */
		void updateRelativeBox_();

		/** Updates the absolute bounding box of the triangle */
		void updateAbsoluteBox_() override;

	public:
		/** Creates a Triangle object with the specified position, dimensions, velocity, spin, and color.
		 * @param x X-coordinate of the triangle's origin
		 * @param y Y-coordinate of the triangle's origin
		 * @param angle Orientation of the triangle (in degrees)
		 * @param radius Radius of the triangle
		 * @param r Red component of the triangle's color
		 * @param g Green component of the triangle's color
		 * @param b Blue component of the triangle's color
		 * @param drawContour Whether a contour should be drawn for the object
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the object
		 */
		Triangle(float x, float y, float angle, float radius,
			float r, float g, float b, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Alternative constructor with WorldPoint for the position.
		 * @param pt Coordinates of the triangle's origin
		 * @param angle Orientation of the triangle (in degrees)
		 * @param radius Radius of the triangle
		 * @param r Red component of the triangle's color
		 * @param g Green component of the triangle's color
		 * @param b Blue component of the triangle's color
		 * @param drawContour Whether a contour should be drawn for the object
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the object
		 */
		Triangle(const WorldPoint& pt, float angle, float radius,
			float r, float g, float b, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Constructor to create a Triangle with a specified color index.
		 * @param x X-coordinate of the triangle's origin
		 * @param y Y-coordinate of the triangle's origin
		 * @param angle Orientation of the triangle (in degrees)
		 * @param radius Radius of the triangle
		 * @param fillColor Color index for the triangle's color
		 * @param drawContour Whether a contour should be drawn for the object
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the object
		 */
		Triangle(float x, float y, float angle, float radius,
			ColorIndex fillColor, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Alternative constructor with WorldPoint and color index.
		 * @param pt Coordinates of the triangle's origin
		 * @param angle Orientation of the triangle (in degrees)
		 * @param radius Radius of the triangle
		 * @param fillColor Color index for the triangle's color
		 * @param drawContour Whether a contour should be drawn for the object
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the object
		 */
		Triangle(const WorldPoint& pt, float angle, float radius,
			ColorIndex fillColor, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Destructor */
		~Triangle();

		/** Determines if a point is inside the Triangle object.
		 * @param x X-coordinate of the point to test
		 * @param y Y-coordinate of the point to test
		 * @return True if the point is inside (collides with) this object
		 */
		bool isInside(float x, float y) const override;

		/** Returns the radius of this isosceles triangle.
		 * @return Radius of the triangle
		 */
		inline float getRadius() const { return radius_; }

		/** Updates the state of the Triangle object.
		 * This function shows that a subclass may have more to update than just the position and orientation.
		 * @param dt Time (in seconds) elapsed since the last call of this function
		 * @return Status value indicating what happened during the update
		 */
		UpdateStatus update(float dt) override;

		/** Returns this triangle's unique creation index.
		 * @return Unique creation index of the triangle
		 */
		unsigned int getIndex() const override;

		/** Returns the total number of Triangle objects created.
		 * @return Total count of Triangle objects created
		 */
		static unsigned int getCount();

		/** Returns the number of Triangle objects still "alive".
		 * @return Count of Triangle objects still "alive"
		 */
		static unsigned int getLiveCount();

		// Deleted constructors and operators
		Triangle() = delete;
		Triangle(const Triangle&) = delete;
		Triangle(Triangle&&) = delete;
		Triangle& operator =(const Triangle&) = delete;
		Triangle& operator =(Triangle&&) = delete;
	};
}

#endif // TRIANGLE_H
