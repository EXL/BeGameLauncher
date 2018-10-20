#include "BeDirectoryFilter.h"
#include "BeUtils.h"

BeDirectoryFilter::BeDirectoryFilter(void)
{

}

bool
BeDirectoryFilter::Filter(const entry_ref *ref, BNode *node,
                          stat_beos *stat, const char *mimeType)
{
	BeUnused(node);
	BeUnused(mimeType);

	if(S_ISDIR(stat->st_mode))
	{
		return true;
	}

	if(S_ISLNK(stat->st_mode))
	{
		BEntry entry(ref, true);
		return entry.IsDirectory();
	}

	return false;
}

BeDirectoryFilter::~BeDirectoryFilter(void)
{

}
