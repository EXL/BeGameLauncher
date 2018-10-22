#include "BeAboutStripeView.h"

#include <Size.h>
#include <Point.h>
#include <InterfaceDefs.h>
#include <GraphicsDefs.h>

#define G_ICON_OFFSET                  24.0f
#define G_STRIPE_WIDTH                 30.0f
#define G_ICON_X                       15.0f
#define G_ICON_Y                       10.0f

#define O_ABOUT_STRIPE_VIEW            "aboutStripeView"

BeAboutStripeView::BeAboutStripeView(const BBitmap *icon)
                   : BView(O_ABOUT_STRIPE_VIEW, B_WILL_DRAW), fIcon(icon)
{
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	float width = 0.0f;
	if(fIcon != NULL)
	{
		width += fIcon->Bounds().Width() + G_ICON_OFFSET;
	}

	SetExplicitSize(BSize(width, B_SIZE_UNSET));
	SetExplicitPreferredSize(BSize(width, B_SIZE_UNLIMITED));
}

void
BeAboutStripeView::Draw(BRect rect)
{
	if(fIcon == NULL)
	{
		return;
	}

	SetHighColor(ViewColor());
	FillRect(rect);

	BRect stripeRect = Bounds();
	stripeRect.right = G_STRIPE_WIDTH;
	SetHighColor(tint_color(ViewColor(), B_DARKEN_1_TINT));
	FillRect(stripeRect);

	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	DrawBitmapAsync(fIcon, BPoint(G_ICON_X, G_ICON_Y));
}

float
BeAboutStripeView::GetIconX(void)
{
	return G_ICON_X;
}

float
BeAboutStripeView::GetIconY(void)
{
	return G_ICON_Y;
}

float
BeAboutStripeView::GetIconOffset(void)
{
	return G_ICON_OFFSET;
}

float
BeAboutStripeView::GetStripeWidth(void)
{
	return G_STRIPE_WIDTH;
}

BeAboutStripeView::~BeAboutStripeView(void)
{
	delete fIcon;
	fIcon = NULL;
}
