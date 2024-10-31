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
	class Rectangle2D : public GraphicObject2D
	{
		private:
		
			float width_, height_;

			/** creation index of the rectangle
			 */
			unsigned int index_;
			
			/**	Counter of the number of Rectangle2D objects created
			 */
			static unsigned int count_;

			/**	Counter of the number of Rectangle2D objects still "alive"
			 */
			static unsigned int liveCount_;

			/**	Private rendering function for this class.  Translation and rotation
			 *	have already been applied by the root class, so this function only applies
			 *	scaling prior to rendering.
			 */
			void draw_() const override;
			void updateRelativeBox_();
			/** Update the object's absolute bounding box
			 */
			void updateAbsoluteBox_() override;


		public:

			/**	Creates a Rectangle2D object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM x	x coordinates of the rectangle
			 * @PARAM y	y coordinates of the rectangle
			 * @PARAM angle	orientation of the rectangle (in degree)
			 * @PARAM width width of the rectangle
			 * @PARAM height height of the rectangle
			 * @PARAM red red component [0,1] of the rectangle's color
			 * @PARAM green green component [0,1] of the rectangle's color
			 * @PARAM blue blue component [0,1] of the rectangle's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Rectangle2D(float x, float y, float angle, float width, float height,
						float r, float g, float b, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Rectangle2D object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM pt	coordinates of the rectangle
			 * @PARAM angle	orientation of the rectangle (in degree)
			 * @PARAM width width of the rectangle
			 * @PARAM height height of the rectangle
			 * @PARAM red red component [0,1] of the rectangle's color
			 * @PARAM green green component [0,1] of the rectangle's color
			 * @PARAM blue blue component [0,1] of the rectangle's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Rectangle2D(const WorldPoint& pt, float angle, float width, float height,
						float r, float g, float b, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Rectangle2D object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM x	x coordinates of the rectangle
			 * @PARAM y	y coordinates of the rectangle
			 * @PARAM angle	orientation of the rectangle (in degree)
			 * @PARAM width width of the rectangle
			 * @PARAM height height of the rectangle
			 * @PARAM fillColor the rectangle's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Rectangle2D(float x, float y, float angle, float width, float height,
						ColorIndex fillColor, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Rectangle2D object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM pt	coordinates of the rectangle
			 * @PARAM angle	orientation of the rectangle (in degree)
			 * @PARAM width width of the rectangle
			 * @PARAM height height of the rectangle
			 * @PARAM fillColor the rectangle's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Rectangle2D(const WorldPoint& pt, float angle, float width, float height,
						ColorIndex fillColor, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);

			~Rectangle2D();
			
			/** Returns this rectangle's unique Rectangle2D creation index
			 *	@RETURN this rectangle's unique Rectangle2D creation index
			 */
			unsigned int getIndex() const override;
		
			/** Checks if the point whose coordinates are passed is inside the
		     *	object
		     *	@PARAM x	x coordinate of the point to test
		     *	@PARAM y	y coordinate of the point to test
			 *	@RETURN true if the point lies inside this object
			 */
			bool isInside(float x, float y) const override;

			/**	Returns the number of Rectangle2D objects created
			 *	@return	the number of Rectangle2D objects created
			 */
			static unsigned int getCount();

			/**	Returns the number of Rectangle2D objects still "alive"
			 *	@return	the number of Rectangle2D objects still "alive"
			 */
			static unsigned int getLiveCount();

			//disabled constructors & operators
			Rectangle2D() = delete;
			Rectangle2D(const Rectangle2D& obj) = delete;	// copy
			Rectangle2D(Rectangle2D&& obj) = delete;		// move
			Rectangle2D& operator = (const Rectangle2D& obj) = delete;	// copy operator
			Rectangle2D& operator = (Rectangle2D&& obj) = delete;		// move operator

	};
}

#endif // Rectangle2D_H
