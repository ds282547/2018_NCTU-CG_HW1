#ifndef _CG_HW1_DBLPEN_H_
#define _CG_HW1_DBLPEN_H_

#include "Vector.hpp"
#include <list>

/**
 * This class is for representing a sinfgle double pendulum system abstractly.
 * 
*/

class DoublePendulum
{
public:

	DoublePendulum(Vector3f rootPosition, float topRodLength, float bottomRodLength, float topBobWeight, float bottomBobWeight
		, bool fireball=false, GLuint textureID = 0):
	rootPosition ( rootPosition ) ,
	topRodLength ( topRodLength ) ,
	bottomRodLength ( bottomRodLength ) ,
	topBobWeight ( topBobWeight ) ,
	bottomBobWeight ( bottomBobWeight ) ,
	textureID( textureID) ,
	fireball ( fireball ) {
		cubeSize = powf(topBobWeight, 0.333f) * 0.9f;
		sphereSize = powf(bottomBobWeight, 0.333f) * 0.45;
		sphere = gluNewQuadric();
		gluQuadricTexture(sphere, fireball);
	}

	void render()
	{
		// reminder : the radius of a bob should be proportional to the weight of the bob ( r : weight^(0.33) )

		// you can use GLUquadricObj to draw sphere more easily, but please remember to Translate to the 
		// correct position.
		firstPoint = this->rootPosition;
		firstPoint += Vector3f(sinf(topAngle)*topRodLength, -cosf(topAngle)*topRodLength, 0);
		secondPoint = firstPoint;
		secondPoint += Vector3f(sinf(bottomAngle)*bottomRodLength, -cosf(bottomAngle)*bottomRodLength, 0);

		glDisable(GL_LIGHTING);
		
		glBegin(GL_LINES);
		glColor3f(0.0,1.0,0.0);
		glVertex3fv(this->rootPosition());
		glVertex3fv(firstPoint());
		glColor3f(0.0, 0.0, 1.0);
		glVertex3fv(firstPoint());
		glVertex3fv(secondPoint());
		glEnd();

		// trail
		if (fireball) {
			glBegin(GL_LINE_STRIP);
			glColor3f(1.0, 1.0, 1.0);
			trail_list.emplace_back(secondPoint);
			if (trail_list.size() >= TRAIL_LEN)
				trail_list.pop_front();
			for (auto &pt : trail_list)
				glVertex3fv(pt());
			glEnd();
		}
		

		glEnable(GL_LIGHTING);

		// draw top bob
		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glTranslatef(firstPoint[0], firstPoint[1], firstPoint[2]);
		glRotatef(topAngle/M_PI*180.0f, 0, 0, 1);
		glutSolidCube(cubeSize);
		glPopMatrix();
		
		// center ball texture setting
		if (fireball) {

			glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_LIGHTING);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glEnable(GL_TEXTURE_2D);
			
		}
		// draw bottom bob
		glPushMatrix();
		glTranslatef(secondPoint[0], secondPoint[1], secondPoint[2]);

		if (fireball)
			glRotatef(bottomAngle / M_PI * 180.0f, 0, 0, 1);

		gluSphere(sphere, sphereSize, 20, 20);
		glPopMatrix();

		if (fireball) 
			glDisable(GL_TEXTURE_2D);

	}

	void updatePhysics(float deltaTime)
	{
		// Reference : https://www.youtube.com/watch?v=uWzPe_S-RVE

		// this method should be erased when publishing template project
		
		static const float g = 9.8;

		const float m1 = topBobWeight;
		const float m2 = bottomBobWeight;
		const float a1 = topAngle;
		const float a2 = bottomAngle;
		const float r1 = topRodLength;
		const float r2 = bottomRodLength;
		const float a1_v = topAngleVelocity;
		const float a2_v = bottomAngleVelocity;


		float num1 = -g * (2 * m1 + m2)*sinf(a1);
		float num2 = -m2 * g*sinf(a1 - 2 * a2);
		float num3 = -2 * sinf(a1 - a2)*m2;
		float num4 = a2_v * a2_v * r2 + a1_v * a1_v *r1 * cosf(a1 - a2);
		float den = r1 * (2 * m1 + m2 - m2 * cosf(2 * a1 - 2 * a2));

		topAngleAcceleration = (num1 + num2 + num3 * num4) / den;

		num1 = 2 * sinf(a1 - a2);
		num2 = (a1_v*a1_v*r1*(m1 + m2));
		num3 = g * (m1 + m2)*cosf(a1);
		num4 = a2_v * a2_v * r2 * m2 * cosf(a1 - a2);
		den = r2 * (2 * m1 + m2 - m2 * cosf(2 * a1 - 2 * a2));

		bottomAngleAcceleration = num1 * (num2 + num3 + num4) / den;


		topAngleVelocity += topAngleAcceleration * deltaTime;
		bottomAngleVelocity += bottomAngleAcceleration * deltaTime;

		topAngle += topAngleVelocity * deltaTime;
		bottomAngle += bottomAngleVelocity * deltaTime;


	}

public:
	float topRodLength;
	float topBobWeight;
	float bottomRodLength;
	float bottomBobWeight;

	float cubeSize;
	float sphereSize;



	Vector3f GetTopBobPosition()
	{
		return firstPoint;
	}

	Vector3f GetBottomBobPosition()
	{
		return secondPoint;
	}

private:
	Vector3f rootPosition;
	Vector3f firstPoint;
	Vector3f secondPoint;
	

	// for updating motion
	float topAngle = M_PI / 4; // in this template, this is represented in radian form
	float bottomAngle = 0; // in this template, this is represented in radian form

	float topAngleVelocity = 1.2;
	float bottomAngleVelocity = -0.7;

	float topAngleAcceleration = 0;
	float bottomAngleAcceleration = 0;

	// for rendering settings
	bool fireball;
	GLuint textureID;
	//GLU sphere
	GLUquadricObj *sphere;
	// for trajectory 
	std::list<Vector3f> trail_list;
	static const unsigned TRAIL_LEN;
};
const unsigned DoublePendulum::TRAIL_LEN = 50;

#endif