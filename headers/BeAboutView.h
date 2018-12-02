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

#ifndef BEABOUTVIEW_H
#define BEABOUTVIEW_H

#include "BeAboutStripeView.h"
#include "BeMultiStringView.h"

#include <GroupView.h>
#include <StringView.h>
#include <Box.h>
#include <Button.h>

class BeAboutView : public BGroupView
{
	BStringView *fNameView;
	BStringView *fVersionView;

	BBox *fAdditionalBox;
	BButton *fAboutButton;

	BeMultiStringView *fInformationView;
	BeAboutStripeView *fAboutStripeView;

protected:
	virtual void AllAttached(void);

public:
	explicit BeAboutView(const char *name, const char *version);
	virtual ~BeAboutView(void);

	BBox *GetAdditionalBox(void) const;
	BButton *GetAboutButton(void) const;

	BeMultiStringView *GetInformationView(void) const;
	BeAboutStripeView *GetAboutStripeView(void) const;
};

#endif // BEABOUTVIEW_H
