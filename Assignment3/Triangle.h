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
	class Triangle : public GraphicObject2D
	{
		private:
		
			/** Radius of the isoceles triangle
			 */
			float radius_;

			/** creation index of the triangle
			 */
			unsigned int index_;
			
			/** array of coordinates of an isoceles triangle
			 */
			static float xy_[3][2];

			/**	Counter of the number of Triangle objects created
			 */
			static unsigned int count_;

			/**	Counter of the number of Triangle objects still "alive"
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

			/**	Creates a Triangle object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM x	x coordinates of the triangle's origin
			 * @PARAM y	y coordinates of the triangle's origin
			 * @PARAM angle	orientation of the triangle (in degree)
			 * @PARAM radius radius of the triangle
			 * @PARAM red red component [0,1] of the triangle's color
			 * @PARAM green green component [0,1] of the triangle's color
			 * @PARAM blue blue component [0,1] of the triangle's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Triangle(float x, float y, float angle, float radius,
						float r, float g, float b, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Triangle object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM pt	coordinates of the triangle's origin
			 * @PARAM angle	orientation of the triangle (in degree)
			 * @PARAM radius radius of the triangle
			 * @PARAM red red component [0,1] of the triangle's color
			 * @PARAM green green component [0,1] of the triangle's color
			 * @PARAM blue blue component [0,1] of the triangle's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Triangle(const WorldPoint& pt, float angle, float radius,
						float r, float g, float b, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Triangle object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM x	x coordinates of the triangle's origin
			 * @PARAM y	y coordinates of the triangle's origin
			 * @PARAM angle	orientation of the triangle (in degree)
			 * @PARAM radius radius of the triangle
			 * @PARAM fillColor the triangle's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Triangle(float x, float y, float angle, float radius,
						ColorIndex fillColor, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Triangle object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM pt	coordinates of the triangle's origin
			 * @PARAM angle	orientation of the triangle (in degree)
			 * @PARAM radius radius of the triangle
			 * @PARAM fillColor the triangle's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Triangle(const WorldPoint& pt, float angle, float radius,
						ColorIndex fillColor, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);

			/** Destructor
			 */
			~Triangle();

			/** Checks if the point whose coordinates are passed is inside the
		     *	object
		     *	@PARAM x	x coordinate of the point to test
		     *	@PARAM y	y coordinate of the point to test
			 *	@RETURN true if the point lies inside this object
			 */
			bool isInside(float x, float y) const override;

			/** Returns this isoceles triangle's radius
			 */
			inline float getRadius() const
			{
				return radius_;
			}
			
			/** Updates the state of the Triangle object.  I put this one in just
			 * to show that a subclass may have more to update than just the position
			 * and orientation
			 *	@PARAM dt	time (in s) elapsed since the last call of this function
			 *	@RETURN		status value indicating what happened during update
			 */
			UpdateStatus update(float dt) override;

			/** Returns this triangle's unique SmilingFace creation index
			 *	@RETURN this triangle's unique SmilingFace creation index
			 */
			unsigned int getIndex() const override;
		
			/**	Returns the number of Triangle2D objects created
			 *	@return	the number of Triangle2D objects created
			 */
			static unsigned int getCount();

			/**	Returns the number of Triangle2D objects still "alive"
			 *	@return	the number of Triangle2D objects still "alive"
			 */
			static unsigned int getLiveCount();
			

			//	Deleted constructors and operators
			Triangle() = delete;
			Triangle(const Triangle&) = delete;
			Triangle(Triangle&&) = delete;
			Triangle& operator =(const Triangle&) = delete;
			Triangle& operator =(Triangle&&) = delete;

	};
}
#endif //	TRIANGLE_H
