#include "BeAboutView.h"
#include "BeImageView.h"
#include "BeUrlStringView.h"
#include "BeUnderlineStringView.h"
#include "BeUtils.h"

#include <InterfaceDefs.h>
#include <GraphicsDefs.h>
#include <Font.h>
#include <Size.h>
#include <Message.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <ScrollView.h>

#include <Catalog.h>

#define G_URL_EXLMOTO_BLOG_TITLE      "http://exlmoto.ru/haiku-packages"
#define G_URL_EXLMOTO_BLOG_LINK       "http://exlmoto.ru/haiku-packages#7"
#define G_URL_GITHUB_TITLE_LINK       "http://github.com/EXL/BeGameLauncher"

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT         "BeGameLauncher"

#define L_ABOUT_BUTTON                B_TRANSLATE("OK")
#define L_ABOUT_LAUNCHER_STRING       B_TRANSLATE("BeGameLauncer (c) EXL, 2018           ")

#define O_ABOUT_VIEW                  "aboutView"
#define O_ABOUT_NAME                  "aboutName"
#define O_ABOUT_VERSION               "aboutVersion"
#define O_ABOUT_INFORMATION           "aboutInformation"
#define O_ABOUT_BUTTON                "aboutButton"
#define O_ABOUT_SCROLLER              "aboutScroller"
#define O_ABOUT_ADDITIONAL_BOX        "aboutAdditionalBox"
#define O_ABOUT_LINE_VIEW             "aboutLineView"
#define O_ABOUT_LAUNCHER_STRING       "aboutLauncherString"
#define O_ABOUT_URL_STRING_VIEW_EM    "aboutUrlStringViewEm"
#define O_ABOUT_URL_STRING_VIEW_GH    "aboutUrlStringViewGh"

BeAboutView::BeAboutView(const char *name, const char *version)
	: BGroupView(O_ABOUT_VIEW, B_VERTICAL)
{
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	fAboutStripeView = new BeAboutStripeView(BeImageView::GetIconBitmapByIndex(K_ICON));

	fNameView = new BStringView(O_ABOUT_NAME, name);
	BFont font;
	fNameView->GetFont(&font);
	font.SetFace(B_BOLD_FACE);
	font.SetSize(font.Size() * 2.0f);
	fNameView->SetFont(&font, B_FONT_FAMILY_AND_STYLE | B_FONT_SIZE | B_FONT_FLAGS);
	fNameView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));

	fVersionView = new BStringView(O_ABOUT_VERSION, version);
	fVersionView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));

	rgb_color textColor = HighColor();
	fInformationView = new BeMultiStringView(O_ABOUT_INFORMATION, &textColor);

	// BScrollView *informationScroller = new BScrollView(O_ABOUT_SCROLLER, fInformationView,
	//                                                    B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_NO_BORDER);

	fAdditionalBox = new BBox(O_ABOUT_ADDITIONAL_BOX);
	fAdditionalBox->SetBorder(B_NO_BORDER);

	BeUnderlineStringView *aboutLauncherString = new BeUnderlineStringView(O_ABOUT_LAUNCHER_STRING,
	                                                                       L_ABOUT_LAUNCHER_STRING);
	BeUrlStringView *beUrlEmStringView = new BeUrlStringView(O_ABOUT_URL_STRING_VIEW_EM,
	                                                         G_URL_EXLMOTO_BLOG_TITLE, G_URL_EXLMOTO_BLOG_LINK,
	                                                         G_SMALL_FONT_SIZE);
	BeUrlStringView *beUrlGhStringView = new BeUrlStringView(O_ABOUT_URL_STRING_VIEW_GH,
	                                                         G_URL_GITHUB_TITLE_LINK, G_URL_GITHUB_TITLE_LINK,
	                                                         G_SMALL_FONT_SIZE);

	fAboutButton = new BButton(O_ABOUT_BUTTON, L_ABOUT_BUTTON, new BMessage(B_QUIT_REQUESTED));

	GroupLayout()->SetSpacing(0.0f);
	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0.0f)
	        .Add(fAboutStripeView)
	        .AddGroup(B_VERTICAL)
	            .SetInsets(0.0f, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
	            .AddGroup(B_VERTICAL, 0.0f)
	                .Add(fNameView)
	                .Add(fVersionView)
	                .AddStrut(B_USE_SMALL_SPACING)
	                .Add(fInformationView)
	                .Add(fAdditionalBox)
	                .AddGlue()
	            .End()
	            .AddGroup(B_HORIZONTAL, 0.0f)
	                .AddGroup(B_VERTICAL, 0.0f)
	                    .Add(aboutLauncherString)
	                    .Add(beUrlEmStringView)
	                    .Add(beUrlGhStringView)
	                .End()
	                .AddGlue()
	                .AddGroup(B_VERTICAL, 0.0f)
	                    .AddGlue()
	                    .Add(fAboutButton)
	                .End()
	            .End()
	        .End()
	        .View()->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
}

void
BeAboutView::AllAttached()
{
	fNameView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	fVersionView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	fInformationView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
}

BBox *
BeAboutView::GetAdditionalBox(void) const
{
	return fAdditionalBox;
}

BButton *
BeAboutView::GetAboutButton(void) const
{
	return fAboutButton;
}

BeMultiStringView *
BeAboutView::GetInformationView(void) const
{
	return fInformationView;
}

BeAboutStripeView *
BeAboutView::GetAboutStripeView(void) const
{
	return fAboutStripeView;
}

BeAboutView::~BeAboutView()
{

}
