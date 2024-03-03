//** www.projecthome.ir ** 
// Draw a simple house 

// header ha
#include <windows.h>
#include "stdafx.h"
#include <gl/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include<stdio.h>
#include<stdlib.h>

#include <ctime>

#include <iostream>
#include <agents.h>
#include <ppltasks.h>
#include <string>


using namespace std;


// tavabe
void init(void) // in tabe range zamine va mokhtasate did ra namayesh midahad
{
	glClearColor(1.0, 1, 1, 0); // range background be soorate (ghermez, sabz, abi, meghdare alpha) *adadha beyne 0.0 - 1.0
	glMatrixMode(GL_PROJECTION); // mokhtasat did ya projection
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}

struct Position
{
	int X;
	int Y;
	int Z;
};
void drawQuad(SIZE size, Position pos)
{
	glBegin(GL_QUADS);
	glVertex2i(pos.X, pos.Y);//top left
	glVertex2i(pos.X, pos.Y + size.cy);//bottom left
	glVertex2i(pos.X + size.cx, pos.Y + size.cy);	//bottom right
	glVertex2i(pos.X + size.cx, pos.Y);	//top right
	glEnd();
}


void Draw_House(void) // in tabe khane ra tarsim mikonad
{
	glClear(GL_COLOR_BUFFER_BIT); // range khotoot khatha
	glColor3f(0.0, 0.0, 0);
}

template <class T>
void call_after(const T& callback, unsigned int timeInMs)
{
	concurrency::task_completion_event<void> tce;
	auto call = new concurrency::call<int>(
		[callback, tce](int)
	{
		callback();
		tce.set();
	});

	auto timer = new concurrency::timer<int>(timeInMs, 0, call, false);
	concurrency::task<void> event_set(tce);
	event_set.then([timer, call]()
	{
		delete call;
		delete timer;
	});

	timer->start();
}
void DoEvents()
{
	MSG msg;
	BOOL result;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		result = ::GetMessage(&msg, NULL, 0, 0);
		if (result == 0) // WM_QUIT
		{
			::PostQuitMessage(msg.wParam);
			break;
		}
		else if (result == -1)
		{
			// Handle errors/exit application, etc.
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}
bool flagUpDown = true;
bool flagRightLeft = true;
bool flagHorVert = true;
int lastY = 0;
int lastX = 0;
int step = 0;

Position randomPos(int w, int h)
{
	int rrW = rand() % 100;
	int rrH = rand() % 100;
	Position pp;
	pp.X = (rrW * w) / 100;
	pp.Y = (rrH * h) / 100;
	return pp;
}

void printtext(Position pos, SIZE si, string String)
{
	//(x,y) is from the bottom left of the window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, si.cx, 0, si.cy, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2i(pos.X, pos.Y);
	for (int i = 0; i < String.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void GaimMain()
{
	SIZE ss;
	ss.cx = 25;
	ss.cy = 25;
	SIZE ssT;
	ssT.cx = 10;
	ssT.cy = 10;
	int i = 0;
	int score = 0;
	int level = 1;
	Position targetPos;
	Position textPos;
	Position textPos2;
	SIZE sT;

	textPos.X = 40;
	textPos.Y = 47;

	textPos2.X = 40;
	textPos2.Y = 44;

	sT.cx = 50;
	sT.cy = 50;
	targetPos = randomPos(600, 500);

	while (true)
	{
		i += step;
		glClear(GL_COLOR_BUFFER_BIT); // range khotoot khatha
		glColor3f(0.0, 0.0, 0);
		drawQuad(ssT, targetPos);
		printtext(textPos, sT, "Score : " + to_string(score));
		printtext(textPos2, sT, "Level : " + to_string(level));
		Position pp;
		

		if (flagHorVert)
		{
			pp.Y = lastY;
			if (flagRightLeft)
				pp.X = i;
			else
				pp.X = i;
			lastX = pp.X;
		}
		else
		{
			pp.X = lastX;
			if (flagUpDown)
				pp.Y = i;
			else
				pp.Y = i;
			lastY = pp.Y;
		}
		drawQuad(ss, pp);
		if (pp.X + 25 > targetPos.X && pp.X < targetPos.X + 10)
		{
			if (pp.Y + 25 > targetPos.Y && pp.Y < targetPos.Y + 10)
			{
				targetPos = randomPos(600, 500);
				score += 10;
				if (score > 100)
				{
					level++;
					score = 0;
				}
			}
		}
		Sleep(10 / (level >= 10 ? 10 : level));
		DoEvents();
		glFlush();
		if (pp.X + 25 > 600 || pp.X < 0 || pp.Y + 25>500 || pp.Y < 0)
		{
			MessageBox(
				NULL,
				(LPCWSTR)L"Game Over...",
				(LPCWSTR)L"Your are fail.",
				MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
			);
			step = 0;
			break;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			// the 'Up' arrow key is currently being held down
			flagUpDown = true;
			flagHorVert = false;
			i = lastY;
			step = 1;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			flagUpDown = false;
			flagHorVert = false;
			i = lastY;
			step = -1;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			flagRightLeft = true;
			flagHorVert = true;
			i = lastX;
			step = 1;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			flagRightLeft = false;
			flagHorVert = true;
			i = lastX;
			step = -1;
		}
	}
}
void main(int argc, char** argv) // tabe main
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);  // makane panjere dar safhe namayesh
	glutInitWindowSize(600, 500); // size panjere
	glutCreateWindow("snake game");
	init();
	glutDisplayFunc(Draw_House);
	GaimMain();
	//glutMainLoop();
}
