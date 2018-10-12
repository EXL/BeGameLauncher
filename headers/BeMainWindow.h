#ifndef BEMAINWINDOW_H
#define BEMAINWINDOW_H

#include <Window.h>
#include <Message.h>
#include <Rect.h>

class BeMainWindow : public BWindow
{
public:
	BeMainWindow(const BRect &frame, const char *title);
	virtual void MessageReceived(BMessage *msg);
	virtual void CreateForm(void) = 0;
	virtual bool QuitRequested(void);
};

#endif // BEMAINWINDOW_H
