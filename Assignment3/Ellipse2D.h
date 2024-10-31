//
//  Ellipse2D.hpp
//
//  Created by Jean-Yves Hervé on 2023-09-14.  Revised 2024-10-03
//

#ifndef ELLIPSE2D_H
#define ELLIPSE2D_H

#include "GraphicObject2D.h"

namespace earshooter
{
	class Ellipse2D : public GraphicObject2D
	{
		friend bool initEllipseFunc();
		friend void drawDisk();
		friend void drawArc(float startFrac, float endFrac);
		
		private:
		
			float radiusX_, radiusY_;
				
			/** creation index of the ellipse
			 */
			unsigned int index_;
			
			static const int numCirclePts_;
			static float** circlePts_;

			/**	Counter of the number of Ellipse2D objects created
			 */
			static unsigned int count_;

			/**	Counter of the number of Ellipse2D objects still "alive"
			 */
			static unsigned int liveCount_;

			/**	Private rendering function for this class.  Translation and rotation
			 *	have already been applied by the root class, so this function only applies
			 *	scaling prior to rendering.
			 */
			void draw_() const override;

			/** Update the object's absolute bounding box
			 */
			void updateAbsoluteBox_() override;


		public:
		
			/**	Creates a Ellipse2D object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM x	x coordinates of the ellipse
			 * @PARAM y	y coordinates of the ellipse
			 * @PARAM angle	orientation of the ellipse (in degree)
			 * @PARAM radiusX x radius of the ellipse
			 * @PARAM radiusY y radius of the ellipse
			 * @PARAM red red component [0,1] of the ellipse's color
			 * @PARAM green green component [0,1] of the ellipse's color
			 * @PARAM blue blue component [0,1] of the ellipse's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Ellipse2D(float x, float y, float angle, float radiusX, float radiusY,
						float r, float g, float b, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Ellipse2D object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM pt	coordinates of the ellipse
			 * @PARAM angle	orientation of the ellipse (in degree)
			 * @PARAM radiusX x radius of the ellipse
			 * @PARAM radiusY y radius of the ellipse
			 * @PARAM red red component [0,1] of the ellipse's color
			 * @PARAM green green component [0,1] of the ellipse's color
			 * @PARAM blue blue component [0,1] of the ellipse's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Ellipse2D(const WorldPoint& pt, float angle, float radiusX, float radiusY,
						float r, float g, float b, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Ellipse2D object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM x	x coordinates of the ellipse
			 * @PARAM y	y coordinates of the ellipse
			 * @PARAM angle	orientation of the ellipse (in degree)
			 * @PARAM radiusX x radius of the ellipse
			 * @PARAM radiusY y radius of the ellipse
			 * @PARAM fillColor the ellipse's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Ellipse2D(float x, float y, float angle, float radiusX, float radiusY,
						ColorIndex fillColor, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a Ellipse2D object with the specified position, dimensions,
			 * velocity, spin, and color
			 * @PARAM pt	coordinates of the ellipse
			 * @PARAM angle	orientation of the ellipse (in degree)
			 * @PARAM radiusX x radius of the ellipse
			 * @PARAM radiusY y radius of the ellipse
			 * @PARAM fillColor the ellipse's color
			 * @PARAM drawContour should a contour be drawn for the object
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			Ellipse2D(const WorldPoint& pt, float angle, float radiusX, float radiusY,
						ColorIndex fillColor, bool drawContour,
						float vx=0.f, float vy=0.f, float spin=0.f);

			/** Destructor
			 */
			~Ellipse2D();
			
			/** Checks if the point whose coordinates are passed is inside the
		     *	object
		     *	@PARAM x	x coordinate of the point to test
		     *	@PARAM y	y coordinate of the point to test
			 *	@RETURN true if the point lies inside this object
			 */
			bool isInside(float x, float y) const override;

			/** Returns this ellipse's unique Ellipse2D creation index
			 *	@RETURN this ellipse's unique SmilingFace creation index
			 */
			unsigned int getIndex() const override;
		
			/**	Returns the number of Ellipse2D objects created
			 *	@return	the number of Ellipse2D objects created
			 */
			static unsigned int getCount();

			/**	Returns the number of Ellipse2D objects still "alive"
			 *	@return	the number of Ellipse2D objects still "alive"
			 */
			static unsigned int getLiveCount();

			//disabled constructors & operators
			Ellipse2D() = delete;
			Ellipse2D(const Ellipse2D& obj) = delete;	// copy
			Ellipse2D(Ellipse2D&& obj) = delete;		// move
			Ellipse2D& operator = (const Ellipse2D& obj) = delete;	// copy operator
			Ellipse2D& operator = (Ellipse2D&& obj) = delete;		// move operator
	};

	/** Free function that initializes the coordinates of the points forming the
	 *	contour of a circle.
	 */
	bool initEllipseFunc();
	
	/** Free function that draws a colored disk
     */
	void drawDisk();
	
	/** Free function that draws an arc of a circle from a start fraction to
	 *	an end fraction (0 --> angle 0, 1.0 --> angle 360).  This is a very crude,
	 *	quick and dirty function that only works for fractions in the range [0, 1]
	 *	@PARAM startFrac start fraction in range [0, 1]
	 *	@PARAM endFrac end fraction in range [startFrac, 1]
	 */
	void drawArc(float startFrac, float endFrac);

}

#endif // ELLIPSE2D_H
