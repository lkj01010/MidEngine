#include "iGLView.h"
#include "iWin.h"

int main(int argc, char* argv[])
{
	iWin *win = iWin::getInstance();

	win->setAnimationInterval(1/60.f);

	win->run();
	


	return 0;
}