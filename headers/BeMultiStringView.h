#ifndef BEMULTISTRINGVIEW_H
#define BEMULTISTRINGVIEW_H

#include <TextView.h>
#include <View.h>

class BeMultiStringView : public BTextView
{
public:
	BeMultiStringView(const char* name, const BView *view);

	void FrameResized(float newWidth, float newHeight);
};

#endif // BEMULTISTRINGVIEW_H
