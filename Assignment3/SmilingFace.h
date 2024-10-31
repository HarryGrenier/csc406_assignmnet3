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
	class SmilingFace : public GraphicObject2D
	{
		private:
		
			/** scaling size of the face
			 */
			float size_;
			
			/** creation index of the face
			 */
			unsigned int index_;
			
			/**	Counter of the number of SmilingFace objects created
			 */
			static unsigned int count_;

			/**	Counter of the number of SmilingFace objects still "alive"
			 */
			static unsigned int liveCount_;

			const static int LEFT_EAR;
			const static int RIGHT_EAR;
			const static int FACE;
			const static int HEAD;
			
			const static int HEAD_NUM_PARTS;
						
			const static float FACE_RADIUS;
			const static float EAR_RADIUS;
			const static float EYE_OUTER_RADIUS;
			const static float EYE_INNER_RADIUS;
			const static float MOUTH_V_DIAMETER;
			const static float MOUTH_H_DIAMETER;
			//
			const static float EAR_V_OFFSET;
			const static float EAR_H_OFFSET;
			const static float EYE_V_OFFSET;
			const static float EYE_H_OFFSET;
			const static float LEFT_EAR_X;
			const static float LEFT_EAR_Y;
			const static float RIGHT_EAR_X;
			const static float RIGHT_EAR_Y;
			//
			const static float LEFT_EYE_X;
			const static float LEFT_EYE_Y;
			const static float RIGHT_EYE_X;
			const static float RIGHT_EYE_Y;

			const static float MOUTH_H_OFFSET;
			const static float MOUTH_V_OFFSET;

			const static ColorIndex PART_BOX_COLOR[];
			
			/**	Relative bounding boxes of the object's parts
			 */
			std::vector<std::unique_ptr<BoundingBox> > partRelativeBox_;

			/**	Relative bounding boxes of the object's parts
			 */
			std::vector<std::unique_ptr<BoundingBox> > partAbsoluteBox_;

			/**	Initializes the relative boxes' dimensions
			 */
			void initializeRelativeBox_() const;

			/**	Updates the relative boxes' dimensions
			 */
			void updateAbsoluteBox_() const;

			/**	Private rendering function for this class.  Translation and rotation
			 *	have already been applied by the root class, so this function only applies
			 *	scaling prior to rendering.
			 */
			void draw_() const override;

			/** Update the object's absolute bounding box
			 */
			void updateAbsoluteBox_() override;


		public:

			/**	Creates a smiling SmilingFace object with the specified position,
			 * velocity, scale, and color
			 * @PARAM x	x coordinates of the SmilingFace
			 * @PARAM y	y coordinates of the SmilingFace
			 * @PARAM angle	orientation of the SmilingFace (in degree)
			 * @PARAM vel	velocity of the SmilingFace
			 * @PARAM spin	angular velocity of the SmilingFace (in degree/s)
			 * @PARAM scale	scale factor to apply to the SmilingFace, for rendering and collision detection
			 * @PARAM red red component [0,1] of the face's color
			 * @PARAM green green component [0,1] of the face's color
			 * @PARAM blue blue component [0,1] of the face's color
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			SmilingFace(float x, float y, float angle, float scale,
						float r, float g, float b, float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a smiling SmilingFace object with the specified position,
			 * velocity, scale, and color
			 * @PARAM pt	coordinates of the SmilingFace
			 * @PARAM angle	orientation of the SmilingFace (in degree)
			 * @PARAM vel	velocity of the SmilingFace
			 * @PARAM spin	angular velocity of the SmilingFace (in degree/s)
			 * @PARAM scale	scale factor to apply to the SmilingFace, for rendering and collision detection
			 * @PARAM red red component [0,1] of the face's color
			 * @PARAM green green component [0,1] of the face's color
			 * @PARAM blue blue component [0,1] of the face's color
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			SmilingFace(const WorldPoint& pt, float angle, float scale,
						float r, float g, float b, float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a smiling SmilingFace object with the specified position,
			 * velocity, scale, and color
			 * @PARAM x	x coordinates of the SmilingFace
			 * @PARAM y	y coordinates of the SmilingFace
			 * @PARAM angle	orientation of the SmilingFace (in degree)
			 * @PARAM scale	scale factor to apply to the SmilingFace, for rendering and collision detection
			 * @PARAM fillColor the face's color
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			SmilingFace(float x, float y, float angle, float scale,
						ColorIndex fillColor, float vx=0.f, float vy=0.f, float spin=0.f);
						
			/**	Creates a smiling SmilingFace object with the specified position,
			 * velocity, scale, and color
			 * @PARAM pt	coordinates of the SmilingFace
			 * @PARAM angle	orientation of the SmilingFace (in degree)
			 * @PARAM scale	scale factor to apply to the SmilingFace, for rendering and collision detection
			 * @PARAM fillColor the face's color
			 * @PARAM vx x component of the velocity vector
			 * @PARAM vy y component of the velocity vector
			 * @PARAM spin angular velocity of the object
			 */
			SmilingFace(const WorldPoint& pt, float angle, float scale,
						ColorIndex fillColor, float vx=0.f, float vy=0.f, float spin=0.f);
						

			/**	Destructor
			 */
			~SmilingFace();
		
			/** Determines whether a world point is inside the object
			 *	@PARAM x	the point's horizontal coordinate
			 *	@PARAM y	the point's vertical coordinate
			 *	@RETURN		true if the point is inside (collides with) this object
			 */
			bool isInside(float x, float y) const override;

			inline float getSize() const
			{
				return size_;
			}

			void setSize(float size);

			/** Returns this faces's unique SmilingFace creation index
			 *	@RETURN this face's unique SmilingFace creation index
			 */
			unsigned int getIndex() const override;
		
			/**	Returns the number of SmilingFace objects created
			 *	@return	the number of SmilingFace objects created
			 */
			static unsigned int getCount();

			/**	Returns the number of SmilingFace objects still "alive"
			 *	@return	the number of SmilingFace objects still "alive"
			 */
			static unsigned int getLiveCount();
			
			//	Deleted constructors and operators
			SmilingFace(const SmilingFace& obj) = delete;
			SmilingFace(SmilingFace&& obj) = delete;
			const SmilingFace& operator = (const SmilingFace& obj) = delete;
			SmilingFace& operator = (SmilingFace&& obj) = delete;
			

	};

}

#endif //	SMILING_FACE_H
