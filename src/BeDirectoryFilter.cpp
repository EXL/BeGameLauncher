#include "BeDirectoryFilter.h"

BeDirectoryFilter::BeDirectoryFilter()
{

}

bool
BeDirectoryFilter::Filter(const entry_ref *ref, BNode *node,
                          stat_beos *stat, const char *mimeType)
{
	_UNUSED(node);
	_UNUSED(mimeType);

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

BeDirectoryFilter::~BeDirectoryFilter()
{

}
