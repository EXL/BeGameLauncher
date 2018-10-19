#include "BeLauncherView.h"
#include "BeImageView.h"
#include "BeUtils.h"

#include <InterfaceDefs.h>
#include <LayoutBuilder.h>

#define O_LAUNCHER_VIEW            "launcherView"
#define O_BANNER_VIEW              "bannerView"
#define O_ICON_VIEW                "iconView"

BeLauncherView::BeLauncherView(const char *name)
    : BGroupView(O_LAUNCHER_VIEW, B_VERTICAL)
{
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	BeImageView *bannerView = new BeImageView(O_BANNER_VIEW, K_BANNER);
	BeImageView *iconView = new BeImageView(O_ICON_VIEW, K_ICON);

	GroupLayout()->SetSpacing(0.0f);
	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0.0f)
	        .Add(bannerView)
	        .AddGroup(B_VERTICAL, 0.0f)
	            // .SetInsets(0.0f, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
	            .AddGroup(B_HORIZONTAL, 0.0f)
	            .AddGlue()
	            .Add(iconView)
	            .End()
	        .End()
	        .View()->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
}

BeLauncherView::~BeLauncherView()
{

}
