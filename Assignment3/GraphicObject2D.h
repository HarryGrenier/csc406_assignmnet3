//
//  GraphicObject2D.h
//  Week 03 - Prog 2
//
//  Created by Jean-Yves Hervé on 2024-09-19.
//

#ifndef GRAPHIC_OBJECT_2D_H
#define GRAPHIC_OBJECT_2D_H

#include <memory>
#include <stdio.h>
#include "World2D.h"
#include "BoundingBox.h"
#include <vector>

namespace earshooter
{
	/*
	emum for instance of object type*/
	enum class ObjectType {
		Generic,
		SpaceShip,
		Projectile
	};

	class GraphicObject2D
	{
	protected:
		/*
		Vectors to store all the differnt bounding box instances if needed for the object
		*/
		std::vector<std::unique_ptr<BoundingBox>> partRelativeBox_;
		std::vector<std::unique_ptr<BoundingBox> > partAbsoluteBox_;
	private:
		float cx_, cy_, angle_;
		float vx_, vy_, spin_;
		float r_, g_, b_;
		bool drawContour_;
		std::unique_ptr<BoundingBox> relativeBox_;
		std::unique_ptr<BoundingBox> absoluteBox_;
		bool dead = false;
		/** creation index of the object
		 */
		unsigned int index_;

		/**	Counter of the number of GraphicObject2D objects created
		 */
		static unsigned int count_;

		/**	Counter of the number of GraphicObject2D objects still "alive"
		 */
		static unsigned int liveCount_;

		/**	Pure virtual (abstract) function.   Translation and rotation
		 *	have already been applied by the root class, so the implementation of this
		 *	function in the child class should only apply scaling prior to rendering.
		 */
		virtual void draw_() const = 0;

		/** Update the object's absolute bounding box
		 */
		virtual void updateAbsoluteBox_()
		{
		}

		/** Update the object's absolute bounding box
		 */
		virtual void updateRelativeBox_()
		{
		}

	public:


		/*set defult type of object to Generic*/
		virtual ObjectType getObjectType() const { return ObjectType::Generic; }


		/**	Creates a GraphicObject2D object with the specified position,
		 * velocity, spin, and color
		 * @PARAM x	x coordinates of the SmilingFace
		 * @PARAM y	y coordinates of the SmilingFace
		 * @PARAM angle	orientation of the SmilingFace (in degree)
		 * @PARAM red red component [0,1] of the face's color
		 * @PARAM green green component [0,1] of the face's color
		 * @PARAM blue blue component [0,1] of the face's color
		 * @PARAM drawContour should a contour be drawn for the object
		 * @PARAM vx x component of the velocity vector
		 * @PARAM vy y component of the velocity vector
		 * @PARAM spin angular velocity of the object
		 */
		GraphicObject2D(float x, float y, float angle,
			float r, float g, float b, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/**	Creates a GraphicObject2D object with the specified position,
		 * velocity, spin, and color
		 * @PARAM pt	coordinates of the SmilingFace
		 * @PARAM angle	orientation of the SmilingFace (in degree)
		 * @PARAM red red component [0,1] of the face's color
		 * @PARAM green green component [0,1] of the face's color
		 * @PARAM blue blue component [0,1] of the face's color
		 * @PARAM drawContour should a contour be drawn for the object
		 * @PARAM vx x component of the velocity vector
		 * @PARAM vy y component of the velocity vector
		 * @PARAM spin angular velocity of the object
		 */
		GraphicObject2D(const WorldPoint& pt, float angle,
			float r, float g, float b, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/**	Creates a GraphicObject2D object with the specified position,
		 * velocity, spin, and color
		 * @PARAM x	x coordinates of the SmilingFace
		 * @PARAM y	y coordinates of the SmilingFace
		 * @PARAM angle	orientation of the SmilingFace (in degree)
		 * @PARAM fillColor the face's color
		 * @PARAM drawContour should a contour be drawn for the object
		 * @PARAM vx x component of the velocity vector
		 * @PARAM vy y component of the velocity vector
		 * @PARAM spin angular velocity of the object
		 */
		GraphicObject2D(float x, float y, float angle,
			ColorIndex fillColor, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/**	Creates a GraphicObject2D object with the specified position,
		 * velocity, spin, and color
		 * @PARAM pt	coordinates of the SmilingFace
		 * @PARAM angle	orientation of the SmilingFace (in degree)
		 * @PARAM fillColor the face's color
		 * @PARAM drawContour should a contour be drawn for the object
		 * @PARAM vx x component of the velocity vector
		 * @PARAM vy y component of the velocity vector
		 * @PARAM spin angular velocity of the object
		 */
		GraphicObject2D(const WorldPoint& pt, float angle,
			ColorIndex fillColor, bool drawContour,
			float vx = 0.f, float vy = 0.f, float spin = 0.f);

		virtual ~GraphicObject2D();

		/** Returns this object's x position as a GraphicObject2D
		 *	@RETURN this object's x position
		 */
		inline float getX() const
		{
			return cx_;
		}
		/** Returns this object's y position as a GraphicObject2D
		 *	@RETURN this object's y position
		 */
		inline float getY() const
		{
			return cy_;
		}


		/**
		 * Gets the current horizontal velocity of the object.
		 * @return The x-component of the object's velocity.
		*/
		inline float getVX() const { return vx_; }

		/**
		 * Gets the current vertical velocity of the object.
		 * @return The y-component of the object's velocity.
		 */
		inline float getVY() const { return vy_; }

		/**
		 * Sets the position of the object.
		 * @param x The new x-coordinate for the object.
		 * @param y The new y-coordinate for the object.
		 */
		void setPosition(float x, float y);

		/**
		 * Sets the position of the object.
		 * @param pt A WorldPoint object representing the new position for the object.
		 */
		void setPosition(const WorldPoint& pt);

		/**
		 * Sets the angle of the object.
		 * @param angle The new angle in degrees, where 0 represents the default orientation.
		 */
		void setAngle(float angle);

		/**
		 * Gets the current angle of the object.
		 * @return The object's angle in degrees.
		 */
		inline float getAngle() const
		{
			return angle_;
		}

		/** Returns this object's creation index as a GraphicObject2D
		 *	@RETURN this object's creation index
		 */
		inline unsigned int getBaseIndex() const
		{
			return index_;
		}

		/** Returns this object's creation index in its final class
		 *	@RETURN this object's creation index in its final class
		 */
		virtual unsigned int getIndex() const = 0;

		static unsigned int getBaseCount();
		static unsigned int getBaseLiveCount();

		inline float getR() const
		{
			return r_;
		}
		inline float getG() const
		{
			return g_;
		}
		inline float getB() const
		{
			return b_;
		}
		void setColor(float r, float g, float b);

		//	now a regular virtual function
		virtual void draw() const;

		/** Updates the position and orientation of the object.  If the subclass
		 * has more stuff to update, it can override this function.
		 *	@PARAM dt	time (in s) elapsed since the last call of this function
		 *	@RETURN		status value indicating what happened during update
		 */
		virtual  UpdateStatus update(float dt);

		/** Checks if the point whose coordinates are passed is inside the
		 *	object
		 *	@PARAM x	x coordinate of the point to test
		 *	@PARAM y	y coordinate of the point to test
		 *	@RETURN true if the point lies inside this object
		 */
		virtual bool isInside(float x, float y) const = 0;

		/** Checks if the point whose coordinates are passed is inside the
		 *	object
		 *	@PARAM pt 	the point to test
		 *	@RETURN true if the point lies inside this object
		 */
		inline bool isInside(const WorldPoint& pt) const
		{
			return isInside(pt.x, pt.y);
		}

		void setDead(bool isDead);


		/**	Reports whether this object is set to draw its contour
		 *	@RETURN 	true if the object draws its contour
		 */
		inline bool getDrawContour() const
		{
			return drawContour_;
		}
		/**	Sets whether the object should draw its contour (or not)
		 *	@PARAM drawContour	true if the object should draw its contour
		 */
		void setDrawContour(bool drawContour);

		/** Sets the corners of the absolute bounding box
		 * @PARAM xmin 	min x coordinates of the box
		 * @PARAM xmax 	max x coordinates of the box
		 * @PARAM ymin 	min y coordinates of the box
		 * @PARAM ymax 	max y coordinates of the box
		 */
		void setAbsoluteBoundingBox(float xmin, float xmax, float ymin, float ymax);

		/** Sets the corners of the absolute bounding box
		 * @PARAM ul 	the box's upper-left corner
		 * @PARAM lr 	the box's lower-right corner
		 */
		void setAbsoluteBoundingBox(const WorldPoint& ul, const WorldPoint& lr);

		/** Sets the corners of the relative bounding box
		 * @PARAM xmin 	min x coordinates of the box
		 * @PARAM xmax 	max x coordinates of the box
		 * @PARAM ymin 	min y coordinates of the box
		 * @PARAM ymax 	max y coordinates of the box
		 */
		void setRelativeBoundingBox(float xmin, float xmax, float ymin, float ymax);

		/** Sets the corners of the relative bounding box
		 * @PARAM ul 	the box's upper-left corner
		 * @PARAM lr 	the box's lower-right corner
		 */
		void setRelativeBoundingBox(const WorldPoint& ul, const WorldPoint& lr);

		/**	Returns a reference to the object's absolute bounding box.  If
		 *	the box hasn't been defined, then the EMPTY_BOX is returned.
		 * @RETURN  a reference to the object's absolute bounding box
		 */
		const BoundingBox& getAbsoluteBoundingBox() const;

		/**	Returns a reference to the object's relative bounding box.  If
		 *	the box hasn't been defined, then the EMPTY_BOX is returned.
		 * @RETURN  a reference to the object's absolute bounding box
		 */
		const BoundingBox& getRelativeBoundingBox() const;

		//	Deleted constructors and operators
		GraphicObject2D() = delete;
		GraphicObject2D(const GraphicObject2D&) = delete;
		GraphicObject2D(GraphicObject2D&&) = delete;
		GraphicObject2D& operator =(const GraphicObject2D&) = delete;
		GraphicObject2D& operator =(GraphicObject2D&&) = delete;

	};
}

#endif // GRAPHIC_OBJECT_2D_H