/************************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2019 3dEyes**
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

#include "BeApp.h"
#include "BeLauncherBase.h"
#include "BeAboutWindow.h"
#include "BeUrlStringView.h"
#include "BeDirectoryFilePanel.h"
#include "BeDirectoryFilter.h"
#include "BeUtils.h"

#include <Rect.h>
#include <String.h>
#include <View.h>
#include <StringList.h>
#include <StringView.h>
#include <CheckBox.h>
#include <TextControl.h>
#include <ListView.h>
#include <ScrollView.h>
#include <Message.h>
#include <Font.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <InterfaceDefs.h>
#include <Directory.h>
#include <Entry.h>
#include <Path.h>

#include <Catalog.h>

#include <posix/stdlib.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "Dhewm3GameLauncher"

// Launcher Settings
#define TITLE                          "Dhewm3 Launcher"
#define VERSION                        "1.0.0"
#define PACKAGE_DIR                    "dhewm3"
#define SETTINGS_FILE                  "Dhewm3Launcher.set"
#define EXECUTABLE_FILE                "dhewm3"
#define DATA_PATH_OPT                  "DHEWM3_BASEDIR"
#define GAME_OPT                       "DHEWM3_GAME"

#define BASE_DIR_NAME                  "base"
#define D3XP_DIR_NAME                  "d3xp"
#define CDOOM_DIR_NAME                 "cdoom"
#define RIVENSIN_DIR_NAME              "rivensin"
#define DENTONMOD_DIR_NAME             "dentonmod"
#define FITZ_DIR_NAME                  "fitz"
#define HARDQORE2_DIR_NAME             "hardqore2"

// Globals
#define G_GAMES_LIST_HEIGHT            128.0f
#define G_MAX_GAME_NAME_LENGTH         32

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("dhewm3 is a source port of the original DOOM 3. " \
                                       "Compared to the original version of DOOM 3, dhewm3 has many bugfixes, supports EAX-like " \
                                       "sound effects, has much better support for widescreen resolutions and has 64bit support. \n" \
                                       "It only supports old Mods if they either don't require their own game shared library or have " \
                                       "been ported to dhewm3 - see the Mods page for more information. \n" \
                                       "Note that while the DOOM 3 source code has been released under GPL, you still need " \
                                       "to legally own the game and provide dhewm3 the game data to play.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("EXL\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("3dEyes**\n")
#define L_ABOUT_LINK                   B_TRANSLATE("https://dhewm3.org")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official Site:")
#define L_ABOUT_LINK_SRC               B_TRANSLATE("https://github.com/dhewm/dhewm3")
#define L_ABOUT_LINK_SRC_DESC          B_TRANSLATE("Source Code:")
#define L_ABOUT_LINK_MOD               B_TRANSLATE("https://dhewm3.org/mods.html")
#define L_ABOUT_LINK_MOD_DESC          B_TRANSLATE("Supported Mods:")
#define L_DATA_TEXT_DOOM3              B_TRANSLATE("DOOM 3")
#define L_DATA_LINK_DOOM3              B_TRANSLATE("http://store.steampowered.com/app/9050")
#define L_DATA_TEXT_RESSURRECTION_EVIL B_TRANSLATE("DOOM 3 Resurrection of Evil")
#define L_DATA_LINK_RESSURRECTION_EVIL B_TRANSLATE("http://store.steampowered.com/app/9070")
#define L_DATA_FILES_LINKS_D           B_TRANSLATE("Buy game files on Steam: ")
#define L_DATA_DOWNLOAD_MOD_DESC       B_TRANSLATE("Download ")
#define L_DATA_LINK_MOD_DESC           B_TRANSLATE("Supported Mods")
#define L_SEPARATOR                    B_TRANSLATE(" | ")
#define L_DOT                          B_TRANSLATE(".")
#define L_ERROR_NO_BASE_CATALOG        B_TRANSLATE("Game files directory does not contain the \"base\" catalog.")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("Required data file *.pk4 not found.")
#define L_GAME_LIST_LABEL              B_TRANSLATE("Please select a Game/Mod:")
#define L_CMD_LIST_ARGS                B_TRANSLATE("Arguments:")
#define L_BASE_GAME_NAME               B_TRANSLATE("DOOM 3")
#define L_D3XP_GAME_NAME               B_TRANSLATE("DOOM 3 Resurrection of Evil")
#define L_CDOOM_GAME_NAME              B_TRANSLATE("Classic DOOM 3")
#define L_RIVENSIN_GAME_NAME           B_TRANSLATE("Scarlet Rivensin: The Ruiner")
#define L_DENTONMOD_GAME_NAME          B_TRANSLATE("Denton's Enhanced DOOM 3")
#define L_FITZ_GAME_NAME               B_TRANSLATE("Fitz Packerton")
#define L_HARDQORE2_GAME_NAME          B_TRANSLATE("HardQore2")

// Object Names
#define O_CHECKBOX_OPTION              "checkBoxOption"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK_DOOM3              "dataLinkD3"
#define O_DATA_LINK_RESSURRECTION_EVIL "dataLinkRE"
#define O_DATA_LINKS_DESC              "dataLinkDesc"
#define O_DATA_LINK_MOD                "dataLinkMod"
#define O_DATA_DOWNLOAD_MOD            "dataLinkDownload"
#define O_VIEW_SCROLL                  "viewScroll"
#define O_GAME_LIST_LABEL              "gameListLabel"
#define O_CMD_LIST_ARGS                "cmdListArgsLabel"
#define O_SEPARATOR                    "separatorView"
#define O_DOT                          "dotView"

class Dhewm3AboutWindow : public BeAboutWindow
{
public:
	explicit
	Dhewm3AboutWindow(const BRect &frame, const char *title, const char *version)
		: BeAboutWindow(frame, title, version)
	{
		BStringView *urlDescString = new BStringView(O_ABOUT_LINK_DESC, L_ABOUT_LINK_DESC);
		BeUrlStringView *urlString = new BeUrlStringView(O_ABOUT_LINK, L_ABOUT_LINK);
		BStringView *urlDescSrcString = new BStringView(O_ABOUT_LINK_DESC, L_ABOUT_LINK_SRC_DESC);
		BeUrlStringView *urlSrcString = new BeUrlStringView(O_ABOUT_LINK, L_ABOUT_LINK_SRC);
		BStringView *urlDescModString = new BStringView(O_ABOUT_LINK_DESC, L_ABOUT_LINK_MOD_DESC);
		BeUrlStringView *urlModString = new BeUrlStringView(O_ABOUT_LINK, L_ABOUT_LINK_MOD);

		BeAboutWindow::GetInformationView()->Insert(L_ABOUT_STRING);
		BeAboutWindow::GetInformationView()->SetFontAndColor(be_bold_font);
		BeAboutWindow::GetInformationView()->Insert(L_ABOUT_THANKS_STR_H);
		BeAboutWindow::GetInformationView()->SetFontAndColor(be_plain_font);
		BeAboutWindow::GetInformationView()->Insert(L_ABOUT_THANKS_STR);
		BeAboutWindow::GetInformationView()->SetFontAndColor(be_bold_font);
		BeAboutWindow::GetInformationView()->Insert(L_ABOUT_PORT_STR_H);
		BeAboutWindow::GetInformationView()->SetFontAndColor(be_plain_font);
		BeAboutWindow::GetInformationView()->Insert(L_ABOUT_PORT_STR);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, 0.0f)
		                          .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                              .Add(urlDescString)
		                              .Add(urlString)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                              .Add(urlDescModString)
		                              .Add(urlModString)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                              .Add(urlDescSrcString)
		                              .Add(urlSrcString)
		                              .AddGlue()
		                          .End();
		BeAboutWindow::GetAdditionalBox()->AddChild(boxLayout->View());
	}
};

class Dhewm3GameLauncher : public BeLauncherBase
{
	enum
	{
		MSG_SELECTED_SOME_GAME = 'ssgl'
	};

	BString fSelectedGame;
	BStringList fGameListDescription;
	BStringList fGameListModNames;
	BListView *fGamesList;
	BScrollView *fScrollView;

	void
	SelectSomeGame(void)
	{
		bool found = false;
		for(int i = 0; i < fGameListModNames.CountStrings(); ++i)
		{
			BString modName = fGameListModNames.StringAt(i);
			if(modName.Compare(BeLauncherBase::GetSettings()->GetSettingsString(GAME_OPT)) == 0)
			{
				found = true;
				fGamesList->Select(i);
			}
		}
		if(!found)
		{
			fGamesList->Select(0);
		}
	}

	void
	FillGamesList(void)
	{
		fGamesList->MakeEmpty();
		fGameListModNames.MakeEmpty();
		fGameListDescription.MakeEmpty();

		entry_ref ref;
		BEntry entry(BeLauncherBase::GetTextControl()->Text());
		if(!entry.Exists() || !entry.IsDirectory())
		{
			fGamesList->AddItem(new BStringItem(L_ERROR_NO_BASE_CATALOG));
			return;
		}
		entry.GetRef(&ref);

		BDirectory directory(&ref);
		if(directory.InitCheck() != B_OK)
		{
			fGamesList->AddItem(new BStringItem(L_ERROR_NO_BASE_CATALOG));
			return;
		}
		directory.Rewind();
		while(directory.GetNextEntry(&entry, true) == B_OK)
		{
			if(entry.IsDirectory())
			{
				entry.GetRef(&ref);
				BString modName = BString(ref.name);
				BString modNameDescription;
				BString gameListItem;
				if (modName == BASE_DIR_NAME)
					modNameDescription = L_BASE_GAME_NAME;
				else if (modName == D3XP_DIR_NAME)
					modNameDescription = L_D3XP_GAME_NAME;
				else if (modName == CDOOM_DIR_NAME)
					modNameDescription = L_CDOOM_GAME_NAME;
				else if (modName == RIVENSIN_DIR_NAME)
					modNameDescription = L_RIVENSIN_GAME_NAME;
				else if (modName == DENTONMOD_DIR_NAME)
					modNameDescription = L_DENTONMOD_GAME_NAME;
				else if (modName == FITZ_DIR_NAME)
					modNameDescription = L_FITZ_GAME_NAME;
				else if (modName == HARDQORE2_DIR_NAME)
					modNameDescription = L_HARDQORE2_GAME_NAME;
				if (modNameDescription.Length() != 0) {
					BPath gameDirPath;
					entry.GetPath(&gameDirPath);
					BDirectory gameDir(gameDirPath.Path());
					BPath filePath;
					BString filename;
					bool pk4FileExist = false;

					while(gameDir.GetNextEntry(&entry, true) == B_OK) {
						entry.GetPath(&filePath);
						filename = filePath.Leaf();
						if(filename.IFindLast("pk4", filename.Length()) != B_ERROR) {
							pk4FileExist = true;
							break;
						}
					}
					if(pk4FileExist) {
						fGameListModNames.Add(modName);
						fGameListDescription.Add(modNameDescription);
						gameListItem << modNameDescription << " (" << modName << ")";
						fGamesList->AddItem(new BStringItem(gameListItem));
					}
				}
			}
		}

		if(fGameListModNames.CountStrings() == 0 || !fGameListModNames.HasString(BASE_DIR_NAME))
		{
			fGamesList->MakeEmpty();
			fGameListModNames.MakeEmpty();
			fGameListDescription.MakeEmpty();
			fGamesList->AddItem(new BStringItem(L_ERROR_NO_BASE_CATALOG));
		}
	}

protected:
	virtual void
	MessageReceived(BMessage *msg)
	{
		switch (msg->what)
		{
			case MSG_FILE_PANEL_FILE_SELECTED:
			{
				BeLauncherBase::MessageReceived(msg);
				FillGamesList();
				fSelectedGame = BASE_DIR_NAME;
				BeLauncherBase::GetSettings()->SetSettingsString(GAME_OPT, fSelectedGame);
				SelectSomeGame();
				break;
			}
			case MSG_SELECTED_SOME_GAME:
			{
				int32 selection = fGamesList->CurrentSelection();
				if(selection < 0) {
					fSelectedGame = BASE_DIR_NAME;
				} else {
					fSelectedGame = fGameListModNames.StringAt(selection);
				}
				break;
			}
			default:
			{
				BeLauncherBase::MessageReceived(msg);
				break;
			}
		}
	}

	virtual bool
	CheckCache(void)
	{
		if (fGameListModNames.CountStrings() == 0) {
			BString errorMessage(L_ERROR_NO_GAMEINFO_FILE);
			SetStatusString(B_COLOR_RED, errorMessage);
			return false;
		}
		return true;
	}

	virtual bool
	RunGame(void)
	{
	    if (fSelectedGame.Length() == 0) {
			fSelectedGame = BASE_DIR_NAME;
		}

		BString args = "";
		if (fSelectedGame != BASE_DIR_NAME) {
			args << "+set fs_game " << fSelectedGame;
		}
		
		BeLauncherBase::SetCustomArgs(args.String());

		setenv("XDG_CONFIG_HOME", "/boot/home/config/settings", 1);
		setenv("XDG_DATA_HOME", "/boot/home/config/settings", 1);
		setenv("BASE_GAMEDIR", BeLauncherBase::GetTextControl()->Text(), 1);

		return BeLauncherBase::RunGameViaRoster(true, true);
	}

	virtual bool
	ReadSettings(void)
	{
		BeLauncherBase::ReadSettings();
		return true;
	}

	virtual void
	SaveSettings(bool def)
	{
		BeLauncherBase::GetSettings()->SetSettingsString(GAME_OPT, fSelectedGame);
		BeLauncherBase::SaveSettings(def);
	}

	virtual void
	ShowAboutDialog(void)
	{
		Dhewm3AboutWindow *dhewm3AboutWindow = new Dhewm3AboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 4)),
		                                                             TITLE, VERSION);
		dhewm3AboutWindow->Show();
	}

	virtual void
	FrameResized(float newWidth, float newHeight)
	{
		fGamesList->Invalidate();
		fScrollView->Invalidate();
		BeLauncherBase::FrameResized(newWidth, newHeight);
	}

public:
	explicit
	Dhewm3GameLauncher(const char *startPath)
		: BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH_OPT,
		                 startPath, true, false)
	{		
		BStringView *urlDescString = new BStringView(O_DATA_LINKS_DESC, L_DATA_FILES_LINKS_D);
		BStringView *separatorOne = new BStringView(O_SEPARATOR, L_SEPARATOR);
		BStringView *separatorTwo = new BStringView(O_SEPARATOR, L_SEPARATOR);
		BeUrlStringView *urlStringD3 = new BeUrlStringView(O_DATA_LINK_DOOM3,
		                                                   L_DATA_TEXT_DOOM3, L_DATA_LINK_DOOM3);
		BeUrlStringView *urlStringRE = new BeUrlStringView(O_DATA_LINK_RESSURRECTION_EVIL,
		                                                   L_DATA_TEXT_RESSURRECTION_EVIL, L_DATA_LINK_RESSURRECTION_EVIL);
		BStringView *dot = new BStringView(O_DOT, L_DOT);

		BStringView *urlDescModString = new BStringView(O_DATA_DOWNLOAD_MOD, L_DATA_DOWNLOAD_MOD_DESC);
		BeUrlStringView *urlModString = new BeUrlStringView(O_DATA_LINK_MOD, L_DATA_LINK_MOD_DESC, L_ABOUT_LINK_MOD);

		BStringView *gameListLabel = new BStringView(O_GAME_LIST_LABEL, L_GAME_LIST_LABEL);
		fGamesList = new BListView();
		fGamesList->SetExplicitMinSize(BSize(B_SIZE_UNSET, G_GAMES_LIST_HEIGHT));
		fGamesList->SetSelectionMessage(new BMessage(MSG_SELECTED_SOME_GAME));
		fScrollView = new BScrollView(O_VIEW_SCROLL, fGamesList, B_WILL_DRAW | B_FRAME_EVENTS,
		                                          false, true, B_PLAIN_BORDER);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(gameListLabel)
		                              .AddGlue()
		                          .End()
		                          .Add(fScrollView)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlDescString)
		                              .Add(urlStringD3)
		                              .Add(separatorOne)
		                              .Add(urlStringRE)
		                              .Add(dot)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlDescModString)
		                              .Add(urlModString)
		                              .AddGlue()
		                          .End();
		BeLauncherBase::GetAdditionalBox()->AddChild(boxLayout->View());

		ReadSettings();
		FillGamesList();
		SelectSomeGame();
	}

	virtual
	~Dhewm3GameLauncher()
	{
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	Dhewm3GameLauncher *dhewm3GameLauncher = new Dhewm3GameLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(dhewm3GameLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
