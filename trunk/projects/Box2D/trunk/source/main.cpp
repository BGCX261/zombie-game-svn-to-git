/*
* Copyright (c) 2006 Erin Catto http://www.gphysics.com
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies.
* Erin Catto makes no representations about the suitability 
* of this software for any purpose.  
* It is provided "as is" without express or implied warranty.
*/

#include <windows.h>
#include <gl\gl.h>
#include <string.h>
#include <sstream>
#include "glut.h"

#include "World.h"
#include "Body.h"
#include "Joint.h"

namespace
{
	bool bruteForceHitCalc = false;

	Body bodies[200];
	Joint joints[100];
	
	Body* bomb = NULL;

	float mindt = 0.05f / 60.0f;
	float dt = 1.0f / 60.0f;

	int numBodies = 0;
	int numJoints = 0;

	int demoIndex = 0;
	int child = 0;
	AutoMatic::QuadtreeNode * viewNode = 0;

	World world(Vec2(0.0f, -10.0f), 10, 256, 8);
}

void DrawText(int x, int y, char const *string)
{
	int len, i;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void DrawBody(Body* body)
{
	if (body == bomb)
	{
		glColor3f(0.4f, 0.9f, 0.4f);
	}
	
	
	if(body->_boundingNode == viewNode)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	else
	{
		glColor3f(0.8f, 0.8f, 0.9f);
	}

	glBegin(GL_LINE_LOOP);
		glVertex2f(body->_v1.x, body->_v1.y);
		glVertex2f(body->_v2.x, body->_v2.y);
		glVertex2f(body->_v3.x, body->_v3.y);
		glVertex2f(body->_v4.x, body->_v4.y);
	glEnd();

	glColor3f(0.10f, 0.10f, 0.5f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(body->_minX / 9.9f, body->_minY / 9.9f);
		glVertex2f(body->_maxX / 9.9f, body->_minY / 9.9f);
		glVertex2f(body->_maxX / 9.9f, body->_maxY / 9.9f);
		glVertex2f(body->_minX / 9.9f, body->_maxY / 9.9f);
	glEnd();
}

void DrawJoint(Joint* joint)
{
	Body* b1 = joint->body1;
	Body* b2 = joint->body2;

	Mat22 R1(b1->rotation);
	Mat22 R2(b2->rotation);

	Vec2 x1 = b1->position;
	Vec2 p1 = x1 + R1 * joint->localAnchor1;

	Vec2 x2 = b2->position;
	Vec2 p2 = x2 + R2 * joint->localAnchor2;

	glColor3f(0.5f, 0.5f, 0.8f);
	glBegin(GL_LINES);
	glVertex2f(x1.x, x1.y);
	glVertex2f(p1.x, p1.y);
	glVertex2f(x2.x, x2.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void LaunchBomb()
{
	if (!bomb)
	{
		bomb = bodies + numBodies;
		bomb->Set(Vec2(1.0f, 1.0f), 50.0f);
		bomb->friction = 0.2f;
		world.Add(bomb);
		++numBodies;
	}

	bomb->position.Set(Random(-15.0f, 15.0f), 15.0f);
	bomb->rotation = Random(-1.5f, 1.5f);
	bomb->velocity = -1.5f * bomb->position;
	bomb->angularVelocity = Random(-20.0f, 20.0f);
}

// Single box
void Demo1(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(1.0f, 1.0f), 100.0f);
	b->position.Set(0.0f, 4.0f);
	world.Add(b);
	++b; ++numBodies;
}

// Varying friction coefficients
void Demo2(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(-2.0f, 11.0f);
	b->rotation = -0.25f;
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(0.25f, 1.0f), FLT_MAX);
	b->position.Set(5.25f, 9.5f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(2.0f, 7.0f);
	b->rotation = 0.25f;
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(0.25f, 1.0f), FLT_MAX);
	b->position.Set(-5.25f, 5.5f);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(13.0f, 0.25f), FLT_MAX);
	b->position.Set(-2.0f, 3.0f);
	b->rotation = -0.25f;
	world.Add(b);
	++b; ++numBodies;

	for (int i = 0; i < 5; ++i)
	{
		b->Set(Vec2(0.5f, 0.5f), 25.0f);
		b->friction = 0.5f * (1.0f - 0.9f / 4.0f * i);
		b->position.Set(-7.5f + 2.0f * i, 14.0f);
		world.Add(b);
		++b; ++numBodies;
	}
}

// A vertical stack
void Demo3(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;

	for (int i = 0; i < 10; ++i)
	{
		b->Set(Vec2(1.0f, 1.0f), 1.0f);
		b->friction = 0.2f;
		float x = Random(-0.1f, 0.1f);
		b->position.Set(x, 0.51f + 1.02f * i);
		world.Add(b);
		++b; ++numBodies;
	}
}

// A vertical stack on an incline
void Demo4(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = -0.1f;
	world.Add(b);
	++b; ++numBodies;

	for (int i = 0; i < 8; ++i)
	{
		b->Set(Vec2(1.0f, 1.0f), 1.0f);
		b->position.Set(0.0f, 0.75f + 1.1f * i);
		world.Add(b);
		++b; ++numBodies;
	}
}

// A pyramid
void Demo5(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;

	Vec2 x(-6.0f, 0.75f);
	Vec2 y;

	for (int i = 0; i < 12; ++i)
	{
		y = x;

		for (int j = i; j < 12; ++j)
		{
			b->Set(Vec2(1.0f, 1.0f), 10.0f);
			b->friction = 0.2f;
			b->position = y;
			world.Add(b);
			++b; ++numBodies;

			y += Vec2(1.125f, 0.0f);
		}

		x += Vec2(0.5625f, 1.125f);
	}
}


// A simple pendulum
void Demo6(Body* b, Joint* j)
{
	Body* b1 = b + 0;
	b1->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b1->friction = 0.2f;
	b1->position.Set(0.0f, -0.5f * b1->width.y);
	b1->rotation = 0.0f;
	world.Add(b1);

	Body* b2 = b + 1;
	b2->Set(Vec2(1.0f, 1.0f), 100.0f);
	b2->friction = 0.2f;
	b2->position.Set(9.0f, 11.0f);
	b2->rotation = 0.0f;
	world.Add(b2);

	numBodies += 2;

	j->Set(b1, b2, Vec2(0.0f, 11.0f));
	world.Add(j);

	numJoints += 1;
}

// A teeter
void Demo7(Body* b, Joint* j)
{
	Body* b1 = b + 0;
	b1->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b1->position.Set(0.0f, -0.5f * b1->width.y);
	world.Add(b1);

	Body* b2 = b + 1;
	b2->Set(Vec2(12.0f, 0.25f), 100.0f);
	b2->position.Set(0.0f, 1.0f);
	world.Add(b2);

	Body* b3 = b + 2;
	b3->Set(Vec2(0.5f, 0.5f), 25.0f);
	b3->position.Set(-5.0f, 2.0f);
	world.Add(b3);

	Body* b4 = b + 3;
	b4->Set(Vec2(0.5f, 0.5f), 25.0f);
	b4->position.Set(-5.5f, 2.0f);
	world.Add(b4);

	Body* b5 = b + 4;
	b5->Set(Vec2(1.0f, 1.0f), 100.0f);
	b5->position.Set(5.5f, 15.0f);
	world.Add(b5);

	numBodies += 5;

	j->Set(b1, b2, Vec2(0.0f, 1.0f));
	world.Add(j);

	numJoints += 1;
}

// A suspension bridge
void Demo8(Body* b, Joint* j)
{
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->friction = 0.2f;
	b->position.Set(0.0f, -0.5f * b->width.y);
	b->rotation = 0.0f;
	world.Add(b);
	++b; ++numBodies;

	const int numPlanks = 15;

	for (int i = 0; i < numPlanks; ++i)
	{
		b->Set(Vec2(1.0f, 0.25f), 50.0f);
		b->friction = 0.2f;
		b->position.Set(-8.5f + 1.25f * i, 5.0f);
		world.Add(b);
		++b; ++numBodies;
	}

	float relaxation = 0.80f;

	for (int i = 0; i < numPlanks; ++i)
	{
		j->Set(bodies+i, bodies+i+1, Vec2(-9.125f + 1.25f * i, 5.0f));
		j->relaxation = relaxation;
		world.Add(j);
		++j; ++numJoints;
	}

	j->Set(bodies + numPlanks, bodies, Vec2(-9.125f + 1.25f * numPlanks, 5.0f));
	j->relaxation = relaxation;
	world.Add(j);
	++j; ++numJoints;
}

// Dominos
void Demo9(Body* b, Joint* j)
{
	Body* b1 = b;
	b->Set(Vec2(100.0f, 20.0f), FLT_MAX);
	b->position.Set(0.0f, -0.5f * b->width.y);
	world.Add(b);
	++b; ++numBodies;

	b->Set(Vec2(12.0f, 0.5f), FLT_MAX);
	b->position.Set(-1.5f, 10.0f);
	world.Add(b);
	++b; ++numBodies;

	for (int i = 0; i < 10; ++i)
	{
		b->Set(Vec2(0.2f, 2.0f), 10.0f);
		b->position.Set(-6.0f + 1.0f * i, 11.125f);
		b->friction = 0.1f;
		world.Add(b);
		++b; ++numBodies;
	}

	b->Set(Vec2(14.0f, 0.5f), FLT_MAX);
	b->position.Set(1.0f, 6.0f);
	b->rotation = 0.3f;
	world.Add(b);
	++b; ++numBodies;

	Body* b2 = b;
	b->Set(Vec2(0.5f, 3.0f), FLT_MAX);
	b->position.Set(-7.0f, 4.0f);
	world.Add(b);
	++b; ++numBodies;

	Body* b3 = b;
	b->Set(Vec2(12.0f, 0.25f), 20.0f);
	b->position.Set(-0.9f, 1.0f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b1, b3, Vec2(-2.0f, 1.0f));
	world.Add(j);
	++j; ++numJoints;

	Body* b4 = b;
	b->Set(Vec2(0.5f, 0.5f), 10.0f);
	b->position.Set(-10.0f, 15.0f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b2, b4, Vec2(-7.0f, 15.0f));
	world.Add(j);
	++j; ++numJoints;

	Body* b5 = b;
	b->Set(Vec2(2.0f, 2.0f), 20.0f);
	b->position.Set(6.0f, 2.5f);
	b->friction = 0.1f;
	world.Add(b);
	++b; ++numBodies;

	j->Set(b1, b5, Vec2(6.0f, 2.6f));
	world.Add(j);
	++j; ++numJoints;

	Body* b6 = b;
	b->Set(Vec2(2.0f, 0.2f), 10.0f);
	b->position.Set(6.0f, 3.6f);
	world.Add(b);
	++b; ++numBodies;

	j->Set(b5, b6, Vec2(7.0f, 3.5f));
	world.Add(j);
	++j; ++numJoints;
}

void (*demos[])(Body* b, Joint* j) = {Demo1, Demo2, Demo3, Demo4, Demo5, Demo6, Demo7, Demo8, Demo9};
char* demoStrings[] = {
			"A Single Box",
			"Varying Friction Coefficients",
			"Randomized Stacking",
			"Tilted Stacking",
			"Pyramid Stacking",
			"Simple Pendulum",
			"A Teeter",
			"A Suspension Bridge",
			"Dominos"};

void InitDemo(int index)
{
	world.Clear();
	numBodies = 0;
	numJoints = 0;
	bomb = NULL;

	demoIndex = index;
	demos[index](bodies, joints);
}

int viewNodeBodyCount = 0;

void RenderQuadtreeNode(AutoMatic::Box2DQuadtreeNode * node_)
{
	if(node_ == viewNode)
	{
		glColor3f(1.0f,0.5f,0.0f);
		viewNodeBodyCount = node_->bodies().size();
	}
	else
	{
		glColor3f(1.0f, 1.0f, 1.0f);
	}

	if((node_ == viewNode) || node_->bodies().size() > 2)
	{
		glVertex2f(node_->x() - node_->width(), node_->y() - node_->width());
		glVertex2f(node_->x() + node_->width(), node_->y() + node_->width());
		glVertex2f(node_->x() + node_->width(), node_->y() - node_->width());
		glVertex2f(node_->x() - node_->width(), node_->y() - node_->width());
		glVertex2f(node_->x() + node_->width(), node_->y() + node_->width());
		glVertex2f(node_->x() + node_->width(), node_->y() - node_->width());
		glVertex2f(node_->x() - node_->width(), node_->y() + node_->width());
		glVertex2f(node_->x() - node_->width(), node_->y() - node_->width());
		glVertex2f(node_->x() - node_->width(), node_->y() + node_->width());
		glVertex2f(node_->x() + node_->width(), node_->y() + node_->width());
	}

	AutoMatic::QuadtreeNode ** c = node_->children(false);
	if(c)
	{
		for (int i = 0; i < 4; ++i)
		{
			RenderQuadtreeNode(static_cast<AutoMatic::Box2DQuadtreeNode *>(c[i]));
		}
	}
}

void RenderQuadtree(AutoMatic::Box2DQuadtree & quadtree_)
{
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_LINES);
	RenderQuadtreeNode(static_cast<AutoMatic::Box2DQuadtreeNode *>(quadtree_.root()));
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(1.0f);
}

float timepassed = 0.0f;
void SimulationLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	viewNodeBodyCount = 0;
	RenderQuadtree(world._quadtree);

	DrawText(5, 15, demoStrings[demoIndex]);
	DrawText(5, 45, "Keys: 1-9 Demos, Space to Launch the Bomb");
	DrawText(5, 75, (bruteForceHitCalc ? "Brute-force" : "Quadtree"));
	
	std::stringstream s; s << "Collision Checks: " << world._collisionChecks << " Contacts: " << world._contacts << " Updates:  " << world._numUpdates
		<< " MouseNode bodies: " << viewNodeBodyCount;
	DrawText(5, 90, s.str().c_str());

	world._contacts = 0;
	world._numUpdates = 0;
	world._collisionChecks = 0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, -7.0f, -25.0f);

	timepassed += dt;
	while (timepassed >= mindt)
	{
		world.Step(mindt, bruteForceHitCalc);
		timepassed -= mindt;
	}
	

	for (int i = 0; i < numBodies; ++i)
		DrawBody(bodies + i);

	for (int i = 0; i < numJoints; ++i)
		DrawJoint(joints + i);

	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		{
			InitDemo(key - '1');
		}
		break;
	case 'b':
		{
			bruteForceHitCalc = !bruteForceHitCalc;
		}
		break;
	case 'p':
		{
			if(dt != 0.0f){ dt = 0.0f; }
			else { dt = 1.0f / 60.0f;}
		}
		break;
	case ' ':
		{
			LaunchBomb();
		}
		break;
	case 'u':
		{
			if(viewNode->parent()){ viewNode = viewNode->parent(); }
		}
		break;
	case 'd':
		{
			viewNode->children(true);
			if(viewNode->child(AutoMatic::QuadtreeNode::pXpY))
			{
				viewNode = viewNode->child(AutoMatic::QuadtreeNode::pXpY);
				child = 0;
			}
		}
		break;
	case 'n':
		{
			child++; if(child >= 4){ child = 0; }
			if(viewNode->parent())
			{
				viewNode = viewNode->parent()->child((AutoMatic::QuadtreeNode::ChildId)child);
			}
		}
		break;
	}
}

void ArrowKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	}
}

void Reshape(int width, int height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)width/(float)height, 0.1, 100.0);
}

void MouseFunc(int button, int state, int x, int y)
{
	/*if(state == GLUT_UP)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			if(mouseLevel > 1)
			{
				mouseLevel /= 2;
			}
		}
		else if(button == GLUT_RIGHT_BUTTON)
		{
			if(mouseLevel < 256)
			{
				mouseLevel *= 2;
			}
		}
	}*/
}

void MouseMotion(int x, int y)
{
	//mouseNode = world._quadtree.find(x, y, mouseLevel);
}

void main(int argc, char** argv)
{
	InitDemo(0);
	
	viewNode = world._quadtree.root();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Box2D");

	glutReshapeFunc(Reshape);
	glutDisplayFunc(SimulationLoop);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(ArrowKeys);
	glutIdleFunc(SimulationLoop);
	glutMouseFunc(MouseFunc);
	glutPassiveMotionFunc(MouseMotion);

	glutMainLoop();
}
