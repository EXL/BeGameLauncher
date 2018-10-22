#include "BeMainWindow.h"

#include <Application.h>

BeMainWindow::BeMainWindow(const BRect &frame, const char *title)
              : BWindow(frame, title, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{

}

void
BeMainWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		default:
		{
			BWindow::MessageReceived(msg);
		}
	}
}

bool
BeMainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

BeMainWindow::~BeMainWindow(void)
{

}
