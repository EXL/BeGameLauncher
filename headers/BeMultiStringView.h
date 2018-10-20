#ifndef BEMULTISTRINGVIEW_H
#define BEMULTISTRINGVIEW_H

#include <TextView.h>
#include <GraphicsDefs.h>

class BeMultiStringView : public BTextView
{
public:
	explicit BeMultiStringView(const char* name, const rgb_color *color);
	virtual ~BeMultiStringView(void);

	static float GetMinWidth(void);
	static float GetMinHeight(void);
	static float GetGeneralInset(void);
};

#endif // BEMULTISTRINGVIEW_H
