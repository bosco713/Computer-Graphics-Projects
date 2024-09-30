# COMP4411_Project_3

## Reference : [Project 3 : Trace](https://course.cse.ust.hk/comp4411/Password_Only/projects/trace/index.html)

## Basic requirement:
Your ray tracer should recursively trace rays to account for these. Recursion should proceed to a maximum depth as set by the user.
1. Box intersections:
    - [x] Implement box intersections
    - [x] Expand ray-casting into ray-tracing by adding support for reflected and refracted rays
2. Phong specular-reflection model:
    - [x] Implement the Phong specular-reflection model
    - [x] Include support for transparent objects
3. Contribution from: multiple light sources, distance attenuation, and these additional types of lights: 
    - [x] Implement directional lights
    - [x] Implement point lights
4. Other extensions: 
    - [x] Implement shadow attenuation
    - [x] Implement reflection
    - [x] Implement refraction

## Bonus 

1. Implement an adaptive termination criterion for tracing rays, based on ray contribution. [1W]
    - [x] Control the adaptation threshold with a slider.
2. Implement antialiasing by supersampling and averaging down. [1W]
    - [ ] You must provide a slider that indicates the number of sub-pixel samples being used (e.g., something in the range 1-5 indicating 1x1, 2x2, ... 5x5 supersampling). This whistle upgrades to a bell if you implement the first stage of distribution ray tracing by jittering the sub-pixel samples.  The noise introduced by jittering should be evident when casting 1 ray per pixel.
3. Implement spot lights [1B]
    - [ ] Done
4. Specify a background image to replace the environment's ambient color during the rendering.  That is, any ray that goes off into infinity behind the scene should return a color from the loaded image, instead of just black.  The background should appear as the backplane of the rendered image with suitable reflections and refractions to it. [1B] 
    - [ ] Add a menu option about this
5. Find a good way to accelerate shoadow attenuation [1B]
    - [ ] Implement it
    - [ ] Know how to demonstrate and explain it
6. Deal with overlapping objects intelligently.  While the skeleton code handles materials with arbitrary indices of refraction, it assumes that objects don't intersect one another. It breaks down when objects intersect or are wholly contained inside other objects. [1B]
    - [ ] Add support to the refraction code for detecting this and handling it in a more realistic fashion.
    Note, however, that in the real world, objects can't coexist in the same place at the same time. You will have to make assumptions as to how to choose the index of refraction in the overlapping space.  Make those assumptions clear when demonstrating the results.
7. Add texture mapping support to the program. The most basic kind of texture mapping is to apply the map to the diffuse color of a surface. But many other parameters can be mapped. [1B1W]
    - [ ] Reflected color can be mapped to create the sense of a surrounding environment.
    - [ ] Transparency can be mapped to create holes in objects. Additional (variable) extra credit will be given for such additional mappings.
    - [ ] Modify the parser for handling texture mapping inputs the types of mapping mentioned above.  
    Additional credit will be awarded for quality implementation of texture mapping on general trimeshes.
8. Implement antialiasing by adaptive supersampling, as described in Foley, et al., 15.10.4.[1B1W]
    - [ ] Show some sort of visualization of the sampling pattern that results
    For example, you could create another image where each pixel is given an intensity proportional to the number of rays used to calculate the color of the corresponding pixel in the ray traced image.
9. Implement more versatile lighting controls, such as the Warn model described in Foley 16.1.5. [1B1W]
    - [ ] Control the shape of the projected light.
10. Implement bump mapping (Watt 8.4; Foley, et al. 16.3.3). [2B]
    - [ ] done
11. Implement solid textures or some other form of procedural texture mapping, as described in Foley, et al., 20.1.2 and 20.8.3. [2B]
    - [ ] Solid textures are a way to easily generate a semi-random texture like wood grain or marble.
12. Add some new types of geometry to the ray tracer. [2B]
    - [ ] Implementing torii or general quadrics. Many other objects are possible here.
13. Add support for height-fields [2B] 
    - [ ] done
14. For each additional[B&W], implement distribution ray tracing to produce one or more or the following effects (See Watt 10.6, Glassner, chapter 5, or Foley, et al., 16.12.4). [2B]
    - [ ] Depth of field, soft shadows
    - [ ] Motion blur
    - [ ] Glossy reflection
15. Add some higher-level geometry to the ray tracer, such as surfaces of revolution, extrusions, metaballs or blend surfaces. [3B] 
    - [ ] Be sure you are actually raytracing the surface as a mathematical construct, not just creating a polygonal representation of the object and tracing that.
16. Implement ray-intersection optimization [3B] 
    - [ ] The BSP Tree implemented
    - [ ] A different optimization method
17. Implement a more realistic shading model. Credit will vary depending on the sophistication of the model. [4B]
    - [ ] A simple model factors in the Fresnel term to compute the amount of light reflected and transmitted at a perfect dielectric (e.g., glass). 
    - [ ] A more complex model incorporates the notion of a microfacet distribution to broaden the specular highlight. Accounting for the color dependence in the Fresnel term permits a more metallic appearance.
    - [ ] Include anisotropic reflections for a plane with parallel grains or a sphere with grains that follow the lines of latitude or longitude.
    Sources: Watt, Chapter 7, Foley et al, Section 16.7; Glassner, Chapter 4, Section 4; Ward's SIGGRAPH '92 paper; Schlick's Eurographics Rendering Workshop '93 paper.
    - [ ] Invent new shading models.
    - [ ] Implement a shading model that produces something that looks like cel animation
18. Implement CSG, constructive solid geometry. [4B]
    - [ ] This extension allows you to create very interesting models. 
    See page 108 of Glassner for some implementation suggestions.
19. Add a particle systems simulation and renderer (Foley 20.5, Watt 17.7). [4B]
    - [ ] done
20. Implement caustics.  Caustics are variations in light intensity caused by refractive focusing--everything from simple magnifying-glass points to the shifting patterns on the bottom of a swimming pool. [4B]
    - [ ] done
