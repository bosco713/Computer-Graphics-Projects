# COMP4411_Project_2

## Reference : [Project 2 : Modeler](https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/index.html)

## Sample executable [exe.](https://github.com/bosco713/COMP4411_Project_2/blob/main/modeler.exe)

## Sample modeler [example](https://github.com/bosco713/COMP4411_Project_2/blob/main/sample_modeler_sp02.exe)

## Basic requirements

1. Implement at least ten primitive shapes:
   - [x] done
2. Implement at least four levels of hierarchy:
   - [x] done
3. Use at least one of the following functions:
   - [x] glTranslate()
   - [x] glRotate()
   - [x] glScale()
4. Use these three functions:
   - [x] glPushMatrix()
   - [x] glPopMatrix()
5. Implement our own version of gluLookat()
   - [x] void Camera::lookAt(double *eye, double *at, double *up)
6. At least one of your controls must be tied to more than one joint/component
   - [x] this knob will change the input to some function that determines how several parts of your model can move.
7. Finish the model
   - [x] :)
8. Finish two bells
   - [x] #10. drawTorus(double radius, double thickness)

## Bonus

1. Change the default light source [1W]
   - [x] Illuminate your scene more dramatically
2. Allow for adjustable levels of detail for character [1W]
   - [x] Add a UI control to control the adjustable levels
3. Come up with another whistle and implement it [1W]
   - [ ] Extends the use of one of the things in an interesting way
         - (Describe here)
4. Use a texture map on all or part of your character [1B]
   - [ ] Implement your own primitives inside your model file that do texture mapping
5. Build a complex shape as a set of ppolygonal faces [1B]
   - [x] Build the shape and use the "triangle" primitive to render them
6. Make an additional "animated" sequence your character can perform [1B]
   - [x] Increment values each time your model's draw() function is called
7. Add some widgets that control adjustable parameters to your model [1B]
   - [x] You can then create individual-looking instances of your character
8. Add a "Frame All" feature that would change the position of the camera [1B]
   - [x] Your model is entirely visible in the model window, regardless of the pose and location of your model
9. The camera code has a constrained up vector [1B, 1W]
   - [x] Handle camera twists (the framework is already there)
   - [x] Come up with an intuitive way for the user to twist the camera via mouse control (ctrl + mouse drag)
10. Add a function in your model file for drawing a new type of primitive [2B]
      - [ ] Extruded surface: given two curves, create a surface by sweeping one curve along the path defined by the other curve
      - [ ] Surfaces of rotation: given a curve and a rotation axis, sweep out a surface by rotating the curve
      - [x] Torus
11. Use some sort of procedural modeling to generate all or part of your character [2B]
      - [ ] Have parameters of the procedural modeler controllable by the user via control widgets
12. Make your character react differently to UI controls[2B]
      - [x] Make your character pose like (something) --> make it like story telling 
13. Implement your hierarchical model and "skin" it with mataballs [4B]
      - [ ] Make it like organic shapes like human arms and shw the bending of the skins
            - Hint: look up "marching cubes" and "marching tetrahedra" --these are two commonly used algorithms for volume rendering.
14. Implement a method to move the model using a set of position constraints [8B]
      - [ ] Move the hand to a certain coordinate, we might have to change the shoulder angle, elbow angle -- maybe even the angle of the knees if the feet are constrained to one position.
      - [ ] (Additional 4B) Make sure that all angle constraints are satisfied as well. In your model, for instance, you might have a slider that constrains the elbow angle between 30 and 180 degrees.
