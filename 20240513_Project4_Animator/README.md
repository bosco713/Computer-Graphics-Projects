# COMP4411_Project_4

## Reference : [Project 4 : Animator](https://course.cse.ust.hk/comp4411/Password_Only/projects/animator03/index.html)

## Basic requirement:
1. Implement the following curve types with support for wrapping:
   - [x] Bézier (splined together with C0 continuity)
   - [x] B-spline
   - [x] Catmull-Rom
2. Implement a particle system that is integrated with your model in some way.
   - [x] done
  
## Bonus
1. Enhance the required spline options. [1W - max 3W]
     - [ ] Let the user control the tension of the Catmull-Rom spline
     - [ ] Implement higher degree polynomial splines (ones that are C3 or C4 continuous)
     - [ ] Implement one of the standard subdivision curves (e.g., Lane-Riesenfeld or Dyn-Levin-Gregory).
     - [ ] Allow the user to specify the derivatives at the two endpoints of your C2 interpolating curves.
     - [ ] Add options to the user interface to enforce C0 or C1 continuity between adjacent Bézier curve segments automatically. (It should also be possible to override this feature in cases where you don't want this type of continuity.)
     - [ ] [obsolete starting Spring 2024] Add the ability to add a new control point to any curve type without changing the curve at all.
2. Extends your project so that it support the use of the transparent TGA files. [1W]
     - [ ] done
3. Render a mirror in your scene. [1W]
     - [ ] Reflect the world about the mirror's plane
     - [ ] Draw the reflected world
     - [ ] Pop the reflection about the mirror plane from your matrix stack
     - [ ] Draw your world as normal.
     - [ ] Clip the reflected image to the mirror's surface using stencil buffer. [Extra 1W]
4. Achieve more spectacular effects with a simple technique called billboarding. [1W]
     - [ ] Load a texture with transparency and turn on alpha blending using billboarding.
5. Use the billboarded quads you implemented above to render the following effects. [1W per item]
     - [ ] Fire (You'll probably want to use additive blending for your particles - glBlendFunc(GL_SRC_ALPHA,GL_ONE);)
     - [ ] Snow
     - [ ] Water fountain
     - [ ] Fireworks
6. Use environment mapping to simulate a reflective material. [1W]
     - [ ] Find the functions to particularly effective at faking a metallic material or reflective, rippling water surface.
     - [ ] Understand how they work.
     - [ ] Use them.
7. Implement a motion blur effect. [1W]
     - [ ] Simulate an accumlation buffer by rendering individual frames to a texture
     - [ ] Combine those textures
8. Implment a skyboxes for rendering background of scene. [1W]
     - [ ] Project images of the environment onto the inside of a box which surrounds the viewing position.
9. Implement your simulation engine using a higher-order method. [1W]
     - [ ] Runge-Kutta technique.
10. Use openGL  display lists to speed up the rendering of one or more of your more complicated models. [1W]
     - [ ] Display lists allow you to render complicated polygons much more quickly by storing more information directly on the graphics card.
11. Implement adaptive Bézier curve generation. [1W]
     - [ ] Use a recursive, divide-and-conquer, de Casteljau algorithm to produce Bézier curves
     - [ ] Provide some way to change these variables, with a keystroke or mouse click.
     - [ ] Have some way of showing (a printf statement is fine) the number of points generated for a curve to demonstrate your adaptive algorithm at work.
     - [ ] Provide visual controls to toggle the feature, modify the flatness parameter (with a slider for e.g.) and show the number of points generated for each curve [Extra 1W]
12. Extend the particle system to handle springs [1B]
     - [ ] Simulated with a simple spring system with one spring endpoint
     - [ ] Implement spring-based cloth. [Extra 1B]
     - [ ] Implement spring-based fur.  The fur must respond to collisions with other geometry and interact with at least two forces like wind and gravity. [Extra 2B]
13. Allow for particles to bounce off each other by detecting collisions when updating their positions and velocities [1B]
     - [ ] done
14. Implement a "general" subdivision curve, so the user can specify and arbitary averaging mask. [1B]
     - [ ] done
     - [ ] Generate, display, and apply the evaluation masks. [Extra cridit?]
15. Implement a great way to build complicated looking maps and terrains pretty easily. [1B]
     - [ ] Heightfields
16. Extend your metaballs so that it support environment mapping. [1+3B(We haven't done it in project2/3)]
     - [ ] done
17. Add a lens flare. [1B1W]
     - [ ] Have at least 5 flare "drops"
     - [ ] Have transparency of the drops should change depending on how far the light source is from the center of the screen.  
     - [ ] Handle the case where the light source is occluded by other geometry. [Extra 1W]
18. Improvement the interface of animator. [1B1W]
     - [ ] done
19. Add flocking behaviors to your particles to simulate creatures moving in flocks, herds, or schools. [2B]
     - [ ] Use a model for your creatures that makes it easy to see their direction and orientation.
     - [ ] Make realistic creature model and have it move realistically according to its motion path. [Extra 1B]
20. Implement a C^2 - interpolating curve. [2B]
     - [ ] done (use the entry for it in the drop-down menu)
21. Add the ability to edit Catmull-Rom curves using the two "inner" Bézier control points as "handles" on the interpolated "outer" Catmull-Rom control points. [2B]
     - [ ] done
22. Implement picking of a part in the model hierarchy.  In other words, make it so that you can click on a part of your model to select its animation curve. [2B]
     - [ ] done
23. Fix the twist in your modeler [2B]
     - [ ] Implement Quaternions as a method for avoding the gimbal lock.
24. Extend the basic requirements so that it can draw bspline surface in the 3D scene. [2B]
     - [ ] Allow user to adjust the control points
25. Implement projected textures. [3B]
     - [ ] Use to sumulate things like a slide projector, spotlight illumination, or casting shadows onto arbitary geometry.
26. Expends the animator to support motion editing. [3B]
     - [ ] Motion warping
27. Implement Cell Shading, also know as Cartoon Rendering. [3B]
     - [ ] done
28. Incorporate regid-body simulations into your program, so that you can correctly simulate collisions and response between rigid objects in your scene. [4B]
     - [ ] Specify a set of objects in your model to be included in the simulation.
     - [ ] User should have the ability to enable and disable the simulation either using the existing "Simulate" button, or with a new button.   
29. Extend your system to support subdivision surfaces. [4B]
     - [ ] Provide a simple interface for the user to edit a surface.
     - [ ] User should also be able to specify surface features that stays constant so that sharp creases can be formed
     - [ ] Tie your surface to the animation curves to demonstrate a dynamic scene.
Disclaimer: please consult the course staff before spending any serious time on these. They are quite difficult, and credit can vary depending on the quality of your method and implementation.
30. Implement Inverse kinematics [8/9B?]
     - [ ] done
31. Implement Interactive Control of Physically-Based Animation [8/9B?]
     - [ ] done
