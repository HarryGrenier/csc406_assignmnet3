//
//  SmilingFace.hpp
//  Week 08 - Earshooter

#ifndef SMILING_FACE_H
#define SMILING_FACE_H

#include <memory>
#include <vector>
#include "commonTypes.h"
#include "GraphicObject2D.h"
#include "BoundingBox.h"

namespace earshooter
{
	/**
	 * @class SmilingFace
	 * @brief Represents a 2D smiling face object with properties for rendering, collision, and scaling.
	 */
	class SmilingFace : public GraphicObject2D
	{
	private:
		/** Scaling size of the face */
		float size_;

		/** Unique creation index of the face */
		unsigned int index_;

		/** Counter for the number of SmilingFace objects created */
		static unsigned int count_;

		/** Counter for the number of SmilingFace objects still "alive" */
		static unsigned int liveCount_;

		/** Indices for different parts of the face */
		const static int LEFT_EAR;
		const static int RIGHT_EAR;
		const static int FACE;
		const static int HEAD;

		/** Number of parts in the head */
		const static int HEAD_NUM_PARTS;

		/** Constant radii for face components */
		const static float FACE_RADIUS;
		const static float EAR_RADIUS;
		const static float EYE_OUTER_RADIUS;
		const static float EYE_INNER_RADIUS;
		const static float MOUTH_V_DIAMETER;
		const static float MOUTH_H_DIAMETER;

		/** Vertical and horizontal offsets for face components */
		const static float EAR_V_OFFSET;
		const static float EAR_H_OFFSET;
		const static float EYE_V_OFFSET;
		const static float EYE_H_OFFSET;
		const static float LEFT_EAR_X;
		const static float LEFT_EAR_Y;
		const static float RIGHT_EAR_X;
		const static float RIGHT_EAR_Y;
		const static float LEFT_EYE_X;
		const static float LEFT_EYE_Y;
		const static float RIGHT_EYE_X;
		const static float RIGHT_EYE_Y;
		const static float MOUTH_H_OFFSET;
		const static float MOUTH_V_OFFSET;

		/** Array of colors for different parts of the bounding box */
		const static ColorIndex PART_BOX_COLOR[];

		/** Initializes the relative bounding boxes of the object's parts */
		void initializeRelativeBox_() const;

		/** Updates the relative bounding box of the face */
		void updateRelativeBox_() override;

		/** Updates the absolute bounding box of the face */
		void updateAbsoluteBox_() const;

		/** Private rendering function for the SmilingFace class.
		 *  Applies scaling before rendering.
		 */
		void draw_() const override;

		/** Updates the object's absolute bounding box */
		void updateAbsoluteBox_() override;

	public:
		/** Creates a smiling face object with specified parameters
		 * @param x X-coordinate of the SmilingFace
		 * @param y Y-coordinate of the SmilingFace
		 * @param angle Orientation of the SmilingFace (in degrees)
		 * @param scale Scale factor for rendering and collision detection
		 * @param r Red component of the face color
		 * @param g Green component of the face color
		 * @param b Blue component of the face color
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the SmilingFace
		 */
		SmilingFace(float x, float y, float angle, float scale,
			float r, float g, float b, float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Alternative constructor with WorldPoint for position
		 * @param pt Position of the SmilingFace
		 * @param angle Orientation of the SmilingFace (in degrees)
		 * @param scale Scale factor for rendering and collision detection
		 * @param r Red component of the face color
		 * @param g Green component of the face color
		 * @param b Blue component of the face color
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the SmilingFace
		 */
		SmilingFace(const WorldPoint& pt, float angle, float scale,
			float r, float g, float b, float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Constructor to create a SmilingFace with a specified color index
		 * @param x X-coordinate of the SmilingFace
		 * @param y Y-coordinate of the SmilingFace
		 * @param angle Orientation of the SmilingFace (in degrees)
		 * @param scale Scale factor for rendering and collision detection
		 * @param fillColor Color index for the face color
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the SmilingFace
		 */
		SmilingFace(float x, float y, float angle, float scale,
			ColorIndex fillColor, float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Alternative constructor with WorldPoint and color index
		 * @param pt Position of the SmilingFace
		 * @param angle Orientation of the SmilingFace (in degrees)
		 * @param scale Scale factor for rendering and collision detection
		 * @param fillColor Color index for the face color
		 * @param vx X-component of the velocity vector
		 * @param vy Y-component of the velocity vector
		 * @param spin Angular velocity of the SmilingFace
		 */
		SmilingFace(const WorldPoint& pt, float angle, float scale,
			ColorIndex fillColor, float vx = 0.f, float vy = 0.f, float spin = 0.f);

		/** Destructor */
		~SmilingFace();

		/** Determines if a point is inside the SmilingFace object
		 * @param x X-coordinate of the point
		 * @param y Y-coordinate of the point
		 * @return True if the point is inside (collides with) the object
		 */
		bool isInside(float x, float y) const override;

		/** Returns the scaling size of the face
		 * @return The size of the face
		 */
		inline float getSize() const { return size_; }

		/** Sets the size of the SmilingFace
		 * @param size New size for the SmilingFace
		 */
		void setSize(float size);

		/** @return The unique creation index of the face */
		unsigned int getIndex() const override;

		/** Returns the total number of SmilingFace objects created
		 * @return The total count of SmilingFace objects
		 */
		static unsigned int getCount();

		/** Returns the number of SmilingFace objects still "alive"
		 * @return The count of SmilingFace objects still "alive"
		 */
		static unsigned int getLiveCount();

		// Deleted constructors and operators
		SmilingFace(const SmilingFace& obj) = delete;
		SmilingFace(SmilingFace&& obj) = delete;
		const SmilingFace& operator = (const SmilingFace& obj) = delete;
		SmilingFace& operator = (SmilingFace&& obj) = delete;

	};

}

#endif // SMILING_FACE_H
