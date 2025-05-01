/*==================================================================================
* COSC 363  Computer Graphics
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf   for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Cone.h"

#include <GL/freeglut.h>
using namespace std;

const float EDIST = 10.0;
const int NUMDIV = 500;
const int MAX_STEPS = 10;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;

vector<SceneObject*> sceneObjects;


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step) {
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(0, 15, 13);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

	ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
	if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found
	glm::vec3 hitPoint = ray.hit;
	glm::vec3 normal = obj->normal(hitPoint);
	glm::vec3 viewVec = -ray.dir;
	//////////////////shadows//////////////////
	color = obj->lighting(lightPos ,-ray.dir, ray.hit);	//Object's lighting

	glm::vec3 lightVec = lightPos - ray.hit;// add shadows 
	Ray shadowRay(ray.hit, lightVec);
	
	float lightDist = glm::length(lightVec); //distance from the light to the hit point
	shadowRay.closestPt(sceneObjects); //the first object(ball) hit by the shadow ray
	if(shadowRay.index > -1 && shadowRay.dist < lightDist){
		color = 0.2f * obj->getColor(); //0.2 = ambient scale factor
	}

	//////////////////reflection////////////////////////
	if (step < MAX_STEPS && obj->isReflective()) {
		glm::vec3 reflectionDir = glm::reflect(ray.dir, normal);
		Ray reflectionRay(hitPoint + 0.01f * reflectionDir, reflectionDir);
		glm::vec3 reflectionColor = trace(reflectionRay, step + 1);

		float reflCoeff = obj->getReflectionCoeff();
		color = (1.0f - reflCoeff) * color + reflCoeff * reflectionColor;
	}

	//////////////////Transparency////////////////////////
	if (step < MAX_STEPS && obj->isTransparent()) {
		Ray gray = Ray(hitPoint, ray.dir);
		gray.closestPt(sceneObjects);
		Ray hray = Ray(gray.hit,gray.dir);
		hray.closestPt(sceneObjects);
		float tranCoeff = obj->getTransparencyCoeff();
		if (hray.index > -1) {
			SceneObject* behindobj = sceneObjects[hray.index];
			color = tranCoeff * color + (1.0f - tranCoeff) * behindobj->getColor();
		}
	}


	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display() {
	float xp, yp;  //grid point
	float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
	float cellY = (YMAX - YMIN) / NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for (int i = 0; i < NUMDIV; i++) {	//Scan every cell of the image plane
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++) {
			yp = YMIN + j * cellY;

			glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray

			Ray ray = Ray(eye, dir);

			glm::vec3 col = trace(ray, 1); //Trace the primary ray and get the colour value
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);
		}
	}

	glEnd();
	glFlush();
}

//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize() {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

	glClearColor(0, 0, 0, 1);

	Plane* plane1 = new Plane(glm::vec3(0, -20, 0), glm::vec3(0, 1, 0),150.,150.); //Plane at y=-10
	plane1->setColor(glm::vec3(1, 1, 0)); //Set colour to yellow
	sceneObjects.push_back(plane1);
	Plane* plane2 = new Plane(glm::vec3(0, 20, 0), glm::vec3(0, -1, 0),150., 150.); //Plane at y=10(noraml filp)
	plane2->setColor(glm::vec3(1, 0, 0)); //Set colour to green
	sceneObjects.push_back(plane2);
	Plane* plane3 = new Plane(glm::vec3(-25, 0, 0), glm::vec3(1, 0, 0),150., 150.); //Plane at x=-10
	plane3->setColor(glm::vec3(0, 0, 1)); //Set colour to blue
	sceneObjects.push_back(plane3);
	Plane* plane4 = new Plane(glm::vec3(25, 0, 0), glm::vec3(-1, 0, 0), 150., 150.); //Plane at x=-10(noraml filp)
	plane4->setColor(glm::vec3(0, 1, 0)); //Set colour to green
	sceneObjects.push_back(plane4);
	Plane* plane5 = new Plane(glm::vec3(0, 0, -40), glm::vec3(0, 0, 1), 150., 150.); //Plane at z=-10
	plane5->setColor(glm::vec3(0, 1, 1)); //Set colour to cyne
	sceneObjects.push_back(plane5);
	plane5->setSpecularity(false);
	Plane* plane6 = new Plane(glm::vec3(0, 0, 50), glm::vec3(0, 0, -1), 150., 150.); //Plane at z=-50
	plane6->setColor(glm::vec3(1, 1, 1));//Set colour to white
	//not visible, behind eye
	sceneObjects.push_back(plane6);
	//This Plane is a mirror
	Plane* mirrorPlane = new Plane(glm::vec3(0, 10, -40), glm::normalize(glm::vec3(0, -0.8, 1)),40.,40.);
	mirrorPlane->setColor(glm::vec3(0));                 
	mirrorPlane->setReflectivity(true, 1.0);           
	sceneObjects.push_back(mirrorPlane);


	Sphere *sphere1 = new Sphere(glm::vec3(-10.0, -8.0, -25.0), 3);
	sphere1->setColor(glm::vec3(0, 0, 1));  //Set colour to blue
	sphere1->setTransparency(true,0.3);
	sceneObjects.push_back(sphere1);	

	//sphere1->setSpecularity(false); 
	//sphere1->setShininess(5); //Set shininess to 5

	Sphere* sphere2 = new Sphere(glm::vec3(0.0, -8.0, -25.0), 3);
	sphere2->setColor(glm::vec3(1, 0, 0));   //Set colour to red
	sceneObjects.push_back(sphere2);		 //Add sphere to scene objects

	Sphere* sphere3 = new Sphere(glm::vec3(10.0, -8.0, -25.0), 3);
	sphere3->setColor(glm::vec3(0, 1, 1));   //Set colour to cyan
	sceneObjects.push_back(sphere3);		 //Add sphere to scene objects

	Sphere* sphere4 = new Sphere(glm::vec3(10.0, -20.0, -20.0), 1);
	sphere4->setColor(glm::vec3(1, 1, 1));   //Set colour to white
	sceneObjects.push_back(sphere4);		 //Add sphere to scene objects
	
	Cylinder* cylinder1 = new Cylinder(glm::vec3(10.0, -20.0, -25.0), 2.5, 8.);
	cylinder1->setColor(glm::vec3(0, 0, 0));
	cylinder1->setReflectivity(true, 1);
	sceneObjects.push_back(cylinder1);

	Cylinder* cylinder2 = new Cylinder(glm::vec3(-10.0, -20.0, -25.0), 2.5, 8.);
	cylinder2->setColor(glm::vec3(0.5, 0.5, 1));
	sceneObjects.push_back(cylinder2);

	Cone* cone1 = new Cone(glm::vec3(0.0, -20.0, -25.0), 2.5, 8.);
	cone1->setColor(glm::vec3(1, 0.5, 0.5));
	sceneObjects.push_back(cone1);



}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Raytracing");

	glutDisplayFunc(display);
	initialize();

	glutMainLoop();
	return 0;
}
