#ifndef BEAPP_H
#define BEAPP_H

#include <OS.h>
#include <Window.h>
#include <Application.h>

class BeApp : public BApplication
{
	BWindow *gMainWindow;

public:
	explicit BeApp(const char *signature);
	virtual ~BeApp(void);

	virtual thread_id Run(void);

	void SetMainWindow(BWindow *window);
};

#endif // BEAPP_H
