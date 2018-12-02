/************************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2018 EXL
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 ** SOFTWARE.
 ************************************************************************************/

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

	fAboutView = NULL;
	fInformationView = NULL;
	fAdditionalBox = NULL;

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
	SetSizeLimits(Bounds().Width(),
	              Frame().InsetBySelf(-gap * 4.0f, 0.0f).Width(),
	              Bounds().Height(),
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
	// Fix some draw glitches when resizing about window.
	if(fAboutView != NULL)
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
