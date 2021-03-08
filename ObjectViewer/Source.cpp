#define _USE_MATH_DEFINES

#include <windows.h>
#include <gl/gl.h>
#include <cmath>

#pragma comment(lib, "opengl32.lib")

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

HWND hwnd;

void Rotate(float anglex, float angley) {
	glRotatef(anglex, 0, 1, 0);
	glRotatef(angley, 1, 0, 0);
}

void Rotation() {
	POINT cursor;
	RECT size;
	GetWindowRect(hwnd, &size);
	GetCursorPos(&cursor);

	if (GetForegroundWindow() != hwnd) return;
	if (cursor.x > size.right || cursor.x < size.left || cursor.y > size.bottom || cursor.y < size.top) return;

	int lengthx = size.right - size.left;
	int lengthy = size.bottom - size.top;

	POINT center;
	center.x = lengthx / 2 + size.left;
	center.y = lengthy / 2 + size.top;

	Rotate((float)(cursor.x - center.x) * (270 / (float)lengthx), (float)(cursor.y - center.y) * (180 / (float)lengthy));
}

void DrawCubeSide(float x, float y, float z, float anglex, float angley, float anglez) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(anglex, 1, 0, 0);
	glRotatef(angley, 0, 1, 0); 
	glRotatef(anglez, 0, 0, 1);

	glBegin(GL_QUADS);
	glColor3f(0, 0, 1);
	glNormal3f(1, 1, 3);
	glVertex3f(1, 1, 0);

	glColor3f(1, 0, 0);
	glNormal3f(1, -1, 3);
	glVertex3f(1, -1, 0);

	glColor3f(1, 1, 0);
	glNormal3f(-1, -1, 3);
	glVertex3f(-1, -1, 0);

	glColor3f(0, 1, 0);
	glNormal3f(-1, 1, 3);
	glVertex3f(-1, 1, 0);
	glEnd();

	glPopMatrix();
}

void DrawCube() {
	DrawCubeSide(0, 0, 1, 0, 0, 0);
	DrawCubeSide(1, 0, 0, 0, 90, 0);
	DrawCubeSide(0, 0, -1, 0, 180, 0);
	DrawCubeSide(-1, 0, 0, 0, -90, 0);
	DrawCubeSide(0, 1, 0, -90, 0, 0);
	DrawCubeSide(0, -1, 0, 90, 0, 0);
}

void DrawPiramidSide(float x, float y, float z, float anglex, float angley, float anglez) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angley, 0, 1, 0);
	glRotatef(anglex, 1, 0, 0);
	glRotatef(anglez, 0, 0, 1);

	glBegin(GL_TRIANGLES);
	glColor3f(0, 1, 0);
	glNormal3f(0, 1, 3);
	glVertex3f(0, 1, 0);

	glNormal3f(1, -1, 3);
	glColor3f(1, 0, 0);
	glVertex3f(1, -1, 0);

	glColor3f(1, 1, 0);
	glNormal3f(-1, -1, 3);
	glVertex3f(-1, -1, 0);
	glEnd();

	glPopMatrix();
}

void DrawPiramid() {
	DrawCubeSide(0, -1, 0, 90, 0, 0);
	DrawPiramidSide(0, -0.135, 0.5, -30, 0, 0);
	DrawPiramidSide(0.5, -0.135, 0, -30, 90, 0);
	DrawPiramidSide(0, -0.135, -0.5, -30, 180, 0);
	DrawPiramidSide(-0.5, -0.135, 0, -30, -90, 0);
}

void DrawCone(int height) {
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);

	int count = 100;
	float radius = 1;
	float step = M_PI * 2 / count;
	float currentStep = 0;
	for (int ind = 0; ind < count; ind++) {
		glBegin(GL_TRIANGLE_STRIP);

		//center
		glNormal3f(0, 0, -1);
		glVertex3f(0, 0, 0);


		glNormal3f(cos(currentStep + step / 2), sin(currentStep + step / 2), -1);
		glVertex3f(cos(currentStep), sin(currentStep), 0);
		currentStep += step;
		glVertex3f(cos(currentStep), sin(currentStep), 0);

		glNormal3f(cos(currentStep - step / 2), sin(currentStep - step / 2), 1);
		glVertex3f(cos(currentStep - step), sin(currentStep - step), 0);
		glVertex3f(cos(currentStep), sin(currentStep), 0);

		//top
		glNormal3f(cos(currentStep - step / 2), sin(currentStep - step / 2), 0.5);
		glVertex3f(0, 0, height);

		glEnd();
	}

	glPopMatrix();
}

void DrawCylinder(int height) {
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);

	int count = 100;
	float radius = 1;
	float step = M_PI * 2 / count;
	float currentStep = 0;
	for (int ind = 0; ind < count; ind++) {
		glBegin(GL_TRIANGLE_STRIP);

		glNormal3f(0, 0, -1);
		glVertex3f(0, 0, 0);

		glNormal3f(cos(currentStep + step / 2), sin(currentStep + step / 2), -1);
		glVertex3f(cos(currentStep), sin(currentStep), 0);
		currentStep += step;
		glVertex3f(cos(currentStep), sin(currentStep), 0);
		glEnd();

		glBegin(GL_QUADS);
		glNormal3f(cos(currentStep - step / 2), sin(currentStep - step / 2), -1);
		glVertex3f(cos(currentStep - step), sin(currentStep - step), 0);
		glVertex3f(cos(currentStep), sin(currentStep), 0);

		glNormal3f(cos(currentStep - step / 2), sin(currentStep - step / 2), 1);
		glVertex3f(cos(currentStep), sin(currentStep), height);
		glVertex3f(cos(currentStep - step), sin(currentStep - step), height);
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);

		//center
		glNormal3f(0, 0, 1);
		glVertex3f(0, 0, height);

		glNormal3f(cos(currentStep + step / 2), sin(currentStep + step / 2), 1);
		glVertex3f(cos(currentStep - step), sin(currentStep - step), height);
		glVertex3f(cos(currentStep), sin(currentStep), height);
		glEnd();
	}

	glPopMatrix();
}

void WindowResize(int x, int y) {
	glViewport(0, 0, x, y);
	float k = x / float(y);
	float size = 0.1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-k * size, k * size, -size, size, 2, 100);
	glMatrixMode(GL_MODELVIEW);
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
#pragma region Window
	WNDCLASSEX wcex;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	float theta = 0.0f;

	/* register window class */
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"GLSample";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;

	if (!RegisterClassEx(&wcex))
		return 0;

	/* create main window */
	hwnd = CreateWindowEx(0,
		L"GLSample",
		L"OpenGL Sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		700,
		700,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);
#pragma endregion
	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING); // встановлення властивостей освітлення
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	WindowResize(700, 700);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -80);


	float tetha = 0;
	/* program main loop */
	while (!bQuit)
	{
		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/* OpenGL animation code goes here */

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glPushMatrix();

			Rotation();

			//glTranslatef(0, -1, 0);
			
			//glColor3f(1, 0.5, 0);
			//DrawCylinder(2);
			//glTranslatef(0, 2, 0);
			DrawCone(2);
			//glTranslatef(0, -3, 0);
			//DrawCube();
			/*glTranslatef(0, 2, 0);*/
			//DrawPiramid();

			glPopMatrix();

			SwapBuffers(hDC);
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		return 0;

	case WM_SIZE:
		WindowResize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hwnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}

