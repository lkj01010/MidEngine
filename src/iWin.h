#ifndef __IWIN_H__
#define __IWIN_H__

#include "stdC.h"

class iWin
{
public:
					iWin();
	virtual		~iWin();

	static iWin *	getInstance();

	int run();
	void			setAnimationInterval(double interval);

public:
	bool			didFinishLauchingCallback();
	void			afterShowGLWindowCallback();
	void			mainLoopCallback();

protected:
	static iWin			*s_win;
	LARGE_INTEGER	m_nAnimationInterval;


protected:
	HACCEL				m_hAccelTable;	// useless
};


#endif
