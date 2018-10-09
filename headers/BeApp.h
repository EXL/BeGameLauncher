#ifndef BEAPP_H
#define BEAPP_H

#include <Application.h>
#include <Window.h>

class BeApp : public BApplication
{
	BWindow *gMainWindow;
public:
	BeApp(const char *signature);
	void SetMainWindow(BWindow *window);
	thread_id Run();
};

#endif // BEAPP_H
