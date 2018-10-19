#ifndef BEMAINWINDOW_H
#define BEMAINWINDOW_H

#include <Window.h>
#include <Message.h>
#include <Rect.h>

class BeMainWindow : public BWindow
{
protected:
	virtual void CreateForm(void) = 0;

	virtual void MessageReceived(BMessage *msg);

	virtual bool QuitRequested();

public:
	BeMainWindow(const BRect &frame, const char *title);
	virtual ~BeMainWindow();
};

#endif // BEMAINWINDOW_H
