#ifndef __IGLVIEW_H__
#define __IGLVIEW_H__


#include "stdC.h"
#include "GL/glew.h"
#include <GL/glu.h>



static int gWinWidth	 =	320;
static int gWinHeight =	280;


class iGLView
{
public:
	iGLView();
	~iGLView();

public:
	static iGLView* getInstance();
	bool initGL();
	void destroyGL();

	void centerWindow();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	void swapBuffers();

protected:
	bool create();
	

protected:
	static iGLView			*s_glView;

public:
	HWND m_hWnd;
	HDC  m_hDC;
	HGLRC m_hRC;
};


#endif