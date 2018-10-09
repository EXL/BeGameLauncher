#ifndef BEMAINWINDOW_H
#define BEMAINWINDOW_H

#include <Window.h>
#include <Message.h>
#include <Rect.h>

class BeMainWindow : public BWindow
{
public:
	BeMainWindow(const BRect &frame, const char *title);
	void MessageReceived(BMessage *msg);
	bool QuitRequested(void);
	virtual void CreateForm(void) = 0;
};

#endif // BEMAINWINDOW_H
