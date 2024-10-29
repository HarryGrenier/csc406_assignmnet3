//
//  common.h
//  Week 08 - Earshooter
//
//  Created by Jean-Yves Hervé on 11/9/22.
//

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <string>

namespace earshooter {

    /**	A set of predefined color labels
	 */
	enum class ColorIndex
							{
								WHITE = 0,
								RED,
								GREEN,
								BLUE,
								YELLOW,
								ORANGE,
								PURPLE,
								BROWN,
								PINK,
								GREY,
								NO_COLOR,
								//
								NB_COLORS
							};


    /**	The RGBA values corresponding to the above labels
	 */
    static	float	COLOR[static_cast<int>(ColorIndex::NB_COLORS)][4] =
					{	{1.f, 1.f, 1.f, 1.f},	//	white
						{1.f, 0.f, 0.f, 1.f},	//	red
						{0.f, 1.f, 0.f, 1.f},	//	green
						{0.f, 0.f, 1.f, 1.f},	//	blue
						{1.f, 1.f, 0.f, 1.f},	//	yellow
						{1.f, 0.6f, 0.2f, 1.f},  // orange
						{0.4f, 0.f, 0.7f, 1.f},  // purple
						{0.2f, 0.1f, 0.f, 1.f},  // brown
						{1.f, 0.f, 0.5f, 1.f},   // pink
						{0.5f, 0.5f,0.5f, 1.f}, //  grey
						{0.f, 0.f, 0.f, 0.f},    // no color
					};

//===============================================================
//	Disabled interface constants
//	I would use these if I wanted to choose colors or sizes
//	through the GUI
//===============================================================

//    static const std::string colorMenuStr[static_cast<int>(ColorIndex::NB_COLORS)] =
//		{	"White",
//			"Red",
//			"Green",
//			"Blue",
//			"Yellow",
//			"Orange",
//			"Purple",
//			"Brown",
//			"Pink",
//			"Grey",
//			"No Color",
//		};
//
//    enum class SizeIndex
//    {
//        TINY = 0,
//        VERY_SMALL,
//        SMALL,
//        MEDIUM,
//        LARGE,
//        VERY_LARGE,
//        REALLY_HUGE,	//	For wahtever reason, HUGE is not a valid identifier
//        //
//        NB_SIZES
//    };

//    static	float	SCALE[static_cast<int>(SizeIndex::NB_SIZES)] =
//		{	0.05f,  //  TINY
//			0.10f,  //  VERY_SMALL
//			0.15f,  //  SMALL
//			0.20f,  //  MEDIUM
//			0.25f,  //  LARGE
//			0.30f,  //  VERY_LARGE
//			0.35f	//  REALLY_HUGE
//		};

//    static const std::string segSizeMenuStr[static_cast<int>(SizeIndex::NB_SIZES)] =
//		{	"Tiny",
//			"Very small",
//			"Small",
//			"Medium",
//			"Large",
//			"Very large",
//			"Huge"
//		};


	/** Rendering mode for an object, indicating which type of bounding box
	 * to display, if any
	 */
	enum class BoundingBoxMode
	{
		NO_BOX = 0,
		RELATIVE_BOX,
		ABSOLUTE_BOX
	};
	
	
	/** Use to enumarate all rendering quadrants in cylindrical and
	 *	spherical worlds
	 */
	enum class Quadrant
	{
		NORTH = 0,
		NORTHWEST,
		WEST,
		SOUTHWEST,
		SOUTH,
		SOUTHEAST,
		EAST,
		NORTHEAST
	};
	
	/** This enum type is used to refine the outcome of the update
	 *	function of graphic objects.  Now the function returns a state
	 *	value that indicates things that may have happened while updating
	 *	the object.  For example some objects may have a limited life time,
	 *	or should expire if they get out of the world's bounds (WINDOW_WORLD
	 *	mode), but the object shouldn't delete itself.  Instead, the state
	 *	value is returned to the application, which can act, or not, on that
	 *	information.
	 */
	 enum class UpdateStatus
	 {
		NORMAL,
		DEAD,
		BOUNCE,		//	we could refine =BOUNCE_W, BOUNCE_E, etc.
		WRAPAROUND	//	same here
	 };
}

#endif //	COMMON_TYPES_H
