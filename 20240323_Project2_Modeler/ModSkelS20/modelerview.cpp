#include "modelerview.h"
#include "modelerapp.h"
#include "camera.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>

#include "modelerglobals.h"

static const int	kMouseRotationButton			= FL_LEFT_MOUSE;
static const int	kMouseTranslationButton			= FL_MIDDLE_MOUSE;
static const int	kMouseZoomButton				= FL_RIGHT_MOUSE;

ModelerView::ModelerView(int x, int y, int w, int h, char *label)
: Fl_Gl_Window(x,y,w,h,label)
{
    m_camera = new Camera();
}

ModelerView::~ModelerView()
{
	delete m_camera;
}
int ModelerView::handle(int event)
{
    unsigned eventCoordX = Fl::event_x();
	unsigned eventCoordY = Fl::event_y();
	unsigned eventButton = Fl::event_button();
	unsigned eventState  = Fl::event_state();

	switch(event)	 
	{
	case FL_PUSH:
		{
			switch(eventButton)
			{
			case kMouseRotationButton:
				m_camera->clickMouse(kActionRotate, eventCoordX, eventCoordY );
				break;
			case kMouseTranslationButton:
				m_camera->clickMouse(kActionTranslate, eventCoordX, eventCoordY );
				break;
			case kMouseZoomButton:
				m_camera->clickMouse(kActionZoom, eventCoordX, eventCoordY );
				break;
			}
			if (Fl::event_ctrl())
			{
				m_camera->clickMouse(kActionTwist, eventCoordX, eventCoordY);
			}
           // printf("push %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	case FL_DRAG:
		{
			m_camera->dragMouse(eventCoordX, eventCoordY);
            //printf("drag %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	case FL_RELEASE:
		{
			switch(eventButton)
			{
			case kMouseRotationButton:
			case kMouseTranslationButton:
			case kMouseZoomButton:
				m_camera->releaseMouse(eventCoordX, eventCoordY );
				break;
			}
          //  printf("release %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	default:
		return 0;
	}
	
	redraw();

	return 1;
}

void ModelerView::draw()
{
    if (!valid())
    {
        glShadeModel( GL_SMOOTH );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
        glEnable( GL_LIGHT1 );
		glEnable( GL_NORMALIZE );
    }

  	glViewport( 0, 0, w(), h() );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0,float(w())/float(h()),1.0,100.0);
				
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_camera->applyViewingTransform();

	GLfloat lightPosition0[] = { VAL(LIGHT0_X), VAL(LIGHT0_Y), VAL(LIGHT0_Z), 0.0f };
	GLfloat lightDiffuse0[] = { VAL(LIGHT0_INTENSITY), VAL(LIGHT0_INTENSITY), VAL(LIGHT0_INTENSITY), 1.0f };
	GLfloat lightPosition1[] = { VAL(LIGHT1_X), VAL(LIGHT1_Y), VAL(LIGHT1_Z), 0.0f };
	GLfloat lightDiffuse1[] = { VAL(LIGHT1_INTENSITY), VAL(LIGHT1_INTENSITY), VAL(LIGHT1_INTENSITY), 1.0f };

    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition0 );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse0 );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition1 );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse1 );
}