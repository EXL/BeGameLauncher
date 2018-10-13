#include "BeAboutWindow.h"
#include "BeLauncherBase.h"           // For GAP-functions
#include "BeUrlStringView.h"

#include <Rect.h>
#include <GroupView.h>
#include <InterfaceDefs.h>
#include <Button.h>
#include <Catalog.h>
#include <Message.h>

#define G_STRIPE_W                    30.0f

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT         "BeAboutWindow"

#define L_ABOUT_BUTTON_OK             B_TRANSLATE("OK")

#define O_ABOUT_MAIN_VIEW             "aboutMainView"
#define O_ABOUT_STRIPE_VIEW           "aboutStripeView"
#define O_ABOUT_BUTTON_OK             "aboutButtonOk"
#define O_ABOUT_URL_STRING_VIEW       "aboutUrlStringView"

BeAboutWindow::BeAboutWindow(const BRect &frame, const char *title)
	: BeMainWindow(frame, title)
{
	SetFlags(Flags() | B_CLOSE_ON_ESCAPE);
	SetType(B_MODAL_WINDOW);
	CreateForm(); // TODO: See this
}

void
BeAboutWindow::CreateForm()
{
	BRect r(Bounds());
	BRect stripeRect(r.left, r.top, G_STRIPE_W, r.bottom);

	BView *mainView = new BView(r, O_ABOUT_MAIN_VIEW, B_FOLLOW_ALL, B_WILL_DRAW);
	mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BView *stripeView = new BView(stripeRect, O_ABOUT_STRIPE_VIEW, B_FOLLOW_ALL, B_WILL_DRAW);
	stripeView->SetViewColor(tint_color(mainView->ViewColor(), B_DARKEN_1_TINT));

	BButton *okButton = new BButton(BRect(), O_ABOUT_BUTTON_OK, L_ABOUT_BUTTON_OK,
	                                new BMessage(B_CLOSE_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	okButton->ResizeToPreferred();
	okButton->MoveTo(r.right - okButton->Bounds().Width() - BeLauncherBase::Gap(),
	                 r.bottom - okButton->Bounds().Height() - BeLauncherBase::Gap());
	SetDefaultButton(okButton);
	mainView->AddChild(okButton);

	BeUrlStringView *beUrlStringView = new BeUrlStringView(BRect(), O_ABOUT_URL_STRING_VIEW, "www.exlmoto.ru");
	beUrlStringView->MoveTo(G_STRIPE_W * 3, r.bottom - okButton->Bounds().Height() - BeLauncherBase::Gap() * 4);
	mainView->AddChild(beUrlStringView);

	AddChild(mainView);
	AddChild(stripeView);
}

bool
BeAboutWindow::QuitRequested()
{
	return true;
}

float
BeAboutWindow::GetStripeWidth()
{
	return G_STRIPE_W;
}
