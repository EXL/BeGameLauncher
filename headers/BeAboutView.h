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
	virtual void AllAttached();

public:
	explicit BeAboutView(const char *name, const char *version);
	virtual ~BeAboutView();

	BBox *GetAdditionalBox(void) const;
	BButton *GetAboutButton(void) const;

	BeMultiStringView *GetInformationView(void) const;
	BeAboutStripeView *GetAboutStripeView(void) const;
};

#endif // BEABOUTVIEW_H
