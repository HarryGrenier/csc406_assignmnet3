
#include <cmath>
#include "glPlatform.h"
#include "World2D.h"

using namespace earshooter;

float World2D::X_MIN;
float World2D::X_MAX;
float World2D::Y_MIN;
float World2D::Y_MAX;
float World2D::WIDTH;
float World2D::HEIGHT;
float World2D::pixelToWorldRatio;
float World2D::worldToPixelRatio;
float World2D::drawInPixelScale;

void World2D::setWorld2DBounds(float xmin, float xmax, float ymin, float ymax,
						   int& paneWidth, int& paneHeight){
	if ((xmax <= xmin) || (ymax <= xmin)){
		exit(5);
	}
	X_MIN = xmin;
	X_MAX = xmax;
	Y_MIN = ymin;
	Y_MAX = ymax;
	WIDTH = xmax - xmin;
	HEIGHT = ymax - ymin;
	
	float widthRatio = WIDTH / paneWidth;
	float heightRatio = HEIGHT / paneHeight;
	float maxRatio = fmax(widthRatio,heightRatio);
//	Removed because this doesn’t work happily with interactive window resizing,
//	// If the two ratios differ by more than 5%,  then reject the dimensions
//	if (fabsf(widthRatio-heightRatio)/maxRatio > 0.05){
//		exit(15);
//	}

	pixelToWorldRatio = maxRatio;
	worldToPixelRatio = 1.f / pixelToWorldRatio;
	drawInPixelScale = pixelToWorldRatio;
	
	paneWidth = static_cast<int>(round(WIDTH * worldToPixelRatio));
	paneHeight = static_cast<int>(round(HEIGHT * worldToPixelRatio));
}

void earshooter::drawReferenceFrame(void)
{
	if (World2D::drawReferenceFrames)
	{
		glLineWidth(2.f);
		glPushMatrix();
		//	switch to pixels
		glScalef(World2D::drawInPixelScale, World2D::drawInPixelScale, 1.f);
		glBegin(GL_LINES);
			//	X --> red.
			glColor3f(1.0f, 0.f, 0.f);
			glVertex2f(-10.f, 0.f);
			glVertex2f(50.f, 0.f);
			//	Y --> green
			glColor3f(0.f, 1.0f, 0.f);
			glVertex2f(0.f, -10.f);
			glVertex2f(0.f, 50.f);
		glEnd();
		//	back to  World2D units
		glPopMatrix();
		glLineWidth(1.f);
	}
}
WorldPoint earshooter::pixelToWorld(float ix, float iy)
{
	return WorldPoint{	World2D::X_MIN + ix*World2D::pixelToWorldRatio,
					World2D::Y_MAX - iy*World2D::pixelToWorldRatio
				};
}

PixelPoint earshooter::worldToPixel(float wx, float wy)
{
	return PixelPoint{(wx - World2D::X_MIN)*World2D::worldToPixelRatio,
				 (World2D::Y_MAX - wy)*World2D::worldToPixelRatio};

}


