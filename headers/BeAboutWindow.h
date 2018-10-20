#ifndef BEABOUTWINDOW_H
#define BEABOUTWINDOW_H

#include "BeMainWindow.h"
#include "BeAboutView.h"
#include "BeMultiStringView.h"
#include "BeUtils.h"

#include <String.h>
#include <Rect.h>
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
	virtual void FrameResized(float newWidth, float newHeight);

	virtual bool QuitRequested(void);

public:
	explicit BeAboutWindow(const BRect &frame, const char *title, const char *version = G_DEFAULT_VERSION);
	virtual ~BeAboutWindow();

	BeAboutView *GetMainView(void) const;
	BeMultiStringView *GetInformationView(void) const;
	BBox *GetAdditionalBox(void) const;
};

#endif // BEABOUTWINDOW_H
