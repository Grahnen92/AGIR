#pragma once
#include "Utilities.h"

#include "Mesh.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cuboid.h"
#include "Room.h"

#include "Camera.h"
#include "Ray.h"
#include "MatrixStack.h"
#include "Image.h"
#include "RNG.h"

#include <vector>
#include <iomanip>
#include <iostream>
#include <random>
#include <time.h>
#include <algorithm>    // std::max

#include <ctime>
#include <math.h>

# define M_PI  3.14159265358979323846

int main()
{
	Image imgTest(512, 512 );

	Camera cam(glm::vec3(0.05f, 0.0f, 0.74f), glm::vec3(0.05f, 0.0f, - 1.0f));

	std::vector<Mesh*>* scene = new std::vector<Mesh*>;
	//light
	scene->push_back(new Cuboid(glm::vec3(0.0f, 0.5f, -0.5f),	//position
		glm::vec3(0.2f, 0.2f, 0.2f),							//dimension
		glm::vec3(100.0f, 100.0f, 100.0f),						//emission
		glm::vec3(0.1f, 0.1f, 0.1f), 0.6f));					//brdf and P
	//objects
	scene->push_back(new Cuboid(glm::vec3(0.5f, 0.5f, -0.5f), 
		glm::vec3(0.2f, 0.2f, 0.2f), 
		glm::vec3(5.5f, 5.5f, 5.5f),
		glm::vec3(0.1f, 0.1f, 0.1f), 0.6f));
	scene->push_back(new Cuboid(glm::vec3(-0.5f, 0.5f, -0.0f),
		glm::vec3(0.2f, 0.2f, 0.2f), 
		glm::vec3(1.5f, 1.5f, 1.5f),
		glm::vec3(0.1f, 0.1f, 0.1f), 0.6f));
	//room
	scene->push_back(new Room(glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(1.5f, 1.5f, 1.5f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.1f, 0.1f, 0.1f), 0.3f));
	
	std::cout << "Rendering started...\n";
	std::cout << "Image Dimensions: " << imgTest.x << "x" << imgTest.y << std::endl;
	clock_t begin = clock();


	//start rendering--------------------------------------------------------------
	//seed random numbers
	//srand(static_cast <unsigned> (time(0)));
	std::uniform_real_distribution<float> distribution(0.0, 1.0f);
	std::default_random_engine generator;

	Ray* rIt;
	float x = (float) imgTest.x / (float) imgTest.y; 
	float y = 1.0f;//(float) imgTest.y / (float) imgTest.x;
	float xCo = -x;
	float yCo = -y;
	float rX; float rY;
	float maxI = 0.0f;
	float tmpFloat;

	RNG rng;

	float xStep = (2* x) / imgTest.x;
	float yStep = (2* y) / imgTest.y;
	glm::vec3 tempRGB;
	//yStep = xStep;
	for (int i = 0; i < imgTest.y; i++)
	{
		yCo += yStep;
		xCo = -x;
		for (int j = 0; j < imgTest.x; j++)
		{
			xCo += xStep;
			tempRGB = glm::vec3(0.0f, 0.0f, 0.0f);
			for (int p = 0; p < 10; p++)
			{
				//rX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / xStep));
				//rY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / yStep));
				rX = rng.dist(rng.mt) * xStep;
				rY = rng.dist(rng.mt) * yStep;

				glm::vec3 rDirection = glm::mat3(cam.getCTransform()) * (glm::vec3(xCo+rX, yCo+rY, -1.0f));
				glm::vec3 rPos = glm::vec3(cam.getCTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				rIt = new Ray(rPos, rDirection, nullptr, scene, rng, glm::vec3(1.0f));
				tempRGB += glm::vec3(rIt->evaluate());

				delete rIt;
			}

			//square root this value?
			imgTest.imgData[i][j] = tempRGB / 10.0f;

			//find max pixel value in image
			tmpFloat = std::max(std::max(imgTest.imgData[i][j].x, imgTest.imgData[i][j].y), imgTest.imgData[i][j].z);
			if(tmpFloat > maxI) maxI = tmpFloat;
		}
	}

	//normalize picture
	maxI = 1/maxI;
	for(int i = 0; i < imgTest.y; i++)
		for(int j = 0; j < imgTest.x; j++)
		{
			imgTest.imgData[i][j].x = sqrt(imgTest.imgData[i][j].x*maxI);
			imgTest.imgData[i][j].y = sqrt(imgTest.imgData[i][j].y*maxI);
			imgTest.imgData[i][j].z = sqrt(imgTest.imgData[i][j].z*maxI);
		}

			
		
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Rendering done. Elapsed time: " << elapsed_secs << " seconds." << std::endl;
	
	imgTest.saveBMP();

	while (true) { }

	return 0;
}
