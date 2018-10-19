#include "BeLauncherView.h"
#include "BeImageView.h"
#include "BeUtils.h"

#include <View.h>
#include <Size.h>
#include <StringView.h>
#include <InterfaceDefs.h>
#include <LayoutBuilder.h>

#include <Catalog.h>

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "BeGameLauncher"

#define L_DATA_STRING_VIEW             B_TRANSLATE("Please select a directory with game files: ")
#define L_DATA_TEXT_CONTROL_TOOLTIP    B_TRANSLATE("Path to a directory with game files.")
#define L_BUTTON_BROWSE_TOOLTIP        B_TRANSLATE("Click to open the file dialog.")
#define L_BUTTON_BROWSE                B_TRANSLATE("...")
#define L_STATUS_READY                 B_TRANSLATE("Ready.")
#define L_BUTTON_RUN                   B_TRANSLATE("Run!")
#define L_BUTTON_RUN_TOOLTIP           B_TRANSLATE("Click to run a game.")
#define L_BUTTON_EXIT                  B_TRANSLATE("Exit")
#define L_BUTTON_EXIT_TOOLTIP          B_TRANSLATE("Click to exit the application.")
#define L_BUTTON_ABOUT                 B_TRANSLATE("About...")
#define L_BUTTON_ABOUT_TOOLTIP         B_TRANSLATE("Click to view about information.")

#define O_LAUNCHER_VIEW                "launcherView"
#define O_BANNER_VIEW                  "bannerView"
#define O_ICON_VIEW                    "iconView"
#define O_DATA_STRING_VIEW             "dataStringView"
#define O_ADDITIONAL_BOX               "additionalBox"
#define O_STATUS_STRING_VIEW           "statusStringView"

BeLauncherView::BeLauncherView(bool showIcon)
    : BGroupView(O_LAUNCHER_VIEW, B_VERTICAL)
{
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	BeImageView *bannerView = new BeImageView(O_BANNER_VIEW, K_BANNER);
	BView *iconView;
	if(showIcon)
	{
		iconView = new BeImageView(O_ICON_VIEW, K_ICON);
	}
	else
	{
		iconView = new BView(O_ICON_VIEW, B_WILL_DRAW);
		iconView->SetExplicitSize(BSize(BeImageView::GetGeneralWidth(), BeImageView::GetGeneralWidth()));
	}

	BStringView *dataStringView = new BStringView(O_DATA_STRING_VIEW, L_DATA_STRING_VIEW);
	fTextControl = new BTextControl(NULL, NULL, NULL);
	fTextControl->SetToolTip(L_DATA_TEXT_CONTROL_TOOLTIP);

	fBrowseButton = new BButton(L_BUTTON_BROWSE, NULL);
	fBrowseButton->SetToolTip(L_BUTTON_BROWSE_TOOLTIP);
	fBrowseButton->ResizeToPreferred();
	fBrowseButton->SetExplicitSize(BSize(fTextControl->Bounds().Height(), fTextControl->Bounds().Height()));

	fAdditionalBox = new BBox(O_ADDITIONAL_BOX);
	fAdditionalBox->SetBorder(B_NO_BORDER);

	fStatusStringView = new BStringView(O_STATUS_STRING_VIEW, L_STATUS_READY);
	fStatusStringView->SetFontSize(G_SMALL_FONT_SIZE);

	fAboutButton = new BButton(L_BUTTON_ABOUT, NULL);
	fAboutButton->SetToolTip(L_BUTTON_ABOUT_TOOLTIP);
	fRunButton = new BButton(L_BUTTON_RUN, NULL);
	fRunButton->SetToolTip(L_BUTTON_RUN_TOOLTIP);
	fExitButton = new BButton(L_BUTTON_EXIT, NULL);
	fExitButton->SetToolTip(L_BUTTON_EXIT_TOOLTIP);

	GroupLayout()->SetSpacing(0.0f);
	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0.0f)
	    .Add(bannerView)
	    .AddGroup(B_VERTICAL, 0.0f)
	        .AddGroup(B_HORIZONTAL, 0.0f)
	            .AddGlue()
	            .Add(iconView)
	        .End()
	        .AddGroup(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
	            .SetInsets(B_USE_DEFAULT_SPACING, 0.0f, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
	            .AddGroup(B_HORIZONTAL, 0.0f)
	                .Add(dataStringView)
	                .AddGlue()
	            .End()
	            .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
	                .Add(fTextControl)
	                .Add(fBrowseButton)
	            .End()
	            .Add(fAdditionalBox)
	            .AddGlue()
	            .AddGroup(B_HORIZONTAL, 0.0f)
	                .Add(fStatusStringView)
	                .AddGlue()
	            .End()
	            .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
	                .Add(fAboutButton)
	                .AddGlue()
	                .Add(fRunButton)
	                .Add(fExitButton)
	            .End()
	        .End()
	    .End()
	    .View()->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
}

BTextControl *
BeLauncherView::GetTextControl(void) const
{
	return fTextControl;
}

BButton *
BeLauncherView::GetBrowseButton(void) const
{
	return fBrowseButton;
}

BBox *
BeLauncherView::GetAdditionalBox(void) const
{
	return fAdditionalBox;
}

BStringView *
BeLauncherView::GetStatusStringView(void) const
{
	return fStatusStringView;
}

BButton *
BeLauncherView::GetAboutButton(void) const
{
	return fAboutButton;
}

BButton *
BeLauncherView::GetRunButton(void) const
{
	return fRunButton;
}

BButton *
BeLauncherView::GetExitButton(void) const
{
	return fExitButton;
}

const char *
BeLauncherView::GetReadyString(void)
{
	return L_STATUS_READY;
}

BeLauncherView::~BeLauncherView()
{

}
