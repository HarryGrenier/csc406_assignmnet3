//
//  Rectangle2D.h
//  Template Project
//
//  Created by Jean-Yves Hervé on 2023-10-12.  Revised 2024-10-03
//

#ifndef Rectangle2D_H
#define Rectangle2D_H

#include "GraphicObject2D.h"

namespace earshooter
{
	/**
	 * @class Rectangle2D
	 * @brief Represents a 2D rectangle object with properties for rendering, collision detection, and scaling.
	 */
	class Rectangle2D : public GraphicObject2D
	{
	private:
		/** Width of the rectangle */
		float width_;

		/** Height of the rectangle */
		float height_;

		/** Unique creation index of the rectangle */
		unsigned int index_;

		/** Counter for the number of Rectangle2D objects created */
		static unsigned int count_;

		/** Counter for the number of Rectangle2D objects still "alive" */
		static unsigned int liveCount_;

		/** Private rendering function for the Rectangle2D class.
		 * Translation and rotation have already been applied by the root class,
		 * so this function only applies scaling before rendering.
		 */
		void draw_() const override;

		/** Updates the relative bounding box of the rectangle */
		void updateRelativeBox_();

		/** Updates the absolute bounding box of the rectangle */
		void updateAbsoluteBox_() override;

	public:
		/** Creates a Rectangle2D object with the specified position, dimensions, velocity, spin, and color.
		 * @param x X-coordinate of the rectangle
		 * @param y Y-coordinate of the rectangle
		 * @param angle Orientation of the rectangle (in degrees)
		 * @param width Width of the rectangle
		 * @param height Height of the rectangle
		 * @param r Red component of the rectangle's color
		 * @param g Green component of the rectangle's color
		 * @param b Blue component of the rectangle's color
		 * @param drawContour Whether a contour should be drawn for the object
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the object
		 */
		Rectangle2D(float x, float y, float angle, float width, float height,
			float r, float g, float b, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Alternative constructor with WorldPoint for the position.
		 * @param pt Coordinates of the rectangle
		 * @param angle Orientation of the rectangle (in degrees)
		 * @param width Width of the rectangle
		 * @param height Height of the rectangle
		 * @param r Red component of the rectangle's color
		 * @param g Green component of the rectangle's color
		 * @param b Blue component of the rectangle's color
		 * @param drawContour Whether a contour should be drawn for the object
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the object
		 */
		Rectangle2D(const WorldPoint& pt, float angle, float width, float height,
			float r, float g, float b, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Constructor to create a Rectangle2D with a specified color index.
		 * @param x X-coordinate of the rectangle
		 * @param y Y-coordinate of the rectangle
		 * @param angle Orientation of the rectangle (in degrees)
		 * @param width Width of the rectangle
		 * @param height Height of the rectangle
		 * @param fillColor Color index for the rectangle's color
		 * @param drawContour Whether a contour should be drawn for the object
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the object
		 */
		Rectangle2D(float x, float y, float angle, float width, float height,
			ColorIndex fillColor, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Alternative constructor with WorldPoint and color index.
		 * @param pt Coordinates of the rectangle
		 * @param angle Orientation of the rectangle (in degrees)
		 * @param width Width of the rectangle
		 * @param height Height of the rectangle
		 * @param fillColor Color index for the rectangle's color
		 * @param drawContour Whether a contour should be drawn for the object
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the object
		 */
		Rectangle2D(const WorldPoint& pt, float angle, float width, float height,
			ColorIndex fillColor, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Destructor */
		~Rectangle2D();

		/** Returns this rectangle's unique creation index.
		 * @return Unique creation index of the rectangle
		 */
		unsigned int getIndex() const override;

		/** Determines if a point is inside the Rectangle2D object.
		 * @param x X-coordinate of the point to test
		 * @param y Y-coordinate of the point to test
		 * @return True if the point is inside (collides with) this object
		 */
		bool isInside(float x, float y) const override;

		/** Returns the total number of Rectangle2D objects created.
		 * @return Total count of Rectangle2D objects created
		 */
		static unsigned int getCount();

		/** Returns the number of Rectangle2D objects still "alive".
		 * @return Count of Rectangle2D objects still "alive"
		 */
		static unsigned int getLiveCount();

		// Deleted constructors & operators
		Rectangle2D() = delete;
		Rectangle2D(const Rectangle2D& obj) = delete;    // copy
		Rectangle2D(Rectangle2D&& obj) = delete;         // move
		Rectangle2D& operator = (const Rectangle2D& obj) = delete;    // copy operator
		Rectangle2D& operator = (Rectangle2D&& obj) = delete;         // move operator
	};
}

#endif // Rectangle2D_H
