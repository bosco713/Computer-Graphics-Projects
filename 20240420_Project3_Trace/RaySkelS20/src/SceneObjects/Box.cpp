#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	double T_near = -1e5;
	double T_far = 1e5;
	int T_corresponding_axis = 0;
	const vec3f& positionVector = r.getPosition();			// Xo
	const vec3f& directionVector = r.getDirection();		// Xd

	// I am not sure about this part
	// BoundingBox boundingBox = ComputeLocalBoundingBox();
	// const BoundingBox& boundingBox = this->getBoundingBox();
	// const vec3f& boxH = boundingBox.max;
	// const vec3f& boxI = boundingBox.min;
	vec3f boxH = vec3f(0.5, 0.5, 0.5);				// Xh
	vec3f boxI = vec3f(-0.5, -0.5, -0.5);			// Xi
	// vec3f intersectionNear = vec3f(0, 0, 0);
	// 

	// For x plane:
	for (int i = 0; i < 3; ++i) {
		if (directionVector[i] <= RAY_EPSILON && directionVector[i] >= -RAY_EPSILON) {
			if (positionVector[i] < boxI[i] || positionVector[i] > boxH[i]) {
				// printf("KILLED\n");
				return false;
			}
		}
		else {
			double T1 = (boxI[i] - positionVector[i]) / directionVector[i];
			double T2 = (boxH[i] - positionVector[i]) / directionVector[i];
			if (T1 > T2) {		// do swapping	// since T1 interseaction with near plane
				double temp = T2;
				T2 = T1;
				T1 = temp;
			}
			if (T1 > T_near) {	// want largest T_near
				T_near = T1;
				T_corresponding_axis = i;
			}
			if (T2 < T_far) {	// want smallest T_far
				T_far = T2;
			}
			if (T_near > T_far) {	// box is missed
				return false;
			}
			if (T_far <= RAY_EPSILON) {	// box is behind ray 
				return false;
			}
			// intersectionNear[i] = T_near;
		}
	}
	i.obj = this;
	i.t = T_near;
	switch(T_corresponding_axis) {
		default:
			if (directionVector[0] < RAY_EPSILON){
				i.N = vec3f(1.0, 0.0, 0.0);
			} else {
				i.N = vec3f(-1.0, 0.0, 0.0);
			}
			break;
		case (1):
			if (directionVector[1] < RAY_EPSILON){
				i.N = vec3f(0.0, 1.0, 0.0);
			} else {
				i.N = vec3f(0.0, -1.0, 0.0);
			}
			break;
		case (2):
			if (directionVector[2] < RAY_EPSILON){
				i.N = vec3f(0.0, 0.0, 1.0);
			} else {
				i.N = vec3f(0.0, 0.0, -1.0);
			}
			break;
	}
	// printf("The box survive!\n");
	return true;
}
