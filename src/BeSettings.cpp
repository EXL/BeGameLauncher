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

#include "BeSettings.h"
#include "BeUtils.h"

#include <File.h>
#include <Errors.h>
#include <StorageDefs.h>

BeSettings::BeSettings(const char *fileName)
            : BMessage()
{
	pathToSettingsFile = BeUtils::GetPathToSettingsFile(fileName);
}

void
BeSettings::SetSettingsString(const char *name, const char *string)
{
	BString buffer;
	if(FindString(name, &buffer) != B_OK)
	{
		AddString(name, string);
	}
	else
	{
		ReplaceString(name, string);
	}
}

bool
BeSettings::DumpSettingsToFile(void)
{
	BFile out;
	if(out.SetTo(pathToSettingsFile.String(), B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE) != B_OK)
	{
		return false;
	}
	if(Flatten(&out) != B_OK)
	{
		return false;
	}
	out.Unset();

	return true;
}

bool
BeSettings::ReadSettingsFromFile(void)
{
	BFile in;
	if(in.SetTo(pathToSettingsFile.String(), B_READ_ONLY) != B_OK)
	{
		return false;
	}
	if(Unflatten(&in) != B_OK)
	{
		return false;
	}

	return true;
}

const char *
BeSettings::GetSettingsString(const char *name) const
{
	BString buffer;
	if (FindString(name, &buffer) != B_OK)
	{
		return "";
	}

	return buffer.String();
}

BeSettings::~BeSettings(void)
{

}
