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

#include "BeAboutStripeView.h"
#include "BeImageView.h"
#include "BeUtils.h"

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
	else
	{
		width += BeImageView::GetGeneralWidth() + G_ICON_OFFSET;
	}

	SetExplicitSize(BSize(width, B_SIZE_UNSET));
	SetExplicitPreferredSize(BSize(width, B_SIZE_UNLIMITED));
}

void
BeAboutStripeView::Draw(BRect rect)
{
	SetHighColor(ViewColor());
	FillRect(rect);

	BRect stripeRect = Bounds();
	stripeRect.right = G_STRIPE_WIDTH;
	SetHighColor(tint_color(ViewColor(), B_DARKEN_1_TINT));
	FillRect(stripeRect);

	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	if(fIcon != NULL)
	{
		DrawBitmapAsync(fIcon, BPoint(G_ICON_X, G_ICON_Y));
	}
	else
	{
		BRect iconRect(G_ICON_X, G_ICON_Y,
		               G_ICON_X + BeImageView::GetGeneralWidth(), G_ICON_Y + BeImageView::GetGeneralWidth());
		SetHighColor(K_RED);
		FillRect(iconRect);
	}
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
	if(fIcon != NULL)
	{
		delete fIcon;
		fIcon = NULL;
	}
}
