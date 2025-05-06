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
#include "TextureOpenIL.h" 
#include <glm/gtc/random.hpp>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>



using namespace std;

const float EDIST = 10.0;
const int NUMDIV = 500;
const int MAX_STEPS = 10;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;

vector<SceneObject*> sceneObjects;
TextureOpenIL texture;

glm::vec3 diskRand(float radius) {
	float angle = glm::linearRand(0.0f, 2.0f * 3.14159f); // Random angle
	float r = glm::linearRand(0.0f, radius); // Random radius within the disk
	return glm::vec3(r * cos(angle), r * sin(angle), 0.0f); // 2D point on the disk in X,Y
}

//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step) {
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	//Multiple light and shadows
	glm::vec3 lightPos1(20, 23, 15);					//Light1's position
	glm::vec3 lightPos2(-20, 23, 15);                   //Light2's position


	glm::vec3 color(0);
	SceneObject* obj ;
	SceneObject* ShadowObj1;
	SceneObject* ShadowObj2;

	ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
	if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found
	glm::vec3 hitPoint = ray.hit;
	glm::vec3 normal = obj->normal(hitPoint);
	glm::vec3 viewVec = -ray.dir;



	///////////////////Spot Light//////////////////////////////
	glm::vec3 spotLight(0, 5, -5);
	glm::vec3 spotTarget(0, -15, -40);  //focouse on table
	glm::vec3 spotDir = glm::normalize(spotTarget - spotLight);
	float cutoffAngle = 20.0f;
	float cutoff = glm::cos(glm::radians(cutoffAngle));
	
	//////////////////Multiple Lights shadows//////////////////
	//ambientTerm , diffuseTerm and specularTerm are in SceneObject::lighting();
	//base on a ambient, if the obj not in shadows, 
	float ambient_factor = 0.2f;
	glm::vec3 ambient = ambient_factor * obj->getColor();
	color += ambient;

	// add shadows for light 1
	glm::vec3 lightVec1 = lightPos1 - hitPoint;
	Ray shadowRay1(hitPoint, lightVec1);
	shadowRay1.closestPt(sceneObjects);
	bool inShadow1 = (shadowRay1.index != -1 && shadowRay1.dist < glm::length(lightVec1));
	if (!inShadow1) {
		color += obj->lighting(lightPos1, viewVec, hitPoint);
	}
	else if (sceneObjects[shadowRay1.index]->isTransparent()) {
		glm::vec3 totalLight1 = obj->lighting(lightPos1, viewVec, hitPoint);
		glm::vec3 shadowAmbient1 = 0.2f * obj->getColor();
		glm::vec3 diffSpec = totalLight1 - shadowAmbient1;
		color += shadowAmbient1 + 0.8f * diffSpec;
	}

	// add shadows for light 2 
	glm::vec3 lightVec2 = lightPos2 - hitPoint;
	Ray shadowRay2(hitPoint, lightVec2);
	shadowRay2.closestPt(sceneObjects);
	bool inShadow2 = (shadowRay2.index != -1 && shadowRay2.dist < glm::length(lightVec2));

	if (!inShadow2) {
		color += obj->lighting(lightPos2, viewVec, hitPoint);
	}
	else if (sceneObjects[shadowRay2.index]->isTransparent()) {
		glm::vec3 totalLight2 = obj->lighting(lightPos2, viewVec, hitPoint);
		glm::vec3 shadowAmbient2 = 0.2f * obj->getColor();
		glm::vec3 diffSpec = totalLight2 - shadowAmbient2;
		color += shadowAmbient2 + 0.8f * diffSpec;
	}

	// Spotlight (independent of other lights)
	glm::vec3 spotLightVec = spotLight - hitPoint;
	Ray spotShadowRay(hitPoint, glm::normalize(spotLightVec));
	spotShadowRay.closestPt(sceneObjects);
	bool inSpotShadow = (spotShadowRay.index != -1 && spotShadowRay.dist < glm::length(spotLightVec));
	//std::cout << "Spot shadow ray hit object index: " << spotShadowRay.index << std::endl;
	if (!inSpotShadow) {
		float angleCos = glm::dot(glm::normalize(-spotLightVec), spotDir);
		if (angleCos >= cutoff) { // Inside spotlight cone
			color +=  obj->lighting(spotLight, viewVec, hitPoint);
		}
	}


	//////////////////reflection////////////////////////
	if (obj->isReflective() && step < MAX_STEPS)
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(hitPoint);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(hitPoint, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}


	////////////////////Refraction/////////////////////////
	if (obj->isRefractive() && step < MAX_STEPS)
	{
		float IOR = obj->getRefractiveIndex();
		float refrCoeff = obj->getRefractionCoeff();

		//nIn is the n in lecture note
		glm::vec3 nIN = obj->normal(hitPoint);
		float eta = 1.0f / IOR;// comein from air(eg, glass = 1.0/1.5)

		//From inside of obj, inverse 
		if (glm::dot(ray.dir, nIN) > 0) {
			nIN = -nIN;
			eta = IOR;
		}

		
		glm::vec3 g = glm::refract(ray.dir, nIN, eta);
		if (glm::length(g) == 0) {
			return color;
		}
		else if (glm::length(g) > 0) {
			Ray refrRayinside(hitPoint, g);
			refrRayinside.closestPt(sceneObjects);

			//nOut is the m in lecture note
			glm::vec3 nOUT = obj->normal(refrRayinside.hit);
			if (glm::dot(refrRayinside.dir, nOUT) > 0) {
				nOUT = -nOUT;
			}
			glm::vec3 h = glm::refract(g, nOUT, 1.0f / eta);

			Ray refrRayToOut(refrRayinside.hit, h);
			refrRayToOut.closestPt(sceneObjects);

			glm::vec3 refrColor = trace(refrRayToOut, step + 1);
			color = color + refrCoeff * refrColor;
		}
	}
	
	//////////////////Transparency////////////////////////
	if (step < MAX_STEPS && obj->isTransparent()) {
		Ray gray = Ray(hitPoint, ray.dir);
		gray.closestPt(sceneObjects);
		Ray hray = Ray(gray.hit,gray.dir);
		hray.closestPt(sceneObjects);
		float tranCoeff = obj->getTransparencyCoeff();

		trace(hray, step + 1);
		color = tranCoeff * color + (1.0f - tranCoeff) * trace(hray, step + 1);
		
	}

	////////////// Chessboard pattern //////////////////
	if (ray.index == 0)
	{
		int tileSize = 1;
		int ix = int(ray.hit.x / tileSize);
		int iz = int(ray.hit.z / tileSize);
		int k = (ix + iz) % 2;

		glm::vec3 checkerColor;
		if (k == 0) checkerColor = glm::vec3(1.0, 1.0, 1.0);
		else        checkerColor = glm::vec3(0.2, 0.2, 0.2);

		obj->setColor(checkerColor);
		float x1 = -15, x2 = 15, z1 = -30, z2 = -20;
		float texcoords = (ray.hit.x - x1) / (x2 - x1);
		float texcoordt = (ray.hit.z - z1) / (z2 - z1);
		if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1) {
			color = texture.getColorAt(texcoords, texcoordt);
			obj->setColor(color);


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
	glm::vec3 eye(0., 0., 3.);

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
// | Hex Code | RGB (0–255)       | `glm::vec3(r, g, b)` |
//---------- | ------------------ - | ---------------------- - |
// `#7400B8`| (116, 0, 184) | `glm::vec3(0.455, 0.0, 0.722)` |
// `#6930C3`| (105, 48, 195) | `glm::vec3(0.412, 0.188, 0.765)` |
// `#5E60CE`| (94, 96, 206) | `glm::vec3(0.369, 0.376, 0.808)` |
// `#5390D9`| (83, 144, 217) | `glm::vec3(0.325, 0.565, 0.851)` |
// `#4EA8DE`| (78, 168, 222) | `glm::vec3(0.306, 0.659, 0.871)` |
// `#48BFE3`| (72, 191, 227) | `glm::vec3(0.282, 0.749, 0.89)` |
// `#56CFE1`| (86, 207, 225) | `glm::vec3(0.337, 0.812, 0.882)` |
// `#64DFDF`| (100, 223, 223) | `glm::vec3(0.392, 0.875, 0.875)` |
// `#72EFDD`| (114, 239, 221) | `glm::vec3(0.447, 0.937, 0.867)` |
// `#80FFDB`| (128, 255, 219) | `glm::vec3(0.502, 1.0, 0.859)` |
// 
// 
//----------------------------------------------------------------------------------
void initialize() {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
	texture = TextureOpenIL("Flowers.jpg");

	glClearColor(0, 0, 0, 1);
	Plane* tableTop = new Plane(
		glm::vec3(-20, -17, -30),
		glm::vec3(-20, -17, -20),
		glm::vec3(20, -17, -20),
		glm::vec3(20, -17, -30)
	);
	tableTop->setColor(glm::vec3(0)); 
	//tableTop->setReflectivity(true, 0.8);
	sceneObjects.push_back(tableTop);

	Plane* bottomPlane = new Plane(
		glm::vec3(35, -25, 40),
		glm::vec3(35, -25, -40),
		glm::vec3(-35, -25, -40),
		glm::vec3(-35, -25, 40)
	
	);
	bottomPlane->setColor(glm::vec3(0.275, 0.510, 0.706));
	sceneObjects.push_back(bottomPlane);


	Plane* topPlane = new Plane(
		glm::vec3(-35, 25, -40),
		glm::vec3(35, 25, -40),
		glm::vec3(35, 25, 40),
		glm::vec3(-35, 25, 40)
	);
	topPlane->setColor(glm::vec3(0.502, 1.0, 0.859));
	sceneObjects.push_back(topPlane);

	Plane* leftPlane = new Plane(
		glm::vec3(-35, -25, -40),
		glm::vec3(-35, 25, -40),
		glm::vec3(-35, 25, 40),
		glm::vec3(-35, -25, 40)
	);
	leftPlane->setColor(glm::vec3(0.455, 0.0, 0.722));
	sceneObjects.push_back(leftPlane);


	Plane* rightPlane = new Plane(
		glm::vec3(35, 25, -40),
		glm::vec3(35, -25, -40),
		glm::vec3(35, -25, 40),
		glm::vec3(35, 25, 40)
	);
	rightPlane->setColor(glm::vec3(0.412, 0.188, 0.765));
	sceneObjects.push_back(rightPlane);


	Plane* backPlane = new Plane(
		glm::vec3(-35, -25, -40),
		glm::vec3(35, -25, -40),
		glm::vec3(35, 25, -40),
		glm::vec3(-35, 25, -40)
	);
	backPlane->setColor(glm::vec3(0, 0.18, 0.65));
	sceneObjects.push_back(backPlane);

	Plane* behindPlane = new Plane(
		glm::vec3(-35, 25, 40),
		glm::vec3(35, 25, 40),
		glm::vec3(35, -25, 40),
		glm::vec3(-35, -25, 40)
	);
	behindPlane->setColor(glm::vec3(0, 0.18, 0.65));
	sceneObjects.push_back(behindPlane);

	//This Plane is a mirror
	Plane* mirrorPlane = new Plane(
		glm::vec3(25, 10, -33),
		glm::vec3(25, -10, -39),
		glm::vec3(-25, -10, -39),
		glm::vec3(-25, 10, -33)
	);
	mirrorPlane->setColor(glm::vec3(0));                 
	mirrorPlane->setReflectivity(true, 1.0);           
	sceneObjects.push_back(mirrorPlane);



	Cylinder* leg1 = new Cylinder(glm::vec3(-10.0, -25.0, -30.0), 0.5, 8.0); 
	leg1->setColor(glm::vec3(0.4, 0.2, 0.1)); 
	sceneObjects.push_back(leg1);
	Cylinder* leg2 = new Cylinder(glm::vec3(10.0, -25.0, -30.0), 0.5, 8.0); 
	leg2->setColor(glm::vec3(0.4, 0.2, 0.1)); 
	sceneObjects.push_back(leg2);
	Cylinder* leg3 = new Cylinder(glm::vec3(-10.0, -25.0, -21.0), 0.5, 8.0); 
	leg3->setColor(glm::vec3(0.4, 0.2, 0.1)); 
	sceneObjects.push_back(leg3);
	Cylinder* leg4 = new Cylinder(glm::vec3(10.0, -25.0, -21.0), 0.5, 8.0); 
	leg4->setColor(glm::vec3(0.4, 0.2, 0.1)); 
	sceneObjects.push_back(leg4);


	Sphere *sphere1 = new Sphere(glm::vec3(-12.0, -5.0, -23.0), 3);
	sphere1->setColor(glm::vec3(0.369, 0.376, 0.808));  //Set colour to blue
	sphere1->setTransparency(true,0.7);
	sceneObjects.push_back(sphere1);	

	//sphere1->setSpecularity(false); 
	//sphere1->setShininess(5); //Set shininess to 5

	Sphere* sphere2 = new Sphere(glm::vec3(0.0, -5.0, -23.0), 3);
	sphere2->setColor(glm::vec3(0, 0, 0));  
	sceneObjects.push_back(sphere2);		 //Add sphere to scene objects
	sphere2 ->setReflectivity(true, 1.0);


	float CDR = 3.14159265 / 180.0;
	glm::mat4 t = glm::mat4(1.0);
	t = glm::translate(t, glm::vec3(20.0, -2.0, -30.0));
	t = glm::rotate(t, 15 * CDR, glm::vec3(0.0, 1.0, 0.0));
	t = glm::scale(t, glm::vec3(5.0, 1.0, 1.0));
	Sphere* sphere3 = new Sphere(glm::vec3(12.0, -5.0, -23.0), 3);
	sphere3->setTransform(t);
	sphere3->setColor(glm::vec3(1, 1, 0));   
	sceneObjects.push_back(sphere3);		 //Add sphere to scene objects
	
	Cylinder* cylinder1 = new Cylinder(glm::vec3(12.0, -17.0, -23.0), 2.5, 8.);
	cylinder1->setColor(glm::vec3(0.208f, 0.369f, 0.231f));
	//cylinder1->setReflectivity(true, 1);
	sceneObjects.push_back(cylinder1);
	cylinder1->setRefractivity(true, 0.8, 1.5);
	

	Cylinder* cylinder2 = new Cylinder(glm::vec3(-12.0, -17.0, -23.0), 2.5, 8.);
	cylinder2->setColor(glm::vec3(1.0, 0.078, 0.576));
	//cylinder2->setReflectivity(true, 1);
	sceneObjects.push_back(cylinder2);
	cylinder2->setRefractivity(true, 0.8 , 1.5);

	Cone* cone1 = new Cone(glm::vec3(0.0, -17.0, -23.0), 2.5, 8.);
	cone1->setColor(glm::vec3(0.208, 0.369, 0.231));
	sceneObjects.push_back(cone1);


	Cone* cone2 = new Cone(glm::vec3(-17.0, -17.0, -27.0), 3.0, 20.);
	cone2->setColor(glm::vec3(0.208, 0.369, 0.231));
	sceneObjects.push_back(cone2);


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
