#ifndef BEDIRECTORYFILTER_H
#define BEDIRECTORYFILTER_H

#include <FilePanel.h>

class BeDirectoryFilter : public BRefFilter
{
public:
	BeDirectoryFilter();
	bool Filter(const entry_ref *ref, BNode *node, stat_beos *stat, const char *mimeType);
};

#endif // BEDIRECTORYFILTER_H
