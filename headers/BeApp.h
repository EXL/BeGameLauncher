#ifndef BEAPP_H
#define BEAPP_H

#include <Application.h>
#include <Window.h>

class BeApp : public BApplication
{
	BWindow *gMainWindow;
public:
	BeApp(const char *signature);
	virtual thread_id Run();
	void SetMainWindow(BWindow *window);
};

#endif // BEAPP_H
