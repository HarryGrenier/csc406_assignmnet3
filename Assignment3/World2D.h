#ifndef WORLD_H
#define WORLD_H

#include <cmath>

#define SINGLETON_VERSION	1

namespace earshooter
{
	/** Struct used to store the coordinates of a Point object in pixel
	 *  coordinates.
	 */
	struct PixelPoint
	{
		float ix, iy;
	};
	
	/** Struct used to store the coordinates of a Point object in pixel
	 *  coordinates.
	 */
	struct WorldPoint
	{
		float x, y;
	};
	
	/** Stores a velocity vector so that ot can be addressed either as a struct
	 *	or as an array.
	 */
	union Velocity
	{
		/**	stores the coordinates as a struct of 2 float
		 */
		struct {
					float vx;
					float vy;
		} sxy;
		
		/**	stores the coordinates as an array of 2 float
		 */
		float		vxy[2];
		
		
		const static Velocity NULL_VELOCITY;
		
	};
	

	enum class WorldType {
							WINDOW_WORLD = 0,	// objects disappear at edges
							BOX_WORLD,			// objects bounce at the edge of the World2D
							CYLINDER_WORLD,		//	bounce top-bottom, wrap around left-right,
							SPHERE_WORLD		//	wrap all around
	};


	/** Essentially a set of application-wide global variables defining the
	 * dimensions of the "World2D" and the conversion factors from pixel to World2D
	 * units and back, as well as a few rendering constants.
	 *	A cleaner version would replace all these public static variables by
	 *	private instance variables accessed by setters & getters, so that
	 *	we cann have multipe windows, each running a different World2D
	 */
	struct World2D
	{
		friend bool createUniqueWorld();
		
		
		/**	Minimum x value to get mapped to the display area.
		 *	Set in the main program by a call to setWorld2DBounds.
		 *	@see setWorld2DBounds
		 */
		static float X_MIN;

		/**	Maximum x value to get mapped to the display area.
		 *	Set in the main program by a call to setWorld2DBounds.
		 *	@see setWorld2DBounds
		 */
		static float X_MAX;

		/**	Minimum y value to get mapped to the display area.
		 *	Set in the main program by a call to setWorld2DBounds.
		 *	@see setWorld2DBounds
		 */
		static float Y_MIN;

		/**	Maximum x value to get mapped to the display area.
		 *	Set in the main program by a call to setWorld2DBounds.
		 *	@see setWorld2DBounds
		 */
		static float Y_MAX;

		/**	Calculated as X_MAX-X_MIN in the main program by a call to setWorld2DBounds.
		 *	@see setWorld2DBounds
		 */
		static float WIDTH;

		/**	Calculated as Y_MAX-Y_MIN in the main program by a call to setWorld2DBounds.
		 *	@see setWorld2DBounds
		 */
		static float HEIGHT;

		/**	Scaling factor converting pixel units to World2D units.
		 *	Calculated in the main program by a call to setWorld2DBounds.
		 *	@see setWorld2DBounds
		 */
		static float pixelToWorldRatio;

		/**	Scaling factor converting World2D units to pixel units.
		 *	Calculated in the main program by a call to setWorld2DBounds.
		 *	@see setWorld2DBounds
		 */
		static float worldToPixelRatio;
		
		/**	This one is really equal to pixelToWorldRatio, but it looks confusing
		 *	to write glScalef(pixelToWorldRatio, pixelToWorldRatio, 1.f);
		 *	right before trying to drawe in pixel units.
		 *	@see setWorld2DBounds
		 */
		static float drawInPixelScale;
		
		static WorldType worldType;

		static bool drawReferenceFrames;
		
		/** Function called through the initialization of a global variable in the
		 *	main program.  Although the user specifies dimensions for the rendering pane,
		 *	the function may set different values that agree better with the World2D
		 *	aspect ratio.
		 * @param xmin	Minimum x value to get mapped to the display area.
		 * @param xmax	Maximum x value to get mapped to the display area.
		 * @param ymin	Minimum y value to get mapped to the display area.
		 * @param ymax	Maximum y value to get mapped to the display area.
		 * @param paneWidth		user-set width of the redering pane
		 * @param paneHeight	user-set height of the redering pane
		 * */
		static void setWorld2DBounds(float xmin, float xmax, float ymin, float ymax,
									int& paneWidth, int& paneHeight);


		/**	Returns the conversion factor from pixel to world units
		 */
		inline static float getPixelToWorldScale()
		{
			return pixelToWorldRatio;
		}

		/**	Returns the conversion factor from world to pixel units
		 */
		inline static float getWorldToPixelScale()
		{
			return worldToPixelRatio;
		}
		
		private:
			World2D() = delete;
			~World2D() = default;

	};

	void drawReferenceFrame(void);

	WorldPoint pixelToWorld(float ix, float iy);
	PixelPoint worldToPixel(float wx, float wy);

}

#endif  //  WORLD_H
