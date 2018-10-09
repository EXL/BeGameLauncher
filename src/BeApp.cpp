#include "BeApp.h"

BeApp::BeApp(const char *signature)
	: BApplication(signature)
{

}

void
BeApp::SetMainWindow(BWindow *window)
{
	gMainWindow = window;
}

thread_id
BeApp::Run()
{
	if(gMainWindow)
	{
		gMainWindow->Show();
	}
	BApplication::Run();
}
