#include "BeApp.h"

BeApp::BeApp(const char *signature)
     : BApplication(signature)
{

}

thread_id
BeApp::Run(void)
{
	if(gMainWindow)
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
