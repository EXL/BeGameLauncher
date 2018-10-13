#ifndef BEABOUTWINDOW_H
#define BEABOUTWINDOW_H

#include "BeMainWindow.h"

#include <String.h>
#include <Rect.h>
#include <View.h>

class BeAboutWindow : public BeMainWindow
{
	const char *fTitle;
	BString fVersion;

	BView *fMainView;
	BView *fTextView;

protected:
	BView *GetMainView() const;
	BView *GetTextView() const;

public:
	BeAboutWindow(const BRect &frame, const char *title, const char *version = "1.0.0");

	virtual void CreateForm();
	virtual void SetAboutText();

	virtual bool QuitRequested(void);

	static float GetStripeWidth();
	static float GetSmallFontSize();
};

#endif // BEABOUTWINDOW_H
