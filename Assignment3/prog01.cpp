//
//  prog1.cpp
//
//	Very basic interface with limited keyboard action:
//		- 'q' or ESC exits the app,
//		- ' ' (space) toggles animation on/off
//		- Mouse tracking:
//			* 'm' toggles on/off mouse motion tracking (button pressed)
//			* 'p' toggles on/off passive mouse motion tracking (no button pressed)
//			* 'e' toggles on/off mouse exit/enter
//		- Text display
//			* 't' toggles on/off text overlay display
//			* 'i' activates the input of a second text string
//				(input from the terminal)
//		- World mode selection
//			* 'w' activates "window" mode (objects disappear at the edge)
//			* 'b' activates "box" mode (objects bounce off all edges)
//			* 'c' activates "cylindrical" mode (objects wrap around vertical
//					edges, bounce off the horizontal edges)
//			* 's' activates "spherical" mode (objects wrap around all edges)
//		- Bounding box display
//			* 'a' toggles on/off absolute box drawing.  If relative box was on,
//				then it's turned off when absolute box drawing is activated
//			* 'r' toggles on/off relative box drawing.  If absolute box was on,
//				then it's turned off when relative box drawing is activated
//		- 'f' toggles on/off the drawing of reference frames.
//	Initial aspect ratio of the window is preserved when the window
//	is resized.
//
//  Created by Jean-Yves Hervé on 2024-10-16.
//

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <random>
#include <chrono>
#include <ctime>
//
#include "glPlatform.h"
#include "World2D.h"
#include "Rectangle2D.h"
#include "Ellipse2D.h"
#include "Triangle.h"
#include "SmilingFace.h"
#include "SpaceShip.h"
#include "Projectile.h"

using namespace std;
using namespace earshooter;

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Custom data types
//--------------------------------------
#endif

//	I like to setup my meny item indices as enmerated values, but really
//	regular int constants would do the job just fine.
//	Note that in modern C++, it's generally advised to use "scoped enums", which
//	are declared as "enum class NameOfType".  I agree and do so in general, but
//	here these are meant to be used with my glut interface, and it's really
//	bothersome to do the casting to int each each time.


enum MenuItemID {
	SEPARATOR = -1,
	//
	QUIT_MENU = 0,
	OTHER_MENU_ITEM,
	SOME_ITEM = 10
};

enum TextColorSubmenuItemID {
	FIRST_TEXT = 11,
	RED_TEXT = 11,
	GREEN_TEXT,
	WHITE_TEXT,
	MAGENTA_TEXT,
	//
	MAX_COUNT_TEXT
};
const int NUM_TEXT_COLORS = MAX_COUNT_TEXT - FIRST_TEXT;

enum BackgroundColorSubmenuItemID {
	FIRST_BGND = 21,
	NO_BACKGROUND = 21,
	LIGHT_GREY_BGND,
	DARK_GREY_BGND,
	GREEN_BGND,
	BLUE_BGND,
	BROWN_BGND,
	//
	MAX_COUNT_BGND
};
const int NUM_BGND_COLORS = MAX_COUNT_BGND - FIRST_BGND;

enum FontSize {
	SMALL_FONT_SIZE = 0,
	MEDIUM_FONT_SIZE,
	LARGE_FONT_SIZE,
	//
	NUM_FONT_SIZES
};

using objIter = list<shared_ptr<GraphicObject2D> >::iterator;
using constObjIter = list<shared_ptr<GraphicObject2D> >::const_iterator;


#if 0
//--------------------------------------
#pragma mark -
#pragma mark Function prototypes
//--------------------------------------
#endif

void displayTextualInfo(const string& infoStr, int textRow);
void displayTextualInfo(const char* infoStr, int textRow);
void myDisplayFunc(void);
void myResizeFunc(int w, int h);
void myMouseHandler(int b, int s, int x, int y);
void myMouseMotionHandler(int x, int y);
void myMousePassiveMotionHandler(int x, int y);
void myEntryHandler(int state);
void myKeyHandler(unsigned char c, int x, int y);
void myKeyUpHandler(unsigned char c, int x, int y);
void mySpecialKeyHandler(int key, int x, int y);
void mySpecialKeyUpHandler(int key, int x, int y);
void myMenuHandler(int value);
void mySubmenuHandler(int colorIndex);
void myTimerFunc(int val);
void applicationInit();
//
void drawSquare(float cx, float cy, float size, float r,
	float g, float b, bool contour);

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Constants
//--------------------------------------
#endif

//	Workaround for that pesky M_PI lack on Windows
#ifndef M_PI
#define	M_PI 3.14159265f
#endif

//	This one is really for Macs (32 pixel is traditional height of top menu bar
const int 	INIT_WIN_X = 10,
INIT_WIN_Y = 32;

//	Define world dimensions (cascaded down to the World2D class)
//	Also, some speed limits based on world dimensions.
const float X_MIN = -10.f, X_MAX = +10.f;
const float Y_MIN = -10.f, Y_MAX = +10.f;
//
const float MAX_SPIN = 100.f;	//	degree per second
const float MIN_TIME_TO_CROSS = 5.f;	// shortest time for an object to cross the screen
const float MAX_SPEED = (X_MAX - X_MIN) / MIN_TIME_TO_CROSS;
//	min and max sizes of an object
const float MIN_SIZE = (X_MAX - X_MIN) / 30;
const float MAX_SIZE = (X_MAX - X_MIN) / 10;

//	A bunch of constants for the display of text
const int TEXT_H_PAD = 10;
const int TEXT_V_PAD = 5;

#define SMALL_DISPLAY_FONT    GLUT_BITMAP_HELVETICA_10
#define MEDIUM_DISPLAY_FONT   GLUT_BITMAP_HELVETICA_12
#define LARGE_DISPLAY_FONT    GLUT_BITMAP_HELVETICA_18

const string TEXT_COLOR_STR[NUM_TEXT_COLORS] = { "white",		//	WHITE_TEXT
												"red",			//	RED_TEXT
												"green",		//	GREEN_TEXT
												"magenta" };		//	MAGENTA_TEXT

const string BGND_COLOR_STR[NUM_BGND_COLORS] = { "none",			//
												"light gray",	//	LIGHT_GREY_BGND
												"dark gray",	//	DARK_GREY_BGND
												"green",		//	GREEN_BGND
												"blue",			//	BLUE_BGND
												"brown" };		//	BROWN_BGND

const GLfloat TEXT_COLOR[NUM_TEXT_COLORS][3] = { {1.0f, 1.0f, 1.0f},	//	WHITE_TEXT
												 {1.0f, 0.0f, 0.0f},	//	RED_TEXT
												 {0.0f, 0.8f, 0.0f},	//	GREEN_TEXT
												 {1.0f, 0.0f, 1.0f} };	//	MAGENTA_TEXT

const GLfloat BGND_COLOR[NUM_BGND_COLORS][3] = { {0.0f, 0.0f, 0.0f},	//	NO_BACKGROUND
												 {0.7f, 0.7f, 0.7f},	//	LIGHT_GREY_BGND
												 {0.4f, 0.4f, 0.4f},	//	DARK_GREY_BGND
												 {0.0f, 0.6f, 0.0f},	//	GREEN_BGND
												 {0.0f, 0.0f, 0.6f},	//	BLUE_BGND
												 {0.6f, 0.3f, 0.0f} };	//	BROWN_BGND
const GLfloat* WIN_CLEAR_COLOR = BGND_COLOR[0];

const string WORLD_TYPE_STR[] = {
									"Window world",
									"Box world",
									"Cylinder world",
									"Sphere world" };

const bool displayTextOnLeft = false;
const bool displayTextOnTop = true;
const FontSize fontSize = LARGE_FONT_SIZE;

const int NUM_OBJECTS = 15;

#if 0
//--------------------------------------
#pragma mark -
#pragma mark File-level Global variables
//--------------------------------------
#endif

time_t startTime = -1;
int winWidth = 800,
winHeight = 800;

bool trackEntry = false;
bool trackMousePointer = false;
bool trackPassiveMousePointer = false;
bool pointerInWindow = false;
GLint lastX = -1, lastY = -1;

bool displayText = true;
string stringLine = "";
int textColorIndex = 0;
int bgndColorIndex = 0;//BGND_COLOR[0];

WorldType World2D::worldType = WorldType::SPHERE_WORLD;
bool World2D::drawReferenceFrames = false;

list<shared_ptr<GraphicObject2D> > objList;

int physicsHeartBeat = 1;	// milliseconds
int renderRate = 10;		//	1 rendering frame for 10 simulation heartbeats
bool isAnimated = true;
bool animationJustStarted = false;

random_device myRandDev;
default_random_engine myEngine(myRandDev());
uniform_int_distribution<int> shapeDist(0, 3);	//	triangle - rect - ellipse - face
uniform_real_distribution<float> angleDist(0, 2 * M_PI);
uniform_real_distribution<float> colorDist(0.f, 1.f);
uniform_real_distribution<float> spinDist(-MAX_SPIN, +MAX_SPIN);
uniform_real_distribution<float> speedDist(0.4f * MAX_SPEED, MAX_SPEED);
uniform_real_distribution<float> sizeDist(MIN_SIZE, MAX_SIZE);
uniform_real_distribution<float> xDist(X_MIN, X_MAX);
uniform_real_distribution<float> yDist(Y_MIN, Y_MAX);

std::shared_ptr<SpaceShip> spaceship;

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Callback functions
//--------------------------------------
#endif

//	This is the function that does the actual scene drawing
//	Typically, you shold almost never have to call this function directly yourself.
//	It will be called automatically by glut, the way in Java the JRE invokes the paint
//	method of a frame.  Simply, because there is no inheritance/overriding mechanism, we
//	need to set up this function as a "callback function."  In this demo I do it in the
//	main function.  A plus side of this way of doing is that the function can be named any way
//	we want, and that in fact we can have different display functions and change during
//	the execution of the program the current display function used by glut.
//
void myDisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	//--------------------------
	//	basic drawing code
	//--------------------------

	for (auto obj : objList)
		obj->draw();

	switch (World2D::worldType)
	{
	case WorldType::WINDOW_WORLD:
	case WorldType::BOX_WORLD:
		break;

	case WorldType::CYLINDER_WORLD:
		glPushMatrix();
		//	draw the  left quadrant
		glTranslatef(-World2D::WIDTH, 0.f, 0.f);
		for (auto obj : objList)
			obj->draw();

		//	draw right quadrant
		glTranslatef(2.f * World2D::WIDTH, 0.f, 0.f);
		for (auto obj : objList)
			obj->draw();
		glPopMatrix();
		break;


	case WorldType::SPHERE_WORLD:
		glPushMatrix();
		// Draw central (original) position
		for (auto obj : objList)
			obj->draw();

		// Draw all eight surrounding copies

		// Left and Right translations
		glTranslatef(-World2D::WIDTH, 0.f, 0.f);
		for (auto obj : objList)
			obj->draw();
		glTranslatef(2.f * World2D::WIDTH, 0.f, 0.f);
		for (auto obj : objList)
			obj->draw();
		glTranslatef(-World2D::WIDTH, 0.f, 0.f); // reset to center

		// Top and Bottom translations
		glTranslatef(0.f, World2D::HEIGHT, 0.f);
		for (auto obj : objList)
			obj->draw();
		glTranslatef(0.f, -2.f * World2D::HEIGHT, 0.f);
		for (auto obj : objList)
			obj->draw();
		glTranslatef(0.f, World2D::HEIGHT, 0.f); // reset to center

		// Top-Left, Top-Right, Bottom-Left, Bottom-Right translations
		glTranslatef(-World2D::WIDTH, World2D::HEIGHT, 0.f);
		for (auto obj : objList)
			obj->draw();
		glTranslatef(2.f * World2D::WIDTH, 0.f, 0.f);
		for (auto obj : objList)
			obj->draw();
		glTranslatef(0.f, -2.f * World2D::HEIGHT, 0.f);
		for (auto obj : objList)
			obj->draw();
		glTranslatef(-2.f * World2D::WIDTH, 0.f, 0.f);
		for (auto obj : objList)
			obj->draw();

		glPopMatrix();
		break;

	default:
		break;
	}

	//	Display textual info
	//---------------------------------
	//	We are back at the World2D's origin (by the glPopMatrix() just above), in World2D coordinates.
	//	So we must undo the scaling to be back in pixels, which how text is drawn for now.
	if (displayText)
	{
		//	First, translate to the upper-left corner
		glTranslatef(World2D::X_MIN, World2D::Y_MAX, 0.f);

		//	Then reverse the scaling: back in pixels, making sure that y now points down
		glScalef(World2D::drawInPixelScale, -World2D::drawInPixelScale, 1.f);

		char statusLine[256];
		sprintf(statusLine, "%s | Run time: %d s | %d live ˚objects (%d created) | Mouse last at (%d, %d)",
			WORLD_TYPE_STR[static_cast<int>(World2D::worldType)].c_str(),
			static_cast<int>(time(nullptr) - startTime),
			static_cast<int>(GraphicObject2D::getBaseLiveCount()),
			static_cast<int>(GraphicObject2D::getBaseCount()),
			lastX, lastY);
		displayTextualInfo(statusLine, 0);		//	first row

		if (stringLine != "")
			displayTextualInfo(stringLine, 1);		//	second row
	}

	glPopMatrix();

	drawReferenceFrame();

	//	We were drawing into the back buffer(s), now they should be brought
	//	to the forefront.  This will be explained in a few weeks.
	glutSwapBuffers();
}


//	This callback function is called when the window is resized
//	(generally by the user of the application).
//	It is also called when the window is created, which is why I placed there the
//	code to set up the virtual camera for this application.
//
void myResizeFunc(int w, int h)
{
	winWidth = w;
	winHeight = h;

	if (winWidth != w || winHeight != h)
	{
		glutReshapeWindow(winWidth, winHeight);
	}

	//	This calls says that I want to use the entire dimension of the window for my drawing.
	glViewport(0, 0, winWidth, winHeight);

	//	Here I create my virtual camera.  We are going to do 2D drawing for a while, so what this
	//	does is define the dimensions (origin and units) of the "virtual World2D that my viewport
	//	maps to.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//	Here I define the dimensions of the "virtual World2D" that my
	//	window maps to
	gluOrtho2D(X_MIN, X_MAX, Y_MIN, Y_MAX);

	//	When it's done, request a refresh of the display
	glutPostRedisplay();
}


//	This function is called when a mouse event occurs.  This event, of type s
//	(up, down, dragged, etc.), occurs on a particular button of the mouse.
//
void myMouseHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (spaceship && spaceship->isAlive()) {
			spaceship->fireProjectile();
		}
	}
}



void myMouseMotionHandler(int ix, int iy)
{
	if (trackMousePointer)
	{
		cout << "Active mouse at (" << ix << ", " << iy << ")" << endl;
	}
}
void myMousePassiveMotionHandler(int ix, int iy)
{
	lastX = ix;
	lastY = iy;
	pointerInWindow = (ix >= 0 && ix < winWidth && iy >= 0 && iy < winHeight);

	if (trackPassiveMousePointer)
	{
		cout << "Passive mouse at (" << ix << ", " << iy << ")" << endl;
	}

}
void myEntryHandler(int state)
{
	if (trackEntry)
	{
		if (state == GLUT_ENTERED)
		{
			pointerInWindow = true;
			cout << "===> Pointer entered" << endl;
		}
		else	// GLUT_LEFT
		{
			pointerInWindow = false;
			cout << "<=== Pointer exited" << endl;
		}
	}
}

//	This callback function is called when a keyboard event occurs
//
void myKeyHandler(unsigned char c, int x, int y)
{
	// silence warning
	(void)x;
	(void)y;

	switch (c)
	{
	case 'q':
	case 27:
		exit(0);
		break;

	case ' ':
		if (spaceship && spaceship->isAlive()) {
			spaceship->fireProjectile();
		}
		break;

		//-------------------------
		//	mouse tracking cases
		//-------------------------
	case 'm':
		trackMousePointer = !trackMousePointer;
		break;

	case 'p':
		trackPassiveMousePointer = !trackPassiveMousePointer;
		break;

	case 'e':
		trackEntry = !trackEntry;
		break;

		//-------------------------
		//	text display cases
		//-------------------------
	case 'i':
		cout << "Enter a new line of text: ";
		getline(cin, stringLine);
		break;

	case 't':
		displayText = !displayText;
		break;

		////-------------------------
		////	world mode cases
		////-------------------------
		////	activates "window" mode (objects disappear at the edge)
		//case 'w':
		//	World2D::worldType = WorldType::WINDOW_WORLD;
		//	break;

		////	activates "box" mode (objects bounce off all edges)
		//case 'b':
		//	World2D::worldType = WorldType::BOX_WORLD;
		//	break;

		////	activates "cylindrical" mode (objects wrap around vertical
		////			edges, bounce off the horizontal edges)
		//case 'c':
		//	World2D::worldType = WorldType::CYLINDER_WORLD;
		//	break;

		////	activates "spherical" mode (objects wrap around all edges)
		//case 's':
		//	World2D::worldType = WorldType::SPHERE_WORLD;
		//	break;

		//-----------------------------
		//	Reference frames
		//-----------------------------
	case 'f':
		World2D::drawReferenceFrames = !World2D::drawReferenceFrames;
		break;

		//-----------------------------
		//	Bounding boxes
		//-----------------------------
	case 'A':
		BoundingBox::setDrawAbsoluteBoxes(!BoundingBox::absoluteBoxesAreDrawn());
		break;

	case 'r':
		BoundingBox::setDrawRelativeBoxes(!BoundingBox::relativeBoxesAreDrawn());
		break;
		//-------------------------
		// Spaceship movement
		//-------------------------
	case 'a': // 'A' key pressed for left rotation
		if (spaceship && spaceship->cantmove()) {
			spaceship->setAngularVelocity(200.0f); // Start counterclockwise rotation
		}
		else
		{
			spaceship->setAngularVelocity(0.0f);
		}
		break;

	default:
		break;
	}
}

void myKeyUpHandler(unsigned char c, int x, int y)
{
	// Silence warning
	(void)x;
	(void)y;

	if (spaceship) {
		switch (c) {
		case 'a': // 'A' key released, stop rotation
			spaceship->setAngularVelocity(0.0f); // Stop rotation
			break;

		default:
			break;
		}
	}
}


void mySpecialKeyHandler(int key, int x, int y)
{
	(void)x;  // Suppress unused parameter warning
	(void)y;  // Suppress unused parameter warning

	if (spaceship) {
		switch (key) {
		case GLUT_KEY_LEFT:  // Left arrow key
			if (spaceship && spaceship->cantmove()) {
				spaceship->setAngularVelocity(200.0f); // Start counterclockwise rotation
			}
			else
			{
				spaceship->setAngularVelocity(0.0f);
			}
			break;
		default:
			break;
		}
	}
}

void mySpecialKeyUpHandler(int key, int x, int y)
{
	(void)x;  // Suppress unused parameter warning
	(void)y;  // Suppress unused parameter warning

	if (spaceship) {
		switch (key) {
		case GLUT_KEY_LEFT:  // Left arrow key released
			spaceship->setAngularVelocity(0.0f); // Stop rotation
			break;

		default:
			break;
		}
	}
}

void generateRandomAsteroid() {
	// Generate random properties for the asteroid
	float x = xDist(myEngine);
	float y = yDist(myEngine);
	float angle = angleDist(myEngine);
	float direction = angleDist(myEngine);
	float speed = speedDist(myEngine);
	float spin = spinDist(myEngine);
	float r = colorDist(myEngine);
	float g = colorDist(myEngine);
	float b = colorDist(myEngine);
	float size = sizeDist(myEngine);

	// Choose random shape for the asteroid
	switch (shapeDist(myEngine)) {
	case 0:
		objList.push_back(make_shared<Triangle>(x, y, angle, size, r, g, b, true,
			speed * cosf(direction), speed * sinf(direction), spin));
		break;

	case 1:
		objList.push_back(make_shared<earshooter::Rectangle2D>(x, y, angle, size, size, r, g, b, true,
			speed * cosf(direction), speed * sinf(direction), spin));
		break;

	case 2:
		objList.push_back(make_shared<earshooter::Ellipse2D>(x, y, angle, size, size, r, g, b, true,
			speed * cosf(direction), speed * sinf(direction), spin));
		break;

	case 3:
		objList.push_back(make_shared<SmilingFace>(x, y, angle, size, r, g, b,
			speed * cosf(direction), speed * sinf(direction), spin));
		break;

	default:
		break;
	}
}



void myTimerFunc(int value)
{
	static int updateFrameIndex = 0;
	static chrono::high_resolution_clock::time_point lastTime = chrono::high_resolution_clock::now();
	static float timeSinceLastAsteroid = 0.0f;  // Track time for asteroid spawning
	const float asteroidSpawnInterval = 1.0f;    // Spawn every 1 seconds

	// Re-prime the timer
	glutTimerFunc(physicsHeartBeat, myTimerFunc, value);

	if (isAnimated)
	{
		chrono::high_resolution_clock::time_point currentTime = chrono::high_resolution_clock::now();
		float dt = chrono::duration_cast<chrono::duration<float>>(currentTime - lastTime).count();
		if (animationJustStarted)
		{
			dt = 0.f;
			animationJustStarted = false;
		}
		lastTime = currentTime;

		// Update all objects in objList
		for (auto iter = objList.begin(); iter != objList.end(); )
		{
			UpdateStatus status = (*iter)->update(dt);
			if (status == UpdateStatus::DEAD)
			{
				iter = objList.erase(iter);  // Remove dead objects
			}
			else
			{
				++iter;
			}
		}

		// Periodically generate new asteroids
		timeSinceLastAsteroid += dt;
		if (timeSinceLastAsteroid >= asteroidSpawnInterval && spaceship->isAlive()) {
			generateRandomAsteroid();
			timeSinceLastAsteroid = 0.0f;  // Reset the spawn timer
		}
	}

	// Trigger rendering as needed
	if (updateFrameIndex++ % renderRate == 0)
		glutPostRedisplay();
}




//	This  is where the menu item selected is identified.  This is
//	why  we need a unique code per menu item.
void myMenuHandler(int choice)
{
	switch (choice)
	{
		//	Exit/Quit
	case QUIT_MENU:
		exit(0);
		break;

		//	Do something
	case OTHER_MENU_ITEM:
		break;

	default:	//	this should not happen
		break;

	}

	glutPostRedisplay();
}

//  in this example my submenu selection indicates the keyboard handling
//  function to use.
void mySubmenuHandler(int choice)
{
	switch (choice)
	{
	case RED_TEXT:
	case GREEN_TEXT:
	case WHITE_TEXT:
	case MAGENTA_TEXT:
		textColorIndex = choice - FIRST_TEXT;
		break;

	case LIGHT_GREY_BGND:
	case DARK_GREY_BGND:
	case GREEN_BGND:
	case BLUE_BGND:
	case BROWN_BGND:
		bgndColorIndex = choice - FIRST_BGND;
		break;

	default:
		break;
	}
}

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Utilities
//--------------------------------------
#endif

void drawSquare(float cx, float cy, float size, float r,
	float g, float b, bool drawContour)
{
	float halfSize = 0.5f * size;

	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glVertex2f(cx - halfSize, cy - halfSize);
	glVertex2f(cx + halfSize, cy - halfSize);
	glVertex2f(cx + halfSize, cy + halfSize);
	glVertex2f(cx - halfSize, cy + halfSize);
	glEnd();

	if (drawContour)
	{
		// simply invert the filling color
		glColor3f(1.f - r, 1.f - g, 1.f - b);
		glBegin(GL_LINE_LOOP);
		glVertex2f(cx - halfSize, cy - halfSize);
		glVertex2f(cx + halfSize, cy - halfSize);
		glVertex2f(cx + halfSize, cy + halfSize);
		glVertex2f(cx - halfSize, cy + halfSize);
		glEnd();
	}
}


void displayTextualInfo(const string& infoStr, int textRow) {
	displayTextualInfo(infoStr.c_str(), textRow);
}

void displayTextualInfo(const char* infoStr, int textRow)
{
	//-----------------------------------------------
	//  0.  Build the string to display <-- parameter
	//-----------------------------------------------
	int infoLn = static_cast<int> (strlen(infoStr));

	//-----------------------------------------------
	//  1.  Determine the string's length (in pixels)
	//-----------------------------------------------
	int textWidth = 0, fontHeight = -1;
	switch (fontSize)
	{
	case SMALL_FONT_SIZE:
		fontHeight = 10;
		for (int k = 0; k < infoLn; k++)
		{
			textWidth += glutBitmapWidth(SMALL_DISPLAY_FONT, infoStr[k]);
		}
		break;

	case MEDIUM_FONT_SIZE:
		fontHeight = 12;
		for (int k = 0; k < infoLn; k++)
		{
			textWidth += glutBitmapWidth(MEDIUM_DISPLAY_FONT, infoStr[k]);
		}
		break;

	case LARGE_FONT_SIZE:
		fontHeight = 16;
		for (int k = 0; k < infoLn; k++)
		{
			textWidth += glutBitmapWidth(LARGE_DISPLAY_FONT, infoStr[k]);
		}
		break;

	default:
		break;
	}

	//-----------------------------------------------
	//  2.  get current material properties
	//-----------------------------------------------
	float oldAmb[4], oldDif[4], oldSpec[4], oldShiny;
	glGetMaterialfv(GL_FRONT, GL_AMBIENT, oldAmb);
	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, oldDif);
	glGetMaterialfv(GL_FRONT, GL_SPECULAR, oldSpec);
	glGetMaterialfv(GL_FRONT, GL_SHININESS, &oldShiny);

	glPushMatrix();
	if (displayTextOnTop)
	{
		glTranslatef(0.f, textRow * (fontHeight + 2 * TEXT_V_PAD), 0.f);
	}
	else
	{
		glTranslatef(0.f, winHeight - (textRow + 1) * (fontHeight + 2 * TEXT_V_PAD), 0.f);
	}

	//-----------------------------------------------
	//  3.  Clear background rectangle if required
	//-----------------------------------------------
	if (bgndColorIndex != 0)
	{
		glColor3fv(BGND_COLOR[bgndColorIndex]);
		glBegin(GL_POLYGON);
		glVertex2i(0, 0);
		glVertex2i(0, fontHeight + 2 * TEXT_V_PAD);
		glVertex2i(winWidth, fontHeight + 2 * TEXT_V_PAD);
		glVertex2i(winWidth, 0);
		glEnd();
	}

	//	Move "up" from current plane, to make sure that we overwrite
	glTranslatef(0.f, 0.f, 0.1f);

	//-----------------------------------------------
	//  4.  Draw the string
	//-----------------------------------------------    
	//	Where do we start drawing from
	int xPos = displayTextOnLeft ? TEXT_H_PAD : winWidth - textWidth - TEXT_H_PAD;
	int yPos = fontHeight + TEXT_V_PAD;

	glColor3fv(TEXT_COLOR[textColorIndex]);
	int x = xPos;
	switch (fontSize)
	{
	case SMALL_FONT_SIZE:
		for (int k = 0; k < infoLn; k++)
		{
			glRasterPos2i(x, yPos);
			glutBitmapCharacter(SMALL_DISPLAY_FONT, infoStr[k]);
			x += glutBitmapWidth(SMALL_DISPLAY_FONT, infoStr[k]);
		}
		break;

	case MEDIUM_FONT_SIZE:
		for (int k = 0; k < infoLn; k++)
		{
			glRasterPos2i(x, yPos);
			glutBitmapCharacter(MEDIUM_DISPLAY_FONT, infoStr[k]);
			x += glutBitmapWidth(MEDIUM_DISPLAY_FONT, infoStr[k]);
		}
		break;

	case LARGE_FONT_SIZE:
		for (int k = 0; k < infoLn; k++)
		{
			glRasterPos2i(x, yPos);
			glutBitmapCharacter(LARGE_DISPLAY_FONT, infoStr[k]);
			x += glutBitmapWidth(LARGE_DISPLAY_FONT, infoStr[k]);
		}
		break;

	default:
		break;
	}

	//-----------------------------------------------
	//  5.  Restore old material properties
	//-----------------------------------------------
	glMaterialfv(GL_FRONT, GL_AMBIENT, oldAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, oldDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, oldSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, oldShiny);

	//-----------------------------------------------
	//  6.  Restore reference frame
	//-----------------------------------------------
	glPopMatrix();
}

void printMatrix(const GLfloat* m) {
	cout << "((" << m[0] << "\t" << m[4] << "\t" << m[8] << "\t" << m[12] << ")" << endl;
	cout << " (" << m[1] << "\t" << m[5] << "\t" << m[9] << "\t" << m[13] << ")" << endl;
	cout << " (" << m[2] << "\t" << m[6] << "\t" << m[10] << "\t" << m[14] << ")" << endl;
	cout << " (" << m[3] << "\t" << m[7] << "\t" << m[11] << "\t" << m[15] << "))" << endl;
}

#if 0
//--------------------------------------
#pragma mark -
#pragma mark Application init and main
//--------------------------------------
#endif

void applicationInit()
{
	// Create Menus
	int myMenu;

	//	Submenu for changing keyboard handling function
	int myTextColorSubmenu = glutCreateMenu(mySubmenuHandler);
	for (int k = 0, t = FIRST_TEXT; k < NUM_TEXT_COLORS; k++, t++)
		glutAddMenuEntry(TEXT_COLOR_STR[k].c_str(), t);
	int myBgndColorSubmenu = glutCreateMenu(mySubmenuHandler);
	for (int k = 0, b = FIRST_BGND; k < NUM_BGND_COLORS; k++, b++)
		glutAddMenuEntry(BGND_COLOR_STR[k].c_str(), b);

	// Main menu that the submenus are connected to
	myMenu = glutCreateMenu(myMenuHandler);
	glutAddMenuEntry("Quit", MenuItemID::QUIT_MENU);
	//
	glutAddMenuEntry("-", MenuItemID::SEPARATOR);
	glutAddMenuEntry("Other stuff", MenuItemID::OTHER_MENU_ITEM);
	glutAddMenuEntry("New entry", 64);

	glutAddMenuEntry("-", MenuItemID::SEPARATOR);
	glutAddSubMenu("Text color:", myTextColorSubmenu);
	glutAddSubMenu("Background color:", myBgndColorSubmenu);
	glutAddMenuEntry("-", MenuItemID::SEPARATOR);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	spaceship = std::make_shared<SpaceShip>(0.f, 0.f, 0.f, 0.5f, 1.0f, 0.f, 0.f, true,
		0.f, 0.f, 0.f);
	objList.push_back(spaceship);

	////	Create a bunch of objects
	//for (int k=0; k< NUM_OBJECTS; k++)
	//{
	//	float x = xDist(myEngine), y = yDist(myEngine);
	//	float angle = angleDist(myEngine);
	//	float direction = angleDist(myEngine);
	//	float speed = speedDist(myEngine);
	//	float spin = spinDist(myEngine);
	//	float r = colorDist(myEngine), g = colorDist(myEngine), b = colorDist(myEngine);
	//	switch(shapeDist(myEngine))
	//	{
	//		case 0:
	//		objList.push_back(make_shared<Triangle>(x, y, angle, sizeDist(myEngine), r, g, b, true,
	//							speed*cosf(direction), speed*sinf(direction), spin));
	//		break;

	//		case 1:
	//		objList.push_back(make_shared<earshooter::Rectangle2D>(x, y, angle, sizeDist(myEngine), sizeDist(myEngine), r, g, b, true,
	//							speed*cosf(direction), speed*sinf(direction), spin));
	//		break;

	//		case 2:
	//		objList.push_back(make_shared<earshooter::Ellipse2D>(x, y, angle, sizeDist(myEngine), sizeDist(myEngine), r, g, b, true,
	//							speed*cosf(direction), speed*sinf(direction), spin));
	//		break;

	//		case 3:
	//		objList.push_back(make_shared<SmilingFace>(x, y, angle, sizeDist(myEngine), r, g, b,
	//							speed*cosf(direction), speed*sinf(direction), spin));
	//		break;

	//		default:
	//		break;
	//	}
	//	
	//}


	World2D::setWorld2DBounds(X_MIN, X_MAX, Y_MIN, Y_MAX, winWidth, winHeight);

	//	time really starts now
	startTime = time(nullptr);
}

int main(int argc, char* argv[])
{
	//	Initialize glut and create a new window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
	glutCreateWindow("Earshooter Demo");
	glClearColor(WIN_CLEAR_COLOR[0], WIN_CLEAR_COLOR[1], WIN_CLEAR_COLOR[2], 1.f);

	//	set up the callbacks
	glutDisplayFunc(myDisplayFunc);
	glutReshapeFunc(myResizeFunc);
	glutMouseFunc(myMouseHandler);
	glutMotionFunc(myMouseMotionHandler);
	glutPassiveMotionFunc(myMousePassiveMotionHandler);
	glutEntryFunc(myEntryHandler);
	glutKeyboardFunc(myKeyHandler);
	glutKeyboardUpFunc(myKeyUpHandler);			   // For key releases
	glutSpecialFunc(mySpecialKeyHandler);         // For special key presses
	glutSpecialUpFunc(mySpecialKeyUpHandler);    // For special key releases
	glutTimerFunc(physicsHeartBeat, myTimerFunc, 0);
	//			  time	    name of		value to pass
	//			  in ms		function	to the func
	SpaceShip::setObjectList(&objList);
	Projectile::setObjectList(&objList);

	//	Now we can do application-level
	applicationInit();

	//	Now we enter the main loop of the program and to a large extend
	//	"lose control" over its execution.  The callback functions that
	//	we set up earlier will be called when the corresponding event
	//	occurs
	glutMainLoop();

	//	This will never be executed (the exit point will be in one of the
	//	callback functions).
	return 0;
}
