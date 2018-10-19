#ifndef BEABOUTWINDOW_H
#define BEABOUTWINDOW_H

#include "BeMainWindow.h"
#include "BeAboutView.h"
#include "BeMultiStringView.h"
#include "BeUtils.h"

#include <String.h>
#include <Rect.h>
#include <View.h>
#include <Box.h>

class BeAboutWindow : public BeMainWindow
{
	const char *fTitle;
	BString fVersion;

	BeAboutView *fAboutView;
	BeMultiStringView *fInformationView;
	BBox *fAdditionalBox;

	void CreateForm(void);

protected:
	BeAboutView *GetMainView(void) const;
	BeMultiStringView *GetInformationView(void) const;
	BBox *GetAdditionalBox(void) const;

	virtual void FrameResized(float newWidth, float newHeight);

	virtual bool QuitRequested(void);

public:
	BeAboutWindow(const BRect &frame, const char *title, const char *version = G_DEFAULT_VERSION);
	virtual ~BeAboutWindow();
};

#endif // BEABOUTWINDOW_H
