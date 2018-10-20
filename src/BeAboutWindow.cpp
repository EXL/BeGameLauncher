#include "BeAboutWindow.h"
#include "BeAboutStripeView.h"

#include <Window.h>
#include <GroupLayout.h>

#include <Catalog.h>

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "BeGameLauncher"

#define L_ABOUT_VERSION_STRING         B_TRANSLATE("Version: ")

BeAboutWindow::BeAboutWindow(const BRect &frame, const char *title, const char *version)
             : BeMainWindow(frame, title)
{
	fTitle = title;
	fVersion << L_ABOUT_VERSION_STRING;
	fVersion << version;

	SetFlags(Flags() | B_NOT_ZOOMABLE | B_CLOSE_ON_ESCAPE);
	SetType(B_MODAL_WINDOW);

	CreateForm();
}

void
BeAboutWindow::CreateForm(void)
{
	SetLayout(new BGroupLayout(B_VERTICAL));

	fAboutView = new BeAboutView(fTitle, fVersion);
	fInformationView = fAboutView->GetInformationView();
	fAdditionalBox = fAboutView->GetAdditionalBox();
	SetDefaultButton(fAboutView->GetAboutButton());
	AddChild(fAboutView);

	const float gap = BeAboutStripeView::GetIconX();

	SetSizeLimits(Frame().InsetBySelf(gap, 0.0f).Width(),
	              Frame().InsetBySelf(-gap * 4.0f, 0.0f).Width(),
	              Frame().InsetBySelf(0.0f, gap).Height(),
	              Frame().InsetBySelf(0.0f, -gap * 3.0f).Height());
}

BeAboutView *
BeAboutWindow::GetMainView(void) const
{
	return fAboutView;
}

BeMultiStringView *
BeAboutWindow::GetInformationView(void) const
{
	return fInformationView;
}

BBox *
BeAboutWindow::GetAdditionalBox(void) const
{
	return fAdditionalBox;
}

void
BeAboutWindow::FrameResized(float newWidth, float newHeight)
{
	// Fix some draw glitches via resizing about window.
	if(fAboutView)
	{
		fAboutView->GetAboutStripeView()->Invalidate();
	}
	BeMainWindow::FrameResized(newWidth, newHeight);
}

bool
BeAboutWindow::QuitRequested()
{
	return true;
}

BeAboutWindow::~BeAboutWindow(void)
{
	fAboutView->RemoveSelf();
	delete fAboutView;
	fAboutView = NULL;
}
