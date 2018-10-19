#ifndef BEDIRECTORYFILTER_H
#define BEDIRECTORYFILTER_H

#include <Entry.h>
#include <Node.h>

#include <FilePanel.h>

#include <compat/sys/stat.h>

class BeDirectoryFilter : public BRefFilter
{
public:
	BeDirectoryFilter();
	virtual ~BeDirectoryFilter();

	virtual bool Filter(const entry_ref *ref, BNode *node,
	                    stat_beos *stat, const char *mimeType);
};

#endif // BEDIRECTORYFILTER_H
