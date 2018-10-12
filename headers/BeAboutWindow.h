#ifndef BEABOUTWINDOW_H
#define BEABOUTWINDOW_H

#include "BeMainWindow.h"

class BeAboutWindow : public BeMainWindow
{
public:
	BeAboutWindow(const BRect &frame, const char *title);
	virtual void CreateForm();
	virtual bool QuitRequested(void);

	static float GetStripeWidth();
};

#endif // BEABOUTWINDOW_H
