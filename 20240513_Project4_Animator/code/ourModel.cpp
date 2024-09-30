// Our group's model.  
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerglobals.h"
// #include "lsystem.h"
#include <FL/gl.h>
#include <math.h>
#include <time.h>
#include <iostream>

// To make our Model, we inherit off of ModelerView
class ourModel : public ModelerView
{
public:
    ourModel(int x, int y, int w, int h, char* label)
        : ModelerView(x, y, w, h, label) { }

    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createOurModel(int x, int y, int w, int h, char* label)
{
    return new ourModel(x, y, w, h, label);
}

/*
void inverseKinematics(double *thetaXA, double *thetaXB, double *thetaXC, double *thetaZA, double *thetaZB, double *thetaZC) {
	double l1 = 3 + 0.5;
	double l2 = 3 + 0.5;
	double l3 = 3 + 0.5;
	double x_required = (double) VAL(INVERSEKINEMATICSX);
	double y_required = (double) VAL(INVERSEKINEMATICSY);
	double z_required = (double) VAL(INVERSEKINEMATICSZ) ;
	double root_location_x = (double) 0;
	double root_location_y = (double) 7.25;
	double root_location_z = (double) 7.5;
    double dz = z_required - root_location_z;
    double dy = y_required - root_location_y;
    // cout << "For yz plane:" << endl;
    // cout << "dy = " << dy << endl;
    // cout << "dz = " << dz << endl;
    double dtheta = dz? atan(dy/dz): 0;
    // cout << "dtheta = " << dtheta << endl;

    double upperTan = dy - l3 * sin(dtheta);
    double lowerTan = dz - l3 * cos(dtheta);
    // cout << "upperTan = " << upperTan << endl;
    // cout << "lowerTan = " << lowerTan << endl;

    double upperCos = dy*dy + dz*dz + l1*l1 - l2*l2 + l3*l3 - 2*l3*(dy*sin(dtheta) + dz*cos(dtheta));
    double lowerCos = 2*l1*sqrt(pow(dy - l3*sin(dtheta), 2) + pow(dz - l3*cos(dtheta), 2));
    // cout << "upperCos = " << upperCos << endl;
    // cout << "lowerCos = " << lowerCos << endl;

    double arcTan = atan(upperTan/lowerTan);
    double arcCos = acos(upperCos/lowerCos);
    // cout << "arcTan = " << arcTan << endl;
    // cout << "arcCos = " << arcCos << endl;

    *thetaZA = arcTan - arcCos;
    // cout << "*thetaZA = " << *thetaZA << endl;;

    double cosThetaZA = cos(*thetaZA);
    double arcCos2 = acos((dz-l1*cosThetaZA-l3*cos(dtheta))/l2);
    // cout << "arcCos2 = " << arcCos2 << endl;

	*thetaZB = arcCos2 - *thetaZA;
    // cout << "*thetaZB = " << *thetaZB << endl;

	*thetaZC = dtheta - *thetaZA - *thetaZB;
    // cout << "*thetaZC = " << *thetaZC << endl << endl;
    *thetaZA = *thetaZA/M_PI * 180;
    *thetaZB = *thetaZB/M_PI * 180;
    *thetaZC = *thetaZC/M_PI * 180;


	root_location_x = (double) 0;
	root_location_y = (double) 7.25;
	root_location_z = (double) 7.5;
    dz = z_required - root_location_z;
    double dx = x_required - root_location_x;
    // cout << "For yz plane:" << endl;
    // cout << "dy = " << dy << endl;
    // cout << "dz = " << dz << endl;
    dtheta = dz? atan(dx/dz): 0;
    // cout << "dtheta = " << dtheta << endl;

    upperTan = dx - l3 * sin(dtheta);
    lowerTan = dz - l3 * cos(dtheta);
    // cout << "upperTan = " << upperTan << endl;
    // cout << "lowerTan = " << lowerTan << endl;

    upperCos = dx*dx + dz*dz + l1*l1 - l2*l2 + l3*l3 - 2*l3*(dx*sin(dtheta) + dz*cos(dtheta));
    lowerCos = 2*l1*sqrt(pow(dx - l3*sin(dtheta), 2) + pow(dz - l3*cos(dtheta), 2));
    // cout << "upperCos = " << upperCos << endl;
    // cout << "lowerCos = " << lowerCos << endl;

    arcTan = atan(upperTan/lowerTan);
    arcCos = acos(upperCos/lowerCos);
    // cout << "arcTan = " << arcTan << endl;
    // cout << "arcCos = " << arcCos << endl;

    *thetaXA = arcTan - arcCos;
    // cout << "*thetaZA = " << *thetaZA << endl;;

    double cosThetaXA = cos(*thetaXA);
    arcCos2 = acos((dz-l1*cosThetaXA-l3*cos(dtheta))/l2);
    // cout << "arcCos2 = " << arcCos2 << endl;

	*thetaXB = arcCos2 - *thetaXA;
    // cout << "*thetaZB = " << *thetaZB << endl;

	*thetaXC = dtheta - *thetaXA - *thetaXB;
    // cout << "*thetaZC = " << *thetaZC << endl << endl;
    *thetaXA = *thetaXA/M_PI * 180;
    *thetaXB = *thetaXB/M_PI * 180;
    *thetaXC = *thetaXC/M_PI * 180;
}
*/

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out ourModel
void ourModel::draw()
{
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();

	// draw the our model
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_WHITE);

	double pi = 360 * (ModelerApplication::Instance()->GetFrame() / 49.0f) * M_PI / 180;

	// if (VAL(DISPLAY_LSYSTEM) == 0) {
		// draw the our model
		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(COLOR_GREEN);

		/*
		double thetaXA, thetaXB, thetaXC, thetaZA, thetaZB, thetaZC = 0.0f;
		inverseKinematics(&thetaXA, &thetaXB, &thetaXC, &thetaZA, &thetaZB, &thetaZC);
		*/

		glPushMatrix();
			switch ((int)VAL(MOTION_SET)) {
				case(1):
					glTranslated(0, 0, 4);
					break;
				case(2):
					glTranslated(0, 2, 4);
					glRotated(-20, 1, 0, 0);
					break;
				case(3):
					glTranslated(0, 6, 2);
					glRotated(-30, 1, 0, 0);
					break;
				default:
					glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
					break;
			}
		
			// draw main body (key = 0)
			glPushMatrix();
				glTranslated(-2.25, 5 + 2 * sin(pi), -7.5);
				glRotated(2 * cos(pi), 1, 0, 0);
				drawBox(4.5, 4.5, 15);

				if (VAL(LOD) >= 5) {
					// draw tail
					glPushMatrix();
						// TAIL_1
						setDiffuseColor(COLOR_BLUE);
						glTranslated(1.25, 2.4, 0);
						glRotated(-5 * cos(pi), 1, 0, 0);
						drawBox(2.0, 2.0, -6.0);
						glPushMatrix();
							// TAIL_2
							setDiffuseColor(COLOR_RED);
							glTranslated(0.5, 0.5, -5);
							drawBox(1.0, 1.0, -2.5);
							glPushMatrix();
								// TAIL_3
								setDiffuseColor(COLOR_BLUE);
								glTranslated(-0.5, -0.5, -1.5);
								glRotated(-7 * sin(pi), -1, 0, 0);
								drawBox(2.0, 2.0, -6.0);
								glPushMatrix();
									// TAIL_4
									setDiffuseColor(COLOR_RED);
									glTranslated(0.5, 0.5, -5);
									drawBox(1.0, 1.0, -2.5);
									glPushMatrix();
										// TAIL_5
										setDiffuseColor(COLOR_BLUE);
										glTranslated(-0.5, -0.5, -1.5);
										glRotated(-10 * cos(pi), -1, 0, 0);
										drawBox(2.0, 2.0, -6.0);

									glPopMatrix();
								glPopMatrix();
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				}

				if (VAL(LOD) >= 4) {
					// draw left back leg
					glPushMatrix();
						setDiffuseColor(COLOR_BLUE);
						glTranslated(3.75, 0, 5);
						glRotated(-20 + 2 * sin(pi), 1, 0, 0);
						drawBox(3.5, 3.5, -7.5);
						glPushMatrix();
							//
							setDiffuseColor(COLOR_BLUE);
							glTranslated(0.5, 0.5, -7.5);
							glRotated(15 + 3 * cos(pi), 1, 0, 0);
							drawBox(2.5, 2.5, -6.5);
							glPushMatrix();
								//
								setDiffuseColor(COLOR_BLUE);
								glTranslated(-0.5, 1.5, -6);
								glRotated(-40 + 2 * sin(pi), 1, 0, 0);
								drawBox(3.5, 1, -4.5);

							glPopMatrix();
						glPopMatrix();
					glPopMatrix();

					// draw right back leg
					glPushMatrix();
						setDiffuseColor(COLOR_BLUE);
						glTranslated(-2.75, 0, 5);
						glRotated(-20 + 2 * cos(pi), 1, 0, 0);
						drawBox(3.5, 3.5, -7.5);
						glPushMatrix();
							//
							setDiffuseColor(COLOR_BLUE);
							glTranslated(0.5, 0.5, -7.5);
							glRotated(15 + 3 * sin(pi), 1, 0, 0);
							drawBox(2.5, 2.5, -6.5);
							glPushMatrix();
								//
								setDiffuseColor(COLOR_BLUE);
								glTranslated(-0.5, 1.5, -6);
								glRotated(-40 + 2 * cos(pi), 1, 0, 0);
								drawBox(3.5, 1, -4.5);

							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				}

				if (VAL(LOD) >= 3) {
					// draw left wing
					glPushMatrix();
						// level 1

						setDiffuseColor(COLOR_BLUE);
						glTranslated(3.25, 4.5, 7.5);

						if (ModelerApplication::Instance()->GetAnimate())
							glRotated(-10 * sin(pi), 0, 0, 1);
						else
							switch ((int)VAL(MOTION_SET)) {
								case(1):
									glRotated(5, 0, 0, 1);
									break;
								case(2):
									glRotated(10, 0, 0, 1);
									break;
								case(3):
									glRotated(-5, 0, 0, 1);
									break;
								default:
									glRotated(VAL(WING_A), 0, 0, 1);				// the rotation of wings
									break;
							}

						drawBox(6, 0.5f, 5);	// WING_1
						glPushMatrix();
							setDiffuseColor(COLOR_GREEN);
							glTranslated(6, 0, 5);
							glRotated(180, 0, 0, 1);
							glRotated(90, 1, 0, 0);
							drawRightAngledTrianglePrism(6, 5, 0.5f);	// WING_2

						glPopMatrix();
						glPushMatrix();
							setDiffuseColor(COLOR_RED);
							glTranslated(0, 0.25f, 5);
							glRotated(atan((double)6/5)/M_PI*180, 0, 1, 0);
							drawCylinder(sqrt(61), 0.3f, 0.3f);			// WING_3

						glPopMatrix();
						glPushMatrix();
							// level 2

							glTranslated(6, 0.25f, 0);				// x translate depends on the previous box x value
							drawCylinder(10, 0.3f, 0.3f);			// WING_4
							setDiffuseColor(COLOR_BLUE);
							glTranslated(0, -0.25f, 0);

							if (ModelerApplication::Instance()->GetAnimate())
								glRotated(-20 * sin(pi), 0, 0, 1);
							else
								switch ((int)VAL(MOTION_SET)) {
									case(1):
										glRotated(10, 0, 0, 1);
										break;
									case(2):
										glRotated(20, 0, 0, 1);
										break;
									case(3):
										glRotated(-10, 0, 0, 1);
										break;
									default:
										glRotated(VAL(WING_B), 0, 0, 1);					// the rotation of wings
										break;
								}

							drawBox(8, 0.5f, 10);					// WING_5
							glPushMatrix();
								setDiffuseColor(COLOR_GREEN);
								glTranslated(8, 0.5f, 0);
								glRotated(180, 0, 0, 1);
								glRotated(-90, 1, 0, 0);
								drawRightAngledTrianglePrism(8, 4, 0.5f);			// WING_6

							glPopMatrix();
							glPushMatrix();
								glTranslated(8, 0, 10);
								glRotated(180, 0, 0, 1);
								glRotated(90, 1, 0, 0);
								drawRightAngledTrianglePrism(8, 3, 0.5f);			// WING_7

							glPopMatrix();
							glPushMatrix();
								setDiffuseColor(COLOR_RED);
								glTranslated(0, 0.25f, 10);
								glRotated(atan((double)8/3) / M_PI * 180, 0, 1, 0);
								drawCylinder(sqrt(73), 0.3f, 0.3f);					// WING_8

							glPopMatrix();
							glPushMatrix();
								// level 3

								glTranslated(8, 0.25f, -4);
								drawCylinder(17, 0.3f, 0.3f);				// WING_9
								glPushMatrix();
									glTranslated(0, 0, -1);
									drawCylinder(1, 0.1f, 0.5f);			// WING_10

								glPopMatrix();
								glPushMatrix();
									glTranslated(0, 0, 17);
									glRotated(60, -1, -1, 1);
									drawCylinder(2, 0.6f, 0.2f);			// WING_11

								glPopMatrix();
								setDiffuseColor(COLOR_BLUE);
								glTranslated(0, -0.25f, 4);

								if (ModelerApplication::Instance()->GetAnimate())
									glRotated(-30 * sin(pi), 0, 0, 1);
								else
									switch ((int)VAL(MOTION_SET)) {
										case(1):
											glRotated(15, 0, 0, 1);
											break;
										case(2):
											glRotated(30, 0, 0, 1);
											break;
										case(3):
											glRotated(-15, 0, 0, 1);
											break;
										default:
											glRotated(VAL(WING_C), 0, 0, 1);						// the rotation of wings
											break;
									}

								drawBox(10, 0.5f, 13);						// WING_12
								glPushMatrix();
									setDiffuseColor(COLOR_GREEN);
									glRotated(-90, 1, 0, 0);
									drawRightAngledTrianglePrism(5, 4, 0.5f);		// WING_13

								glPopMatrix();
								glPushMatrix();
									glTranslated(10, 0.5f, 0);
									glRotated(180, 0, 0, 1);
									glRotated(-90, 1, 0, 0);
									drawRightAngledTrianglePrism(5, 4, 0.5f);		// WING_14	

								glPopMatrix();
								glPushMatrix();
									setDiffuseColor(COLOR_RED);
									glTranslated(0, 0.25f, 13);
									glRotated(90, 0, 1, 0);
									drawCylinder(10, 0.3f, 0.3f);					// WING_15

								glPopMatrix();
								glPushMatrix();
									// level 4
						
									glTranslated(10, 0.25f, -4);
									drawCylinder(17, 0.3f, 0.3f);			// WING_16
									glPushMatrix();
										glTranslated(0, 0, -1);
										drawCylinder(1, 0.1f, 0.5f);		// WING_17

									glPopMatrix();
									setDiffuseColor(COLOR_BLUE);
									glTranslated(0, -0.25f, 4);

									if (ModelerApplication::Instance()->GetAnimate())
										glRotated(-20 * sin(pi), 0, 0, 1);
									else
										switch ((int)VAL(MOTION_SET)) {
											case(1):
												glRotated(20, 0, 0, 1);
												break;
											case(2):
												glRotated(-20, 0, 0, 1);
												break;
											case(3):
												glRotated(-25, 0, 0, 1);
												break;
											default:
												glRotated(VAL(WING_D), 0, 0, 1);				// the rotation of wings
												break;
										}

									drawBox(8, 0.5f, 11);					// WING_18
									glPushMatrix();
										setDiffuseColor(COLOR_GREEN);
										glRotated(-90, 1, 0, 0);
										drawRightAngledTrianglePrism(4, 4, 0.5f);		// WING_19

									glPopMatrix();
									glPushMatrix();
										glTranslated(8, 0.5f, 0);
										glRotated(180, 0, 0, 1);
										glRotated(-90, 1, 0, 0);
										drawRightAngledTrianglePrism(4, 5, 0.5f);		// WING_20

									glPopMatrix();
									glPushMatrix();
										glTranslated(0, 0.5f, 11);
										glRotated(90, 1, 0, 0);
										drawRightAngledTrianglePrism(8, 2, 0.5f);		// WING_21

									glPopMatrix();
									glPushMatrix();
										setDiffuseColor(COLOR_RED);
										glTranslated(0, 0.25f, 13);
										glRotated(180-atan((double)8/2) / M_PI * 180, 0, 1, 0);
										drawCylinder(sqrt(68), 0.3f, 0.3f);				// WING_22

									glPopMatrix();
									glPushMatrix();
										// level 5

										glTranslated(8, 0.25f, -5);
										drawCylinder(16, 0.3f, 0.3f);			// WING_23
										glPushMatrix();
											glTranslated(0, 0, -1);
											drawCylinder(1, 0.1f, 0.5f);		// WING_24

										glPopMatrix();
										setDiffuseColor(COLOR_BLUE);
										glTranslated(0, -0.25f, 5);

										if (ModelerApplication::Instance()->GetAnimate())
											glRotated(-20 * sin(pi), 0, 0, 1);
										else
											switch ((int)VAL(MOTION_SET)) {
												case(1):
													glRotated(20, 0, 0, 1);
													break;
												case(2):
													glRotated(-20, 0, 0, 1);
													break;
												case(3):
													glRotated(-25, 0, 0, 1);
													break;
												default:
													glRotated(VAL(WING_E), 0, 0, 1);				// the rotation of wings
													break;
											}

										drawBox(5, 0.5f, 8);					// WING_25
										glPushMatrix();
											setDiffuseColor(COLOR_GREEN);
											glRotated(-90, 1, 0, 0);
											drawRightAngledTrianglePrism(5, 5, 0.5f);		// WING_26

										glPopMatrix();
										glPushMatrix();
											glTranslated(0, 0.5f, 8);
											glRotated(90, 1, 0, 0);
											drawRightAngledTrianglePrism(5, 3, 0.5f);		// WING_27

										glPopMatrix();
										glPushMatrix();
											setDiffuseColor(COLOR_RED);
											glTranslated(0, 0.25f, 11);
											glRotated(180-atan((double)5/3) / M_PI * 180, 0, 1, 0);
											drawCylinder(sqrt(34), 0.3f, 0.3f);				// WING_28

										glPopMatrix();

										// end of level 5
									glPopMatrix();

									// end of level 4
								glPopMatrix();

								// end of level 3
							glPopMatrix();
				
							// end of level 2
						glPopMatrix();

						// end of level 1
					glPopMatrix();

					// draw right wing
					glPushMatrix();
						// level 1

						setDiffuseColor(COLOR_BLUE);
						glTranslated(1.25, 4.5, 7.5);

						if (ModelerApplication::Instance()->GetAnimate())
							glRotated(10 * sin(pi), 0, 0, 1);
						else
							switch ((int)VAL(MOTION_SET)) {
								case(1):
									glRotated(-5, 0, 0, 1);
									break;
								case(2):
									glRotated(-10, 0, 0, 1);
									break;
								case(3):
									glRotated(5, 0, 0, 1);
									break;
								default:
									glRotated(-1 * VAL(WING_A), 0, 0, 1);				// the rotation of wings
									break;
							}

						glTranslated(-6, 0, 0);
						drawBox(6, 0.5f, 5);					// WING_1
						glPushMatrix();
							setDiffuseColor(COLOR_GREEN);
							glTranslated(0, 0.5f, 5);
							glRotated(90, 1, 0, 0);
							drawRightAngledTrianglePrism(6, 5, 0.5f);		// WING_2

						glPopMatrix();
						glPushMatrix();
							setDiffuseColor(COLOR_RED);
							glTranslated(6, 0.25f, 5);
							glRotated(360 - atan((double)6 / 5) / M_PI * 180, 0, 1, 0);
							drawCylinder(sqrt(61), 0.3f, 0.3f);				// WING_3

						glPopMatrix();
						glPushMatrix();
							// level 2
				
							glTranslated(0, 0.25f, 0);
							drawCylinder(10, 0.3f, 0.3f);					// WING_4
							setDiffuseColor(COLOR_BLUE);
							glTranslated(0, -0.25f, 0);
							if (ModelerApplication::Instance()->GetAnimate())
								glRotated(10 * sin(pi), 0, 0, 1);
							else
								switch ((int)VAL(MOTION_SET)) {
									case(1):
										glRotated(-10, 0, 0, 1);
										break;
									case(2):
										glRotated(-20, 0, 0, 1);
										break;
									case(3):
										glRotated(10, 0, 0, 1);
										break;
									default:
										glRotated(-1 * VAL(WING_B), 0, 0, 1);						// the rotation of wings
										break;
								}

							glTranslated(-8, 0, 0);
							drawBox(8, 0.5f, 10);							// WING_5
							glPushMatrix();
								setDiffuseColor(COLOR_GREEN);
								glRotated(-90, 1, 0, 0);
								drawRightAngledTrianglePrism(8, 4, 0.5f);	// WING_6
				
							glPopMatrix();
								glPushMatrix();
								glTranslated(0, 0.5f, 10);
								glRotated(90, 1, 0, 0);
								drawRightAngledTrianglePrism(8, 3, 0.5f);	// WING_7

							glPopMatrix();
							glPushMatrix();
								setDiffuseColor(COLOR_RED);
								glTranslated(0, 0.25f, 13);
								glRotated(180 - atan((double)8 / 3) / M_PI * 180, 0, 1, 0);
								drawCylinder(sqrt(73), 0.3f, 0.3f);			// WING_8

							glPopMatrix();
							glPushMatrix();
								// level 3

								glTranslated(0, 0.25f, -4);
								drawCylinder(17, 0.3f, 0.3f);				// WING_9
								glPushMatrix();
									glTranslated(0, 0, -1);
									drawCylinder(1, 0.1f, 0.5f);			// WING_10

								glPopMatrix();
								glPushMatrix();
									glTranslated(0, 0, 17);
									glRotated(60, -1, 1, -1);
									drawCylinder(2, 0.6f, 0.2f);			// WING_11

								glPopMatrix();
								setDiffuseColor(COLOR_BLUE);
								glTranslated(0, -0.25f, 4);

								if (ModelerApplication::Instance()->GetAnimate())
									glRotated(30 * sin(pi), 0, 0, 1);
								else
									switch ((int)VAL(MOTION_SET)) {
										case(1):
											glRotated(-15, 0, 0, 1);
											break;
										case(2):
											glRotated(-30, 0, 0, 1);
											break;
										case(3):
											glRotated(15, 0, 0, 1);
											break;
										default:
											glRotated(-1 * VAL(WING_C), 0, 0, 1);					// the rotation of wings
											break;
									}

								glTranslated(-10, 0, 0);
								drawBox(10, 0.5f, 13);						// WING_12
								glPushMatrix();
									setDiffuseColor(COLOR_GREEN);
									glRotated(-90, 1, 0, 0);
									drawRightAngledTrianglePrism(5, 4, 0.5f);		// WING_14

								glPopMatrix();
								glPushMatrix();
									glTranslated(10, 0.5f, 0);
									glRotated(180, 0, 0, 1);
									glRotated(-90, 1, 0, 0);
									drawRightAngledTrianglePrism(5, 4, 0.5f);		// WING_13

								glPopMatrix();
								glPushMatrix();
									setDiffuseColor(COLOR_RED);
									glTranslated(0, 0.25f, 13);
									glRotated(90, 0, 1, 0);
									drawCylinder(10, 0.3f, 0.3f);					// WING_15

								glPopMatrix();
								glPushMatrix();
									// level 4

									glTranslated(0, 0.25f, -4);
									drawCylinder(17, 0.3f, 0.3f);					// WING_16
									glPushMatrix();
									glTranslated(0, 0, -1);
										drawCylinder(1, 0.1f, 0.5f);				// WING_17

									glPopMatrix();
									setDiffuseColor(COLOR_BLUE);
									glTranslated(0, -0.25f, 4);

									if (ModelerApplication::Instance()->GetAnimate())
										glRotated(20 * sin(pi), 0, 0, 1);
									else
										switch ((int)VAL(MOTION_SET)) {
											case(1):
												glRotated(-20, 0, 0, 1);
												break;
											case(2):
												glRotated(20, 0, 0, 1);
												break;
											case(3):
												glRotated(25, 0, 0, 1);
												break;
											default:
												glRotated(-1 * VAL(WING_D), 0, 0, 1);							// the rotation of wings
												break;
										}

									glTranslated(-8, 0, 0);
									drawBox(8, 0.5f, 11);							// WING_18
									glPushMatrix();
										setDiffuseColor(COLOR_GREEN);
										glTranslated(8, 0.5f, 0);
										glRotated(180, 0, 0, 1);
										glRotated(-90, 1, 0, 0);
										drawRightAngledTrianglePrism(4, 4, 0.5f);	// WING_19

									glPopMatrix();
									glPushMatrix();
										glRotated(-90, 1, 0, 0);
										drawRightAngledTrianglePrism(4, 5, 0.5f);	// WING_20

									glPopMatrix();
									glPushMatrix();
										glTranslated(8, 0, 11);
										glRotated(180, 0, 0, 1);
										glRotated(90, 1, 0, 0);
										drawRightAngledTrianglePrism(8, 2, 0.5f);	// WING_21

									glPopMatrix();
									glPushMatrix();
										setDiffuseColor(COLOR_RED);
										glTranslated(8, 0.25f, 13);
										glRotated(180 + atan((double)8 / 2) / M_PI * 180, 0, 1, 0);
										drawCylinder(sqrt(68), 0.3f, 0.3f);			// WING_22

									glPopMatrix();
									glPushMatrix();
										// level 5

										glTranslated(0, 0.25f, -5);
										drawCylinder(16, 0.3f, 0.3f);				// WING_23
										glPushMatrix();
											glTranslated(0, 0, -1);
											drawCylinder(1, 0.1f, 0.5f);			// WING_24

										glPopMatrix();
										setDiffuseColor(COLOR_BLUE);
										glTranslated(0, -0.25f, 5);

										if (ModelerApplication::Instance()->GetAnimate())
											glRotated(20 * sin(pi), 0, 0, 1);
										else
											switch ((int)VAL(MOTION_SET)) {
												case(1):
													glRotated(-20, 0, 0, 1);
													break;
												case(2):
													glRotated(20, 0, 0, 1);
													break;
												case(3):
													glRotated(25, 0, 0, 1);
													break;
												default:
													glRotated(-1 * VAL(WING_E), 0, 0, 1);						// the rotation of wings
													break;
											}

										glTranslated(-5, 0, 0);
										drawBox(5, 0.5f, 8);						// WING_25
										glPushMatrix();
											setDiffuseColor(COLOR_GREEN);
											glTranslated(5, 0.5f, 0);
											glRotated(180, 0, 0, 1);
											glRotated(-90, 1, 0, 0);
											drawRightAngledTrianglePrism(5, 5, 0.5f);		// WING_26

										glPopMatrix();
										glPushMatrix();
											glTranslated(5, 0, 8);
											glRotated(180, 0, 0, 1);
											glRotated(90, 1, 0, 0);
											drawRightAngledTrianglePrism(5, 3, 0.5f);		// WING_27

										glPopMatrix();
											glPushMatrix();
											setDiffuseColor(COLOR_RED);
											glTranslated(5, 0.25f, 11);
											glRotated(180 + atan((double)5 / 3) / M_PI * 180, 0, 1, 0);
											drawCylinder(sqrt(34), 0.3f, 0.3f);				// WING_28

										glPopMatrix();

									// end of level 5
									glPopMatrix();

								// end of level 4
								glPopMatrix();

							// end of level 3
							glPopMatrix();

						// end of level 2
						glPopMatrix();

					// end of level 1
					glPopMatrix();
				}

				if (VAL(LOD) >= 2) {
					// draw left front legs
					glPushMatrix();
						setDiffuseColor(COLOR_BLUE);
						glTranslated(3.75, 0, 13.5);
						glRotated(-30 - 7 * cos(pi), 1, 0, 0);
						drawBox(1.75, 1.75, -5.5);
						glPushMatrix();
							//
							setDiffuseColor(COLOR_BLUE);
							glTranslated(0.25, 0.25, -4.5);
							glRotated(20 + 10 * sin(pi), 1, 0, 0);
							drawBox(1.25, 1.25, -3.5);
							glPushMatrix();
								//
								setDiffuseColor(COLOR_BLUE);
								glTranslated(-0.25, -1, -4.5);
								glRotated(35 - 3 * cos(pi), 1, 0, 0);
								drawBox(1.75, 2.5, -1);

							glPopMatrix();
						glPopMatrix();
					glPopMatrix();

					// draw right front legs
					glPushMatrix();
						setDiffuseColor(COLOR_BLUE);
						glTranslated(-1, 0, 13.5);
						glRotated(-30 - 7 * cos(pi), 1, 0, 0);
						drawBox(1.75, 1.75, -5.5);
						glPushMatrix();
							//
							setDiffuseColor(COLOR_BLUE);
							glTranslated(0.25, 0.25, -4.5);
							glRotated(20 + 10 * sin(pi), 1, 0, 0);
							drawBox(1.25, 1.25, -3.5);
							glPushMatrix();
								//
								setDiffuseColor(COLOR_BLUE);
								glTranslated(-0.25, -1, -4.5);
								glRotated(35 - 3 * cos(pi), 1, 0, 0);
								drawBox(1.75, 2.5, -1);

							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				}

				if (VAL(LOD) >= 1) {
					// draw head
					glPushMatrix();
						// HEAD_7
						setDiffuseColor(COLOR_BLUE);
						glTranslated(1.125, 1.125, 15);
						// glRotated(thetaXA, 0, 1, 0);
						// glRotated(thetaZA, 1, 0, 0);
						glRotated(-3 + 3 * cos(pi), 1, 0, 0);
						drawBox(2.25, 2.25, 3);
						glPushMatrix();
							// HEAD_6
							setDiffuseColor(COLOR_RED);
							glTranslated(0.625, 0.625, 2);
							drawBox(1, 1, 2.5);
							glPushMatrix();
								// HEAD_5
								setDiffuseColor(COLOR_BLUE);
								glTranslated(-0.625, -0.625, 1.5);
								// glRotated(thetaXB, 0, 1, 0);
								// glRotated(thetaZB, 1, 0, 0);
								glRotated(-5 + 5 * sin(pi), 1, 0, 0);
								drawBox(2.25, 2.25, 3);
								glPushMatrix();
									// HEAD_4
									setDiffuseColor(COLOR_RED);
									glTranslated(0.625, 0.625, 2);
									drawBox(1, 1, 2.5);
									glPushMatrix();
										// HEAD_3
										setDiffuseColor(COLOR_BLUE);
										glTranslated(-0.625, -0.625, 1.5);
										// glRotated(thetaXC, 0, 1, 0);
										// glRotated(thetaZC, 1, 0, 0);
										glRotated(-5 + 5 * cos(pi), 1, 0, 0);
										drawBox(2.25, 2.25, 3);
										glPushMatrix();
											// HEAD_2
											setDiffuseColor(COLOR_RED);
											glTranslated(0.625, 0.625, 2);
											glPushMatrix();
												// HEAD_1
												setDiffuseColor(COLOR_GREEN);
												glTranslated(0.5, 0.5, 2.5);
												glRotated(10 + 10 * cos(pi), 1, 0, 0);
												drawSphere(3);
												glPushMatrix();
													// mouth and eyes
													glPushMatrix();
														// lower mouth
														setDiffuseColor(COLOR_BLUE);
														glTranslated(-2.5f, -1, 1);
														glRotated(10 * cos(pi), 1, 0, 0);
														drawBox(5, 0.5f, 5);
														glTranslated(0.5f, 0.25f, 0);
														setDiffuseColor(COLOR_RED);
														drawBox(4, 0.5f, 4);
														// end of lower mouth
													glPopMatrix();
													glPushMatrix();
														// upper mouth
														setDiffuseColor(COLOR_BLUE);
														glTranslated(-2.5f, 0, 1);
														glRotated(-10, 1, 0, 0);
														drawBox(5, 0.5f, 5);
														// end of upper mouth
													glPopMatrix();
													glPushMatrix();
														// eyes
														glPushMatrix();
															// right eye
															setDiffuseColor(COLOR_RED);
															glTranslated(-2.5f, 1, 1);
															drawSphere(1);
															// end of right eye
														glPopMatrix();
														glPushMatrix();
															// left eye
															setDiffuseColor(COLOR_RED);
															glTranslated(2.5f, 1, 1);
															drawSphere(1);
															// end of left eye
														glPopMatrix();
														// end of eyes
													glPopMatrix();
													// end of mouth and eyes
												glPopMatrix();
												if (VAL(WIDGETS) > 0) {
													glPushMatrix();
														glTranslated(0, 4, 0);
														drawTorus(3, 1);
													glPopMatrix();
												}
												if (VAL(WIDGETS) == 2) {
													glPushMatrix();
														glTranslated(0, 0, 6);
														glRotated(90, 1, 0, 0);
														drawStar(3, 45, 45);
													glPopMatrix();
												}
											glPopMatrix();
										glPopMatrix();
									glPopMatrix();
								glPopMatrix();
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				}

			glPopMatrix();
		glPopMatrix();
	/*}
	else if (VAL(DISPLAY_LSYSTEM) == 1) {
		if ((int)VAL(LSYSTEM_OBJECT_TYPE) == 0) {
			std::string axiom = "F";

			int numRules = 1;
			Rule rule1 = { 'F', "FF+[+F-F-F]-[-F+F+F]" };

			Rule* rules = new Rule[numRules];
			rules[0] = rule1;

			LSystem lsystem(axiom, rules, numRules);
			lsystem.draw((int)VAL(LSYSTEM_ITERATION));
		}
		else if ((int)VAL(LSYSTEM_OBJECT_TYPE) == 1) {
			std::string axiom = "F+F+F+F";

			int numRules = 1;
			Rule rule1 = { 'F', "FF+F-F+F+FF" };

			Rule* rules = new Rule[numRules];
			rules[0] = rule1;

			LSystem lsystem(axiom, rules, numRules);
			lsystem.draw((int)VAL(LSYSTEM_ITERATION));
		}
		else if ((int)VAL(LSYSTEM_OBJECT_TYPE) == 2) {
			std::string axiom = "Y";

			int numRules = 2;
			Rule rule1 = { 'X', "X[-FFF][+FFF]FX" };
			Rule rule2 = { 'Y', "YFX[+Y][-Y]" };

			Rule* rules = new Rule[numRules];
			rules[0] = rule1;
			rules[1] = rule2;

			LSystem lsystem(axiom, rules, numRules);
			lsystem.draw((int)VAL(LSYSTEM_ITERATION));
		}
		else if ((int)VAL(LSYSTEM_OBJECT_TYPE) == 3) {
			std::string axiom = "F";

			int numRules = 1;
			Rule rule1 = { 'F', "F[+FF][-FF]F[-F][+F]F" };

			Rule* rules = new Rule[numRules];
			rules[0] = rule1;

			LSystem lsystem(axiom, rules, numRules);
			lsystem.draw((int)VAL(LSYSTEM_ITERATION));
		}
		else if ((int)VAL(LSYSTEM_OBJECT_TYPE) == 4) {
			std::string axiom = "F+F+F";

			int numRules = 1;
			Rule rule1 = { 'F', "F-F+F" };

			Rule* rules = new Rule[numRules];
			rules[0] = rule1;

			LSystem lsystem(axiom, rules, numRules);
			lsystem.draw((int)VAL(LSYSTEM_ITERATION));
		}
		else if ((int)VAL(LSYSTEM_OBJECT_TYPE) == 5) {
			std::string axiom = "-YF";

			int numRules = 2;
			Rule rule1 = { 'X', "XFX-YF-YF+FX+FX-YF-YFFX+YF+FXFXYF-FX+YF+FXFX+YF-FXYF-YF-FX+FX+YFYF-" };
			Rule rule2 = { 'Y', "+FXFX-YF-YF+FX+FXYF+FX-YFYF-FX-YF+FXYFYF-FX-YFFX+FX+YF-YF-FX+FX+YFY" };

			Rule* rules = new Rule[numRules];
			rules[0] = rule1;
			rules[1] = rule2;

			LSystem lsystem(axiom, rules, numRules);
			lsystem.draw((int)VAL(LSYSTEM_ITERATION));
		}
		else if ((int)VAL(LSYSTEM_OBJECT_TYPE) == 6) {
			std::string axiom = "F+F+F+F";

			int numRules = 2;
			Rule rule1 = { 'F', "FF+F++F+F" };

			Rule* rules = new Rule[numRules];
			rules[0] = rule1;

			LSystem lsystem(axiom, rules, numRules);
			lsystem.draw((int)VAL(LSYSTEM_ITERATION));
		}
	}*/

}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];
	controls[LOD] = ModelerControl("Levels of Detail", 0, 5, 1, 5);
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.01f, 0.0f);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.01f, 0.0f);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.01f, 0.0f);
	controls[WING_A] = ModelerControl("WING_A", -90, 90, 0.01f, 0.0f);
	controls[WING_B] = ModelerControl("WING_B", -90, 90, 0.01f, 0.0f);
	controls[WING_C] = ModelerControl("WING_C", -90, 90, 0.01f, 0.0f);
	controls[WING_D] = ModelerControl("WING_D", -90, 90, 0.01f, 0.0f);
	controls[WING_E] = ModelerControl("WING_E", -90, 90, 0.01f, 0.0f);
	controls[MOTION_SET] = ModelerControl("Different set of motion", 0, 3, 1, 0);
	controls[LIGHT0_X] = ModelerControl("Light0 X Position", -30, 30, 0.01f, 4.0f);
	controls[LIGHT0_Y] = ModelerControl("Light0 Y Position", -30, 30, 0.01f, 2.0f);
	controls[LIGHT0_Z] = ModelerControl("Light0 Z Position", -30, 30, 0.01f, -4.0f);
	controls[LIGHT0_INTENSITY] = ModelerControl("Light0 Intensity", 0, 1, 0.01f, 1.0f);
	controls[LIGHT1_X] = ModelerControl("Light1 X Position", -30, 30, 0.01f, -2.0f);
	controls[LIGHT1_Y] = ModelerControl("Light1 Y Position", -30, 30, 0.01f, 1.0f);
	controls[LIGHT1_Z] = ModelerControl("Light1 Z Position", -30, 30, 0.01f, 5.0f);
	controls[LIGHT1_INTENSITY] = ModelerControl("Light1 Intensity", 0, 1, 0.01f, 1.0f);
	controls[WIDGETS] = ModelerControl("Widgets", 0, 2, 1, 0);
	/*
	controls[INVERSEKINEMATICSX] = ModelerControl("Required x position of the head", -2, 2, 1, 0.0f);
	controls[INVERSEKINEMATICSY] = ModelerControl("Required y position of the head", 5.25, 9.25, 0.25, 7.25f);
	controls[INVERSEKINEMATICSZ] = ModelerControl("Required z position of the head", 16, 20, 1, 18);
	controls[DISPLAY_LSYSTEM] = ModelerControl("Display L-system", 0, 1, 1, 0);
	controls[LSYSTEM_OBJECT_TYPE] = ModelerControl("L-system: Object type", 0, 6, 1, 0);
	controls[LSYSTEM_LENGTH_FACTOR] = ModelerControl("L-system: Length factor", 0, 5, 0.01f, 0.3f);
	controls[LSYSTEM_INITIAL_ANGLE] = ModelerControl("L-system: Initial angle", 0, 360, 1, 0);
	controls[LSYSTEM_INCREMENT_ANGLE] = ModelerControl("L-system: Angle of increment", 0, 360, 1, 30);
	controls[LSYSTEM_ITERATION] = ModelerControl("L-system: Iteration", 0, 6, 1, 3);
	*/

	ModelerApplication::Instance()->Init(&createOurModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}

