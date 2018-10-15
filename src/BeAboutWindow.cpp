#include "BeAboutWindow.h"
#include "BeLauncherBase.h"           // For GAP-functions
#include "BeUrlStringView.h"
#include "BeImageView.h"
#include "BeUtils.h"

#include <Rect.h>
#include <Point.h>
#include <GroupView.h>
#include <InterfaceDefs.h>
#include <Button.h>
#include <Catalog.h>
#include <Message.h>
#include <Font.h>

#define G_STRIPE_W                    30.0f
#define G_SMALL_FONT_SIZE             10.0f
#define G_ICON_OFFSET_W               24.0f
#define G_URL_EXLMOTO_BLOG_TITLE      "http://exlmoto.ru/haiku-packages"
#define G_URL_EXLMOTO_BLOG_LINK       "http://exlmoto.ru/haiku-packages#7"
#define G_URL_GITHUB_TITLE_LINK       "http://github.com/EXL/BeGameLauncher"

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT         "BeAboutWindow"

#define L_ABOUT_BUTTON_OK             B_TRANSLATE("OK")
#define L_ABOUT_LAUNCHER_STRING       B_TRANSLATE("BeGameLauncer (c) EXL, 2018")
#define L_ABOUT_VERSION_STRING        B_TRANSLATE("Version: ")

#define O_ABOUT_MAIN_VIEW             "aboutMainView"
#define O_ABOUT_ICON_VIEW             "aboutIconView"
#define O_ABOUT_TITLE_VIEW            "aboutTitleView"
#define O_ABOUT_TEXT_VIEW             "aboutTextView"
#define O_ABOUT_VERSION_VIEW          "aboutVersionView"
#define O_ABOUT_STRIPE_VIEW           "aboutStripeView"
#define O_ABOUT_LINE_VIEW             "aboutLineView"
#define O_ABOUT_LAUNCHER_STRING       "aboutLauncherString"
#define O_ABOUT_BUTTON_OK             "aboutButtonOk"
#define O_ABOUT_URL_STRING_VIEW_EM    "aboutUrlStringViewEm"
#define O_ABOUT_URL_STRING_VIEW_GH    "aboutUrlStringViewGh"

BeAboutWindow::BeAboutWindow(const BRect &frame, const char *title, const char *version)
	: BeMainWindow(frame, title)
{
	fTitle = title;
	fVersion << L_ABOUT_VERSION_STRING;
	fVersion << version;

	SetFlags(Flags() | B_CLOSE_ON_ESCAPE);
	SetType(B_MODAL_WINDOW);
}

void
BeAboutWindow::CreateForm()
{
	BRect r(Bounds());
	BRect stripeRect(r.left, r.top, G_STRIPE_W, r.bottom);

	fMainView = new BView(r, O_ABOUT_MAIN_VIEW, B_FOLLOW_ALL, B_WILL_DRAW);
	fMainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BRect iconRect(r.left + G_ICON_OFFSET_W, r.top + 15.0f, 64.0f + G_ICON_OFFSET_W, 64.0f + 15.0f);
	BeImageView *iconView = new BeImageView(iconRect, O_ABOUT_ICON_VIEW, K_ICON, B_FOLLOW_LEFT_TOP, true);

	BStringView *titleView = new BStringView(BRect(), O_ABOUT_TITLE_VIEW, fTitle, B_FOLLOW_LEFT_TOP);
	BFont titleFont;
	titleView->GetFont(&titleFont);
	titleFont.SetFace(B_BOLD_FACE);
	titleFont.SetSize(titleFont.Size() * 2.0f);
	titleView->SetFont(&titleFont, B_FONT_FAMILY_AND_STYLE | B_FONT_SIZE | B_FONT_FLAGS);
	titleView->ResizeToPreferred();
	titleView->MoveTo(iconView->Frame().RightTop().x + BeLauncherBase::Gap(), iconView->Frame().RightTop().y);
	fMainView->AddChild(titleView);

	BStringView *versionView = new BStringView(BRect(), O_ABOUT_VERSION_VIEW, fVersion, B_FOLLOW_LEFT_TOP);
	versionView->ResizeToPreferred();
	versionView->MoveTo(iconView->Frame().RightTop().x + BeLauncherBase::Gap(),
	                    titleView->Frame().RightBottom().y + BeLauncherBase::Gap());
	fMainView->AddChild(versionView);

	BView *stripeView = new BView(stripeRect, O_ABOUT_STRIPE_VIEW, B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW);
	stripeView->SetViewColor(tint_color(fMainView->ViewColor(), B_DARKEN_1_TINT));

	BButton *okButton = new BButton(BRect(), O_ABOUT_BUTTON_OK, L_ABOUT_BUTTON_OK,
	                                new BMessage(B_CLOSE_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	okButton->ResizeToPreferred();
	okButton->MoveTo(r.right - okButton->Bounds().Width() - BeLauncherBase::Gap(),
	                 r.bottom - okButton->Bounds().Height() - BeLauncherBase::Gap());
	SetDefaultButton(okButton);
	fMainView->AddChild(okButton);

	const float offsetWidth = iconView->Frame().RightTop().x + BeLauncherBase::Gap();
	BeUrlStringView *beUrlGhStringView = new BeUrlStringView(BRect(), O_ABOUT_URL_STRING_VIEW_GH,
	                                                         G_URL_GITHUB_TITLE_LINK,
	                                                         G_URL_GITHUB_TITLE_LINK,
	                                                         G_SMALL_FONT_SIZE,
	                                                         B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	beUrlGhStringView->MoveTo(offsetWidth, r.bottom - BeLauncherBase::Gap() * 2);
	fMainView->AddChild(beUrlGhStringView);

	BeUrlStringView *beUrlEmStringView = new BeUrlStringView(BRect(), O_ABOUT_URL_STRING_VIEW_EM,
	                                                         G_URL_EXLMOTO_BLOG_TITLE,
	                                                         G_URL_EXLMOTO_BLOG_LINK,
	                                                         G_SMALL_FONT_SIZE,
	                                                         B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	beUrlEmStringView->MoveTo(offsetWidth,
	                          r.bottom - beUrlGhStringView->Bounds().Height() - BeLauncherBase::Gap() * 2);
	fMainView->AddChild(beUrlEmStringView);

	const float lineHeight = r.bottom - beUrlEmStringView->Bounds().Height() - BeLauncherBase::Gap() * 2;
	BRect lineRect(offsetWidth, lineHeight,
	               offsetWidth + beUrlGhStringView->Bounds().Width(), lineHeight);
	BView *lineView = new BView(lineRect, O_ABOUT_LINE_VIEW, B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	lineView->SetViewColor(tint_color(fMainView->ViewColor(), B_DARKEN_3_TINT));
	fMainView->AddChild(lineView);

	BStringView *aboutLauncherString = new BStringView(BRect(), O_ABOUT_LAUNCHER_STRING, L_ABOUT_LAUNCHER_STRING,
	                                                   B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	aboutLauncherString->SetFontSize(G_SMALL_FONT_SIZE);
	aboutLauncherString->ResizeToPreferred();
	aboutLauncherString->MoveTo(offsetWidth,
	                            lineHeight - aboutLauncherString->Bounds().Height() - BeLauncherBase::StatusGapHack());
	fMainView->AddChild(aboutLauncherString);

	BRect textRect(offsetWidth, iconView->Bounds().Height() + BeLauncherBase::Gap() * 2,
	               r.right - BeLauncherBase::Gap(), lineHeight - BeLauncherBase::Gap() * 2);
	fTextView = new BView(textRect, O_ABOUT_TEXT_VIEW, B_FOLLOW_ALL, B_WILL_DRAW);
	fMainView->AddChild(fTextView);

	AddChild(fMainView);
	AddChild(stripeView);
	AddChild(iconView);

	SetSizeLimits(Frame().InsetBySelf(G_STRIPE_W, 0.0f).Width(),
	              Frame().InsetBySelf(-G_STRIPE_W * 4, 0.0f).Width(),
	              Frame().InsetBySelf(0.0f, G_STRIPE_W).Height(),
	              Frame().InsetBySelf(0.0f, -G_STRIPE_W * 3).Height());

	SetAboutText();
}

void
BeAboutWindow::SetAboutText()
{
	fTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

BView *
BeAboutWindow::GetMainView() const
{
	return fMainView;
}

BView *
BeAboutWindow::GetTextView() const
{
	return fTextView;
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

float
BeAboutWindow::GetSmallFontSize()
{
	return G_SMALL_FONT_SIZE;
}

float
BeAboutWindow::GetIconOffsetX()
{
	return G_ICON_OFFSET_W;
}

float
BeAboutWindow::GetStripeOffsetX()
{
	return G_STRIPE_W - G_ICON_OFFSET_W;
}
