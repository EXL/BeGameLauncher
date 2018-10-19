#ifndef BEAPP_H
#define BEAPP_H

#include <OS.h>
#include <Window.h>
#include <Application.h>

class BeApp : public BApplication
{
	BWindow *gMainWindow;

public:
	BeApp(const char *signature);
	virtual ~BeApp();

	virtual thread_id Run();

	void SetMainWindow(BWindow *window);
};

#endif // BEAPP_H
