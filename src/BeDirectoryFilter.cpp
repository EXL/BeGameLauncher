#include "BeDirectoryFilter.h"

#include <Entry.h>
#include <Node.h>

#include <compat/sys/stat.h>

BeDirectoryFilter::BeDirectoryFilter()
{

}

bool
BeDirectoryFilter::Filter(const entry_ref *ref, BNode *node, stat_beos *stat, const char *mimeType)
{
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
