#ifndef _CG_HW1_TRIPEN_H_
#define _CG_HW1_TRIPEN_H_

#include "Vector.hpp"

// For Bonus

class TriplePendulum
{
public:

	TriplePendulum(Vector3f rootPosition, float topRodLength, float centerRodLength, float bottomRodLength
		,float topBobWeight, float centerBobWeight, float bottomBobWeight,
		GLuint textures[]) :
		rootPosition(rootPosition),
		topRodLength(topRodLength),
		centerRodLength(centerRodLength),
		bottomRodLength(bottomRodLength),
		topBobWeight(topBobWeight),
		centerBobWeight(centerBobWeight),
		bottomBobWeight(bottomBobWeight)
	{
		sphereSize[0] = powf(topBobWeight, 0.333f) * 0.45f;
		sphereSize[1] = powf(centerBobWeight, 0.333f) * 0.45;
		sphereSize[2] = powf(bottomBobWeight, 0.333f) * 0.45;
		this->textures = textures;

		sphere = gluNewQuadric();
		gluQuadricTexture(sphere, true);
	}

	void render()
	{
		// reminder : the radius of a bob should be proportional to the weight of the bob ( r : weight^(0.33) )

		// you can use GLUquadricObj to draw sphere more easily, but please remember to Translate to the 
		// correct position.
		
		bobPosition[0] = this->rootPosition;
		bobPosition[0] += Vector3f(sinf(topAngle)*topRodLength, cosf(topAngle)*topRodLength, 0);
		bobPosition[1] = bobPosition[0];
		bobPosition[1] += Vector3f(sinf(centerAngle)*centerRodLength, cosf(centerAngle)*bottomRodLength, 0);
		bobPosition[2] = bobPosition[1];
		bobPosition[2] += Vector3f(sinf(bottomAngle)*bottomRodLength, cosf(bottomAngle)*bottomRodLength, 0);

		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);
		glColor3f(0.4, 0.2, 0.2);
		glVertex3fv(this->rootPosition());
		glVertex3fv(bobPosition[0]());
		glColor3f(0.2, 0.4, 0.2);
		glVertex3fv(bobPosition[0]());
		glVertex3fv(bobPosition[1]());
		glColor3f(0.2, 0.2, 0.4);
		glVertex3fv(bobPosition[1]());
		glVertex3fv(bobPosition[2]());
		glEnd();

		// Bobs

		glEnable(GL_LIGHTING);
		
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0, 1.0, 1.0);

		// Bob 1
		glBindTexture(GL_TEXTURE_2D, textures[2]);

		glPushMatrix();
		glTranslatef(bobPosition[0][0], bobPosition[0][1], bobPosition[0][2]);
		glRotatef(topAngle / M_PI * 180.0f, 0, 0, 1);
		glRotatef(-90, 1, 0, 0);
		gluSphere(sphere, sphereSize[0], 20, 20);
		glPopMatrix();

		// Bob 2
		glBindTexture(GL_TEXTURE_2D, textures[3]);

		glPushMatrix();
		glTranslatef(bobPosition[1][0], bobPosition[1][1], bobPosition[1][2]);
		glRotatef(-90, 1, 0, 0);
		gluSphere(sphere, sphereSize[1], 20, 20);
		glPopMatrix();

		// Bob 3
		glBindTexture(GL_TEXTURE_2D, textures[4]);

		glPushMatrix();
		glTranslatef(bobPosition[2][0], bobPosition[2][1], bobPosition[2][2]);
		glRotatef(-90, 1, 0, 0);
		gluSphere(sphere, sphereSize[2], 20, 20);
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

	}

	void updatePhysics(float deltaTime)
	{
		// Reference Paper : https://www.nickeyre.com/images/triplependulum.pdf


		deltaTime *= 0.2;
		const float g = 9.8;

		const float m1 = topBobWeight;
		const float m2 = centerBobWeight;
		const float m3 = bottomBobWeight;
		const float m1_2 = m1*m1;
		const float m2_2 = m2*m2;
		const float m3_2 = m3*m3;
		const float a1 = topAngle;
		const float a2 = centerAngle;
		const float a3 = bottomAngle;
		const float l1 = 0.508;
		const float l2 = 0.254;
		const float l3 = 0.127;
		const float l1_2 = l1 * l1;
		const float l2_2 = l2 * l2;
		const float l3_2 = l3 * l3;
		const float a1_v = topAngleVelocity;
		const float a2_v = centerAngleVelocity;
		const float a3_v = bottomAngleVelocity;
		const float a1_v_2 = a1_v * a1_v;
		const float a2_v_2 = a2_v * a2_v;
		const float a3_v_2 = a3_v * a3_v;

		static const float I1 = 3.526e-2;
		static const float I2 = 0.625e-2;
		static const float I3 = 4.848e-2;
		static const float k1 = 1e-8;
		static const float k2 = 0 ;
		static const float k3 = 1e-5 ;

		float num = -(2 * ((l3_2*m3_2*sinf(2 * a1 - 2 * a3)*(4 * I2 - l2_2 * m2) + l2_2 * sinf(2 * a1 - 2 * a2)*(m2 + 2 * m3)*(m2 + 2 * m3)*(m2*m3*l3_2 + 4 * I3*(m2 + 2 * m3)))*l1_2*a1_v_2 +
			(l2*(sinf(a1 - a2)*((m2*m3*(m2 + 3 * m3)*l3_2 + 4 * I3*(m2_2 + 6 * m2*m3 + 8 * m3_2))*l2_2 + 4 * I2*(m3*(m2 +
			m3)*l3_2 + 4 * I3*(m2 + 2 * m3)))) + l3 * m3_2*sinf(a1 + a2 - 2 * a3)*(4 * I2 - l2_2 * m2))*a2_v_2 - 4 * k2*l2*(cos(a1 - a2)*(m3*(m2 +
			m3)*l3_2 + 4 * I3*(m2 + 2 * m3)) - l3_2 * m3_2*cos(a1 + a2 - 2 * a3))*a2_v + l3 * m3*(sinf(a1 - a3)*(8 * I3*m3*l2_2 + 4 * I2*m3*l3_2 + 16 * I2*I3) +
			l2_2 * sinf(a1 - 2 * a2 + a3)*(m2*m3*l3_2 + 4 * I3*(m2 + 2 * m3))*a3_v_2 - 4 * k3*l3*m3*(cos(a1 - a3)*(2 * m3*l2_2 + 4 * I2) - l2_2 * cos(a1 -
			2 * a2 + a3)*(m2 + 2 * m3))*a3_v - g * (sinf(a1)*((m3*(m1*m2 + 2 * m1*m3 + 3 * m2*m3 + m2_2)*l3_2 + 4 * I3*(m2_2 + 6 * m2*m3 +
			m1 * m2 + 4 * m3_2 + 4 * m1*m3))*l2_2 + 4 * I2*(m3*(m1 + 2 * m2 + m3)*l3_2 + 4 * I3*(m1 + 2 * m2 + 2 * m3))) + l3_2 * m3_2*(sinf(a1 -
			2 * a3)*(4 * I2 - l2_2 * m2) - 2 * l2_2*cos(2 * a2 - 2 * a3)*sinf(a1)*(m1 + m2)) + l2_2 * sinf(a1 - 2 * a2)*(m2 + 2 * m3)*(m2*m3*l3_2 +
			4 * I3*(m2 + 2 * m3))))*l1 + 2 * k1*(4 * I2*(m3*l3_2 + 4 * I3) + l2_2 * (m3*(m2 + 2 * m3)*l3_2 + 4 * I3*(m2 + 4 * m3)) - 2 * l2_2*l3_2*m3_2*cos(2 * a2 -
			2 * a3))*a1_v));
		float den = (64 * I1*I2*I3 + 8 * I3*l1_2*l2_2*m3_2 + 8 * I1*l2_2*l3_2*m3_2 + 8 * I2*l1_2*l3_2*m3_2 + 8 * I3*l1_2*l2_2*m3_2 + 32 * I3*l1_2*l2_2*m3_2 + 16 * I2*I3*l1_2*m1 + 16 * I1* I3*l2_2*m2 +
			64 * I2*I3*l1_2*m2 + 16 * I1*I2*l3_2*m3 + 64 * I1*I3*l2_2*m3 + 64 * I2*I3*l1_2*m3 + 4 * I3*l1_2*l2_2*m1*m2 + 4 * I2*l1_2*l3_2*m1*m3 + 16 *I3*l1_2*l2_2*m1*m3 +
			4 * I1*l2_2*l3_2*m2*m3 + 16 * I2*l1_2*l3_2*m2*m3 + 48 * I3*l1_2*l2_2*m2*m3 - 8 * I1*l2_2*l3_2*m3_2*cos(2 * a2 - 2 * a3) - 2 * l1_2*l2_2*cos(2 * a1 - 2 * a2)*(m2 +
				2 * m3)*(m2*m3*l3_2 + 4 * I3*(m2 + 2 * m3)) - 2 * l1_2*l3_2*m3_2*cos(2 * a1 - 2 * a3)*(-m2 * l2_2 + 4 * I2) + 2*l1_2*l2_2*l3_2*m1*m3_2 + 6 * l1_2*l2_2*l3_2*m2*m3_2 +
			2 * l1_2*l2_2*l3_2*m2_2*m3 + l1_2 * l2_2*l3_2*m1*m2*m3 - 2 * l1_2*l2_2*l3_2*m1*m3_2*cos(2 * a2 - 2 * a3) - 4 * l1_2*l2_2*l3_2*m2*m3_2*cos(2 * a2 - 2 * a3));

		topAngleAcceleration = num/den;

		num = (2 * ((l1_2*sin(2 * a1 -2 * a2)*(m2 + 2 * m3)*(m2*m3*l3_2 + 4 * I3*(m2 + 2 * m3)) - l3_2*m3_2*sin(2 * a2 - 2 * a3)*((m1 +
			2 * m2)*l1_2 + 4 * I1))*l2_2*a2_v_2 + l1 * (sin(a1 -a2)*((m3*(m1*(m2 + m3) + 2 * m2*(2 * m2 + 3 * m3))*l3_2 + 4 * I3*(m2 + 2 * m3)*(m1
				+ 4 * m2 + 4 * m3))*l1_2 + 4 * I1*(m3*(m2 + m3)*l3_2 + 4 * I3*(m2 + 2 * m3))) - l3_2*m3_2*sin(a1 + a2 - 2 * a3)*((m1 + 2 * m2)*l1_2 
					+ 4 * I1))*l2*a1_v_2 + 4 * k1*l1*(cos(a1 - a2)*(m3*(m2 + m3)*l3_2 + 4 * I3*(m2 + 2*m3)) - l3_2*m3_2*cos(a1 + a2 - 2 * a3))*l2*a1_v +
			(-l3*m3*(sin(a2 -a3)*((m3*(m1 + 3 * m2)*l3_2 + 4 * I3*(m1 + 3 * m2 + 2 * m3))*l1_2 + 4 * I1*(m3*l3_2 + 4 * I3))-l1_2*sin(2 * a1 -a2 -
				a3)*(m2*m3*l3_2 + 4 * I3*(m2 + 2 * m3)))*a3_v_2 + 4 * k3*l3*m3*(cos(a2-a3)*((m1 + 3 * m2 + 2 * m3)*l1_2 + 4 * I1) -l1_2*cos(2 * a1-
				a2 -a3)*(m2 + 2 * m3)) *a3 + g * (sin(a2)*((m2*m3*(2 * m2 + 3 * m3)*l3_2 + 8 * I3*(m2_2 + 3 * m2*m3 + 2 * m3_2))*l1_2 + 4 * I1*(m3*(m2 +
					m3)*l3_2 + 4 * I3*(m2 + 2 * m3))) - l1_2*sin(2 * a1 - a2)*(m3*(m1*(m2 + m3) + m2*(2 * m2 + 3 * m3))*l3_2 + 4 * I3*(m2 + 
						2 * m3)*(m1 + 2 * m2 + 2 * m3)) + l3_2 * m3_2*(sin(a2 - 2 * a3)*(m2*l1_2 + 4 * I1) + l1_2 * sin(2 * a1 + a2 - 2 * a3)*(m1 + m2))))*l2 -
			2*k2*(4 * I1*(m3*l3_2 + 4 * I3) + l1_2 * (m3*(m1 + 4 * m2 + 2 * m3)*l3_2 + 4 * I3*(m1 + 4 * m2 + 4 * m3)) - 2 * l1_2*l3_2*m3_2*cos(2 * a1 -2*a3))*a2_v));

		den = (64 * I1*I2*I3 + 8 * I3*l1_2*l2_2*m2_2 + 8 * I1*l2_2*l3_2*m3_2 + 8 * I2*l1_2*l3_2*m3_2 + 32 * I3*l1_2*l2_2*m3_2 + 16 * I2*I3*l1_2*m1 + 16 * I1*I3*l2_2*m2
			+ 64 * I2*I3*l1_2*m2 + 16 * I1*I2*l3_2*m3 + 64 * I1*I3*l2_2*m3 + 64 * I2*I3*l1_2*m3 + 4*I3 * l1_2*l2_2*m1*m2 + 4*I2 * l1_2*l3_2*m1*m3 + 16 * I3*l1_2*l2_2*m1*m3 +
			4*I1*l2_2*l3_2*m2*m3 + 16 * I2*l1_2*l3_2*m2*m3 + 48 * I3*l1_2*l2_2*m2*m3 - 8 * I1*l2_2*l3_2*m3_2*cos(2 * a2 - 2 * a3) - 2 * l1_2*l2_2*cos(2 * a1 - 2 * a2)*(m2 + 
				2 * m3)*(m2*m3*l3_2 + 4 * I3*(m2 + 2 * m3))-2*l1_2*l3_2*m3_2*cos(2 * a1-2*a3)*(-m2*l2_2 + 4 * I2) + 2 * l1_2*l2_2*l3_2*m1*m3_2 + 6 * l1_2*l2_2*l3_2*m2*m3_2 + 
			2 * l1_2*l2_2*l3_2*m2_2 * m3 + l1_2 * l2_2*l3_2*m1*m2*m3 - 2 * l1_2*l2_2*l3_2*m1*m3_2*cos(2 * a2 - 2 * a3) - 4 * l1_2*l2_2*l3_2*m2*m3_2*cos(2 * a2 - 2 * a3));

		centerAngleAcceleration = num/den;

		num = -(2 * (32 * I1*I2*k3*a3_v - l2 * l3*m3*a2_v_2 * (sin(a2 - a3)*((l2_2*(m1*m2 + 4 * m1*m3 + 6 * m2*m3 + m2_2) + 4 * I2*(m1 +
			3 * m2 + 2 * m3))*l1_2 + 4 * I1*(4 * I2 + l2_2 * (m2 + 4 * m3))) + l1_2 * sin(2 * a1 - a2 - a3)*(m2 + 2 * m3)*(4 * I2 - m2 * l2_2)) -
			l1 * l3*m3*a1_v_2*(sin(a1 - a3)*(8 * I1*(m3*l2_2 + 2 * I2) + 2 * l1_2*((m1*m3 - m2_2)*l2_2 + 2 * I2*(m1 + 4 * m2 + 4 * m3))) - l2_2 * sin(a1 -
				2 * a2 + a3)*(m2 + 2 * m3)*((m1 + 2 * m2)*l1_2 + 4 * I1)) + 4 * k3*l1_2*l2_2*m2_2*a3_v + 16 * k3*l1_2*l2_2*m3_2*a3_v + 8 * I2*k3*l1_2*m1*a3_v +
			8 * I1*k3*l2_2*m2*a3_v + 32 * I2*k3*l1_2*m2*a3_v + 32 * I1*k3*l2_2*m3*a3_v + 32 * I2*k3*l1_2*m3*a3_v - 4 * k1*l1*l3*m3*a1_v*(cos(a1 - a3)*(2 * m3*l2_2 + 
				4 * I2) - l2_2 * cos(a1 - 2 * a2 + a3)*(m2 + 2 * m3)) - 16 * I1*I2*g*l3*m3*sin(a3) - 4*I2 * l1_2*l3_2*m3_2*a3_v_2*sin(2 * a1 - 2 * a3) - 
			4 * I1*l2_2*l3_2*m3_2*a3_v_2*sin(2 * a2 - 2 * a3) + 8 * I2*g*l1_2*l3*m2 * 3 * sin(2*a1 - a3) + 8 * I1*g*l2_2*l3*m3_2*sin(2 * a2 - a3) + 2 * k3*l1_2*l2_2*m1*m2*a3_v +
			8 * k3*l1_2*l2_2*m1*m3*a3_v + 24 * k3*l1_2*l2_2*m2*m3*a3_v - 4 * k2*l2*l3*m3*a2_v*(cos(a2 - a3)*((m1 + 3 * m2 + 2 * m3)*l1_2 + 4 * I1) - 
				l1_2 * cos(2 * a1 - a2 - a3)*(m2 + 2 * m3)) - 8 * I1*g*l2_2*l3*m3_2*sin(a3) - 8 * I2*g*l1_2*l3*m3_2*sin(a3) - 4 * k3*l1_2*l2_2*m2_2*a3_v*cos(2 * a1 -
				2 * a2) - 16 * k3*l1_2*l2_2*m3_2*a3*cos(2 * a1 - 2 * a2) - 8 * I2*g*l1_2*l3*m2*m3*sin(a3) - 16 * k3*l1_2*l2_2*m2*m3*a3_v*cos(2 * a1 - 2 * a2) - 
			l1_2 * l2_2*l3_2*m1*m3_2*a3_v_2*sin(2 * a2 - 2 * a3) + l1 * l2_2*l3_2*m2*m3_2*a3_v_2*sin(2 * a1 - 2 * a3) - 2 * l1_2*l2_2*l3_2*m2*m3_2*a3_v_2*sin(2 * a2 -
				2 * a3) + 2 * g*l1_2*l2_2*l3*m1*m3_2*sin(2 * a1 - a3) - g * l1_2*l2_2*l3*m2_2*m3*sin(2 * a1 - a3) + 2 * g*l1_2*l2_2*l3*m2*m3_2*sin(2 * a2 -
					a3) + g * l1_2*l2_2*l3*m2_2 * m3*sin(2 * a2 - a3) + 4 * I2*g*l1_2*l3*m1*m3*sin(2 * a1 - a3) + 8 * I2*g*l1_2*l3*m2*m3*sin(2 * a1 - a3) +
			4 * I1*g*l2_2*l3*m2*m3*sin(2 * a2 - a3) - 2 * g*l1_2*l2_2*l3*m1*m3_2*sin(2 * a1 - 2 * a2 + a3) - 2 * g*l1_2*l2_2*l3*m2*m3_2*sin(2 * a1 - 2 * a2 + 
				a3) - g * l1_2*l2_2*l3*m2_2*m3*sin(2 * a1 - 2 * a2 + a3) + g * l1_2*l2_2*l3*m2_2*m3*sin(a3) - g * l1_2*l2_2*l3*m1*m2*m3*sin(2 * a1 - 2 * a2 + a3)));
		den = (64 * I1*I2*I3 + 8 * I3*l1_2*l2_2*m2_2 + 8 * I1*l2_2*l3_2*m3_2 + 8 * I2*l1_2*l3_2*m3_2 + 32 * I3*l1_2*l2_2*m3_2 + 16 * I2*I3*l1_2*m1 + 16 * I1*I3*l2_2*m2 +
			64 * I2*I3*l1_2*m2 + 16 * I1*I2*l3_2*m3 + 64 * I1*I3*l2_2*m3 + 64 * I2*I3*l1_2*m3 + 4 * I3*l1_2*l2_2*m1*m2 + 4 * I2*l1_2*l3_2*m1*m3 + 16 * I3*l1_2*l2_2*m1*m3 +
			4 * I1*l2_2*l3_2*m2*m3 + 16 * I2*l1_2*l3_2*m2*m3 + 48 * I3*l1_2*l2_2*m2*m3 - 8 * I1*l2_2*l3_2*m3_2*cos(2 * a2 - 2 * a3) - 2 * l1_2*l2_2*cos(2 * a1 - 2 * a2)*(m2 +
				2 * m3)*(m2*m3*l3_2 + 4 * I3*(m2 + 2 * m3)) - 2 * l1_2*l3_2*m3_2*cos(2 * a1 - 2 * a3)*(4 * I2 - m2 * l2_2) + 2 * l1_2*l2_2*l3_2*m1*m3_2 + 6 * l1_2*l2_2*l3_2*m2*m3_2 +
			2 * l1_2*l2_2*l3_2*m2_2*m3 + l1_2 * l2_2*l3_2*m1*m2*m3 - 2 * l1_2*l2_2*l3_2*m1*m3_2*cos(2 * a2 - 2 * a3) - 4 * l1_2*l2_2*l3_2*m2*m3_2*cos(2 * a1 - 2 * a3));
			
		bottomAngleAcceleration = num/den;



		topAngleVelocity += topAngleAcceleration * deltaTime;
		centerAngleVelocity += centerAngleAcceleration * deltaTime;
		bottomAngleVelocity += bottomAngleAcceleration * deltaTime;

		topAngle += topAngleVelocity * deltaTime;
		centerAngle += centerAngleVelocity * deltaTime;
		bottomAngle += bottomAngleVelocity * deltaTime;


	}

public:
	float topRodLength;
	float topBobWeight;
	float centerRodLength;
	float centerBobWeight;
	float bottomRodLength;
	float bottomBobWeight;


private:
	Vector3f rootPosition;
	Vector3f bobPosition[3];

	// for updating motion
	float topAngle = M_PI*0.9; // in this template, this is represented in radian form
	float centerAngle = M_PI; // in this template, this is represented in radian form
	float bottomAngle = M_PI; // in this template, this is represented in radian form

	float topAngleVelocity = 0;
	float centerAngleVelocity = 0;
	float bottomAngleVelocity = 0;

	float topAngleAcceleration = 0;
	float centerAngleAcceleration = 0;
	float bottomAngleAcceleration = 0;

	float sphereSize[3];

	GLuint *textures;

	GLUquadricObj *sphere;
};

#endif