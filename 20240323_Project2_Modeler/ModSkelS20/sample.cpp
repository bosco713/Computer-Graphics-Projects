// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
		glTranslated(-5,0,-5);
		drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the sample model
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_GREEN);
	// drawTriangle(1, 1, 1, 2, 2, 2, 3, 3, 3);
	// drawRightAngledTrianglePrism(3, 4, 5);
	// drawTorus(5, 2);
	int angle_of_triangle = 45;
	int flip_in_rotation = 45;
	int size = 3;
	drawStar(size, flip_in_rotation, angle_of_triangle);
	/*glPushMatrix();
		glPushMatrix();
			glTranslated(0, size, 0);
			glRotated(atan(sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI) / sin((double)angle_of_triangle / 180 * M_PI)) * 180 / M_PI, 1, 0, 0);
			glPushMatrix();
				glRotated(flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, 0, 0, size, -1 * size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI));
			glPopMatrix();
			glPushMatrix();
				glRotated(-1 * flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, 0, size);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, size, 0);
			glRotated(90, 0, 1, 0);
			glRotated(atan(sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI) / sin((double)angle_of_triangle / 180 * M_PI)) * 180 / M_PI, 1, 0, 0);
			glPushMatrix();
				glRotated(flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, 0, 0, size, -1 * size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI));
			glPopMatrix();
			glPushMatrix();
				glRotated(-1 * flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, 0, size);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, size, 0);
			glRotated(180, 0, 1, 0);
			glRotated(atan(sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI) / sin((double)angle_of_triangle / 180 * M_PI)) * 180 / M_PI, 1, 0, 0);
			glPushMatrix();
				glRotated(flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, 0, 0, size, -1 * size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI));
			glPopMatrix();
			glPushMatrix();
				glRotated(-1 * flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, 0, size);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, size, 0);
			glRotated(270, 0, 1, 0);
			glRotated(atan(sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI) / sin((double)angle_of_triangle / 180 * M_PI)) * 180 / M_PI, 1, 0, 0);
			glPushMatrix();
				glRotated(flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, 0, 0, size, -1 * size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI));
			glPopMatrix();
			glPushMatrix();
				glRotated(-1 * flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, 0, size);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	// draw the back side
	glPushMatrix();
		// glTranslated(0, 4, 0);
		glRotated(180, 0, 0, 1);
		glTranslated(0, -size+ sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI), 0);
		glPushMatrix();
			glTranslated(0, size, 0);
			glRotated(atan(sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI) / sin((double)angle_of_triangle / 180 * M_PI)) * 180 / M_PI, 1, 0, 0);
			glPushMatrix();
				glRotated(flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, 0, 0, size, -1 * size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI));
			glPopMatrix();
			glPushMatrix();
				glRotated(-1 * flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, 0, size);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, size, 0);
			glRotated(90, 0, 1, 0);
			glRotated(atan(sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI) / sin((double)angle_of_triangle / 180 * M_PI)) * 180 / M_PI, 1, 0, 0);
			glPushMatrix();
				glRotated(flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, 0, 0, size, -1 * size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI));
			glPopMatrix();
			glPushMatrix();
				glRotated(-1 * flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, 0, size);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, size, 0);
			glRotated(180, 0, 1, 0);
			glRotated(atan(sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI) / sin((double)angle_of_triangle / 180 * M_PI)) * 180 / M_PI, 1, 0, 0);
			glPushMatrix();
				glRotated(flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, 0, 0, size, -1 * size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI));
			glPopMatrix();
			glPushMatrix();
				glRotated(-1 * flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, 0, size);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, size, 0);
			glRotated(270, 0, 1, 0);
			glRotated(atan(sin((double)flip_in_rotation / 180 * M_PI) * cos((double)angle_of_triangle / 180 * M_PI) / sin((double)angle_of_triangle / 180 * M_PI)) * 180 / M_PI, 1, 0, 0);
			glPushMatrix();
				glRotated(flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, 0, 0, size, -1 * size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI));
			glPopMatrix();
			glPushMatrix();
				glRotated(-1 * flip_in_rotation, 0, 0, 1);
				drawTriangle(0, 0, 0, size * cos((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, size * sin((double)angle_of_triangle / 180 * M_PI) * sin((double)angle_of_triangle / 180 * M_PI), 0, 0, size);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();*/
	/*glPushMatrix();
		glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

		glPushMatrix();
			glTranslated(-1.5, 0, -2);
			glScaled(3, 1, 4);
			drawBox(1,1,1);
		glPopMatrix();

		// draw cannon
		glPushMatrix();
			glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			drawCylinder(VAL(HEIGHT), 0.1, 0.1);

			glTranslated(0.0, 0.0, VAL(HEIGHT));
			drawCylinder(1, 1.0, 0.9);

			glTranslated(0.0, 0.0, 0.5);
			glRotated(90, 1.0, 0.0, 0.0);
			drawCylinder(4, 0.1, 0.2);
		glPopMatrix();

	glPopMatrix();*/
}

/*int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);

    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}*/
