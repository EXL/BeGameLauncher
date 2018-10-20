#ifndef BELAUNCHERVIEW_H
#define BELAUNCHERVIEW_H

#include <GroupView.h>
#include <TextControl.h>
#include <Box.h>
#include <Button.h>
#include <StringView.h>

class BeLauncherView : public BGroupView
{
	BTextControl *fTextControl;
	BButton *fBrowseButton;
	BBox *fAdditionalBox;
	BStringView *fStatusStringView;
	BButton *fAboutButton;
	BButton *fRunButton;
	BButton *fExitButton;

public:
	explicit BeLauncherView(bool showIcon = false);
	virtual ~BeLauncherView(void);

	BTextControl *GetTextControl(void) const;
	BButton *GetBrowseButton(void) const;
	BBox *GetAdditionalBox(void) const;
	BStringView *GetStatusStringView(void) const;
	BButton *GetAboutButton(void) const;
	BButton *GetRunButton(void) const;
	BButton *GetExitButton(void) const;

	static const char *GetReadyString(void);
};

#endif // BELAUNCHERVIEW_H
