#include "iMacros.h"
#include "iGLView.h"
#include "iCommon.h"




static void SetupPixelFormat(HDC hDC)
{
	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size
		1,                          // version
		PFD_SUPPORT_OPENGL |        // OpenGL window
		PFD_DRAW_TO_WINDOW |        // render to window
		PFD_DOUBLEBUFFER,           // support double-buffering
		PFD_TYPE_RGBA,              // color type
		32,                         // preferred color depth
		0, 0, 0, 0, 0, 0,           // color bits (ignored)
		0,                          // no alpha buffer
		0,                          // alpha bits (ignored)
		0,                          // no accumulation buffer
		0, 0, 0, 0,                 // accum bits (ignored)
		24,                         // depth buffer
		8,                          // no stencil buffer
		0,                          // no auxiliary buffers
		PFD_MAIN_PLANE,             // main layer
		0,                          // reserved
		0, 0, 0,                    // no layer, visible, damage masks
	};

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}

static bool glew_dynamic_binding()
{
	const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

	// If the current opengl driver doesn't have framebuffers methods, check if an extension exists
	if (glGenFramebuffers == NULL)
	{
		iLog("OpenGL: glGenFramebuffers is NULL, try to detect an extension");
		if (strstr(gl_extensions, "ARB_framebuffer_object"))
		{
			iLog("OpenGL: ARB_framebuffer_object is supported");

			glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbuffer");
			glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbuffer");
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffers");
			glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffers");
			glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorage");
			glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameteriv");
			glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebuffer");
			glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebuffer");
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffers");
			glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffers");
			glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatus");
			glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1D");
			glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2D");
			glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3D");
			glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbuffer");
			glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
			glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
		}
		else
			if (strstr(gl_extensions, "EXT_framebuffer_object"))
			{
				iLog("OpenGL: EXT_framebuffer_object is supported");
				glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbufferEXT");
				glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbufferEXT");
				glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
				glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffersEXT");
				glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorageEXT");
				glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameterivEXT");
				glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebufferEXT");
				glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebufferEXT");
				glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
				glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffersEXT");
				glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
				glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1DEXT");
				glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
				glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3DEXT");
				glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
				glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
				glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmapEXT");
			}
			else
			{
				iLog("OpenGL: No framebuffers extension is supported");
				iLog("OpenGL: Any call to Fbo will crash!");
				return false;
			}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
// impliment CCEGLView
//////////////////////////////////////////////////////////////////////////
static iGLView* s_pMainWindow = NULL;
static const WCHAR* s_windowClassName = L"MidWin32";
iGLView* iGLView::s_glView = NULL;
static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (s_pMainWindow && s_pMainWindow->m_hWnd == hWnd)
	{
		return s_pMainWindow->WindowProc(uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}


iGLView::iGLView()
{
	m_hWnd = NULL;
	m_hDC = NULL;
	m_hRC = NULL;
}

iGLView::~iGLView()
{
}

bool iGLView::initGL()
{
	m_hDC = GetDC(m_hWnd);
	SetupPixelFormat(m_hDC);
	//SetupPalette();
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);

	// check OpenGL version at first
	const GLubyte* glVersion = glGetString(GL_VERSION);
	iLog("OpenGL version = %s", glVersion);

	if ( atof((const char*)glVersion) < 1.5 )
	{
		char strComplain[256] = {0};
		sprintf(strComplain,
			"OpenGL 1.5 or higher is required (your version is %s). Please upgrade the driver of your video card.",
			glVersion);
		iMessageBox(strComplain, "OpenGL version too old");
		return false;
	}

	GLenum GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		iMessageBox((char *)glewGetErrorString(GlewInitResult), "OpenGL error");
		return false;
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
	{
		iLog("Ready for GLSL");
	}
	else
	{
		iLog("Not totally ready :(");
	}

	if (glewIsSupported("GL_VERSION_2_0"))
	{
		iLog("Ready for OpenGL 2.0");
	}
	else
	{
		iLog("OpenGL 2.0 not supported");
	}

	if(glew_dynamic_binding() == false)
	{
		iMessageBox("No OpenGL framebuffer support. Please upgrade the driver of your video card.", "OpenGL error");
		return false;
	}

	// Enable point size by default on windows.
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	return true;
}

void iGLView::destroyGL()
{
	if (m_hDC != NULL && m_hRC != NULL)
	{
		// deselect rendering context and delete it
		wglMakeCurrent(m_hDC, NULL);
		wglDeleteContext(m_hRC);
	}
}


iGLView *iGLView::getInstance()
{
	if (s_glView == NULL)
	{
		s_glView = new iGLView();
		if(!s_glView->create())
		{
			delete s_glView;
			s_glView = NULL;
		}
	}

	return s_glView;
}

bool iGLView::create()
{
	bool bRet = false;
	do
	{
		BREAK_IF(m_hWnd);

		HINSTANCE hInstance = GetModuleHandle( NULL );
		WNDCLASS  wc;        // Windows Class Structure

		// Redraw On Size, And Own DC For Window.
		wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc    = _WindowProc;                    // WndProc Handles Messages
		wc.cbClsExtra     = 0;                              // No Extra Window Data
		wc.cbWndExtra     = 0;                                // No Extra Window Data
		wc.hInstance      = hInstance;                        // Set The Instance
		wc.hIcon          = LoadIcon( NULL, IDI_WINLOGO );    // Load The Default Icon
		wc.hCursor        = LoadCursor( NULL, IDC_ARROW );    // Load The Arrow Pointer
		wc.hbrBackground  = NULL;                           // No Background Required For GL
		wc.lpszMenuName   = NULL;                         // 指定窗口菜单资源的指针
		wc.lpszClassName  = s_windowClassName;               // Set The Class Name

		BREAK_IF(! RegisterClass(&wc) && 1410 != GetLastError());

		// center window position
		RECT rcDesktop;
		GetWindowRect(GetDesktopWindow(), &rcDesktop);

		WCHAR wszBuf[50] = {0};
		MultiByteToWideChar(CP_UTF8, 0, "viewName", -1, wszBuf, sizeof(wszBuf));

		// create window
		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,    // Extended Style For The Window
			s_windowClassName,                                    // Class Name
			wszBuf,                                                // Window Title
			WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX,        // Defined Window Style
			0, 0,                                                // Window Position
			//TODO: Initializing width with a large value to avoid getting a wrong client area by 'GetClientRect' function.
			gWinWidth,                                               // Window Width
			gWinHeight,                                               // Window Height
			NULL,                                                // No Parent Window
			NULL,                                                // No Menu
			hInstance,                                            // Instance
			NULL );

		BREAK_IF(! m_hWnd);

		bRet = initGL();
		if(!bRet) destroyGL();
		BREAK_IF(!bRet);

		s_pMainWindow = this;
		bRet = true;
	} while (0);

	return bRet;
}

void iGLView::centerWindow()
{
	if (! m_hWnd)
	{
		return;
	}

	RECT rcDesktop, rcWindow;
	GetWindowRect(GetDesktopWindow(), &rcDesktop);

	// substract the task bar
	HWND hTaskBar = FindWindow(TEXT("Shell_TrayWnd"), NULL);
	if (hTaskBar != NULL)
	{
		APPBARDATA abd;

		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd = hTaskBar;

		SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
		SubtractRect(&rcDesktop, &rcDesktop, &abd.rc);
	}
	GetWindowRect(m_hWnd, &rcWindow);

	int offsetX = rcDesktop.left + (rcDesktop.right - rcDesktop.left - (rcWindow.right - rcWindow.left)) / 2;
	offsetX = (offsetX > 0) ? offsetX : rcDesktop.left;
	int offsetY = rcDesktop.top + (rcDesktop.bottom - rcDesktop.top - (rcWindow.bottom - rcWindow.top)) / 2;
	offsetY = (offsetY > 0) ? offsetY : rcDesktop.top;

	SetWindowPos(m_hWnd, 0, offsetX, offsetY, 0, 0, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}


LRESULT iGLView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bProcessed = FALSE;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		break;

	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}

	return 0;
}

void iGLView::swapBuffers()
{
	if (m_hDC != NULL)
	{
		::SwapBuffers(m_hDC);
	}
}
