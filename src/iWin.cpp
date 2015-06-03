#include "iWin.h"
#include "iGLView.h"
#include "iMath.h"
#include <math.h>

iWin *iWin::s_win = NULL;
iWin::iWin()
{
	m_hAccelTable = NULL;
	m_nAnimationInterval.QuadPart = 0;
}

iWin::~iWin()
{
	assert(this == s_win);
	s_win = 0;
}

iWin *iWin::getInstance()
{
	if (!s_win)
	{
		s_win = new iWin();
	}
	return s_win;
}

int iWin::run()
{
	// Main message loop:
	MSG msg;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nLast);

	// Initialize instance and cocos2d.
	if (!didFinishLauchingCallback())
	{
		return 0;
	}

	iGLView* glView = iGLView::getInstance();
	glView->centerWindow();
	ShowWindow(glView->m_hWnd, SW_SHOW);

	//
	afterShowGLWindowCallback();

	while (1)
	{
		if (! PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Get current time tick.
			QueryPerformanceCounter(&nNow);

			// If it's the time to draw next frame, draw it, else sleep a while.
			if (nNow.QuadPart - nLast.QuadPart > m_nAnimationInterval.QuadPart)
			{
				nLast.QuadPart = nNow.QuadPart;
				mainLoopCallback();
			}
			else
			{
				Sleep(0);
			}
			continue;
		}

		if (WM_QUIT == msg.message)
		{
			// Quit message loop.
			break;
		}

		// Deal with windows message.
		if (! m_hAccelTable || ! TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;

	return 0;
}

void iWin::setAnimationInterval(double interval)
{
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	m_nAnimationInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
}


//////////////////////////////////////////////////////////////////////////
void setProjectionMatrix()
{
	float xfov = 90;
	float yfov = 2 * atan((float)gWinHeight /gWinHeight) * M_RAD2DEG;

	float screenaspect = (float)gWinHeight / gWinHeight;
	glLoadIdentity();
	gluPerspective(yfov, screenaspect, 2, 8192);
}






//////////////////////////////////////////////////////////////////////////

const int    SCREEN_WIDTH    = 400;
const int    SCREEN_HEIGHT   = 300;
const float  CAMERA_DISTANCE = 3.0f;
const int    TEXT_WIDTH      = 8;
const int    TEXT_HEIGHT     = 13;
const int    IMAGE_WIDTH     = 1024;
const int    IMAGE_HEIGHT    = 1024;
const int    CHANNEL_COUNT   = 4;
const int    DATA_SIZE       = IMAGE_WIDTH * IMAGE_HEIGHT * CHANNEL_COUNT;
const GLenum PIXEL_FORMAT    = GL_BGRA;

GLuint textureId;




bool iWin::didFinishLauchingCallback()
{
	
	return true;
}

void iWin::afterShowGLWindowCallback()
{
	// set gl win
	glViewport(0, 0, gWinWidth/2, gWinHeight/2);

	glMatrixMode(GL_PROJECTION);
	setProjectionMatrix();

	glMatrixMode(GL_MODELVIEW);						// 选择模型观察矩阵
	glLoadIdentity();	

	// init
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// 黑色背景

	glClearDepth(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);						// 启用深度测试
	glDepthFunc(GL_LEQUAL);	

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

typedef struct {
	float Position[3];
	float Color[4];
} Vertex;

const Vertex Vertices[] = {
	{{1, -1, 0}, {1, 0, 0, 1}},
	{{1, 1, 0}, {1, 0, 0, 1}},
	{{-1, 1, 0}, {0, 1, 0, 1}},
	{{-1, -1, 0}, {0, 1, 0, 1}},
	{{1, -1, -1}, {1, 0, 0, 1}},
	{{1, 1, -1}, {1, 0, 0, 1}},
	{{-1, 1, -1}, {0, 1, 0, 1}},
	{{-1, -1, -1}, {0, 1, 0, 1}}
};

const GLubyte Indices[] = {
	// Front
	0, 1, 2,
	2, 3, 0,
	// Back
	4, 6, 5,
	4, 7, 6,
	// Left
	2, 7, 3,
	7, 6, 2,
	// Right
	0, 4, 1,
	4, 1, 5,
	// Top
	6, 2, 1, 
	1, 6, 5,
	// Bottom
	0, 3, 7,
	0, 7, 4    
};
GLuint _depthRenderBuffer;
void setupDepthBuffer() {
	glGenRenderbuffers(1, &_depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 320, 280);    
}

void iWin::mainLoopCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	//glPopMatrix();

	iGLView* glView = iGLView::getInstance();
	glView->swapBuffers();
}