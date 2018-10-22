#include "BeApp.h"

BeApp::BeApp(const char *signature)
     : BApplication(signature), gMainWindow(NULL)
{

}

thread_id
BeApp::Run(void)
{
	if(gMainWindow != NULL)
	{
		gMainWindow->Show();
	}
	return BApplication::Run();
}

void
BeApp::SetMainWindow(BWindow *window)
{
	gMainWindow = window;
}

BeApp::~BeApp(void)
{

}
