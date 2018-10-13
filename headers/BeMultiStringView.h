#ifndef BEMULTISTRINGVIEW_H
#define BEMULTISTRINGVIEW_H

#include <TextView.h>
#include <Rect.h>

class BeMultiStringView : public BTextView
{
public:
	BeMultiStringView(const char* name, BRect rect);

	void FrameResized(float newWidth, float newHeight);
};

#endif // BEMULTISTRINGVIEW_H
