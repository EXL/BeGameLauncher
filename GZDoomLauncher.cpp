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
#include "BeUtils.h"

#include <Font.h>
#include <Rect.h>
#include <String.h>
#include <StringView.h>
#include <Entry.h>
#include <Directory.h>
#include <Path.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <InterfaceDefs.h>

#include <Catalog.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "GZDoomLauncher"

// Launcher Settings
#define TITLE                          "GZDoom Launcher"
#define VERSION                        "1.0.0"
#define PACKAGE_DIR                    "GZDoom"
#define SETTINGS_FILE                  "GZDoomLauncher.set"
#define EXECUTABLE_FILE                "GZDoom"
#define DATA_PATH_OPT                  "GZDOOM_DATA"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("Advanced Doom source port with OpenGL support\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("EXL\n\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("Diver\n\t3dEyes**\n")
#define L_ABOUT_LINK                   B_TRANSLATE("https://zdoom.org")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official site:")
#define L_ABOUT_OTH_LINK               B_TRANSLATE("https://doomwiki.org/wiki/ZDoom")
#define L_ABOUT_OTH_LINK_DESC          B_TRANSLATE("ZDoom on Doom Wiki:")

#define L_DATA_ULTIMATE_LINK           B_TRANSLATE("https://steamcommunity.com/app/2280")
#define L_DATA_ULTIMATE_LINK_G         B_TRANSLATE("https://www.gog.com/game/the_ultimate_doom")
#define L_DATA_ULTIMATE_TEXT           B_TRANSLATE("The Ultimate DOOM: ")

#define L_DATA_DOOM2_LINK              B_TRANSLATE("https://steamcommunity.com/app/2300")
#define L_DATA_DOOM2_LINK_G            B_TRANSLATE("https://www.gog.com/game/doom_ii_final_doom")
#define L_DATA_DOOM2_TEXT              B_TRANSLATE("DOOM II: Hell on Earth: ")

#define L_DATA_HEXEN_LINK              B_TRANSLATE("https://steamcommunity.com/app/2360")
#define L_DATA_HEXEN_TEXT              B_TRANSLATE("HeXen: Beyond Heretic: ")

#define L_DATA_HERETIC_LINK            B_TRANSLATE("https://steamcommunity.com/app/2390")
#define L_DATA_HERETIC_TEXT            B_TRANSLATE("Heretic: Shadow of the Serpent Riders: ")

#define L_DATA_SUPPORTED_LINK          B_TRANSLATE("https://zdoom.org/wiki/Category:Supported_games")
#define L_DATA_SUPPORTED_TEXT          B_TRANSLATE("Games that can be run with GZDoom:")
#define L_DATA_SUPPORTED_DESC          B_TRANSLATE("ZDoom Wiki")

#define L_DATA_FILES_LINK_DESC         B_TRANSLATE("\nYou can buy some of these games:")
#define L_DOT                          B_TRANSLATE(" • ")
#define L_GOG                          B_TRANSLATE("GOG")
#define L_STEAM                        B_TRANSLATE("Steam")
#define L_OR                           B_TRANSLATE(" or ")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("WAD file not found.")

// Object Names
#define O_OR                           "orView"
#define O_DOT                          "dotView"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK_DESC               "dataLinkDesc"
#define O_DATA_LINK_ULTIMATE           "dataUltimateLink"
#define O_DATA_LINK_ULTIMATE_G         "dataUltimateLinkG"
#define O_DATA_LINK_DOOM2              "dataDoom2LinkDesc"
#define O_DATA_LINK_DOOM2_G            "dataDoom2LinkDescG"
#define O_DATA_LINK_HEXEN              "dataHexenLinkDesc"
#define O_DATA_LINK_HERETIC            "dataHereticLinkDesc"
#define O_DATA_LINK_SUPPORTED          "dataSupportedLinkDesc"
#define O_DATA_SUPPORTED_DESC          "dataSupportedDesc"

class GZDoomAboutWindow : public BeAboutWindow
{
public:
	explicit GZDoomAboutWindow(const BRect &frame, const char *title, const char *version)
	         : BeAboutWindow(frame, title, version)
	{
		BStringView *urlDescString = new BStringView(O_ABOUT_LINK_DESC, L_ABOUT_LINK_DESC);
		BeUrlStringView *urlString = new BeUrlStringView(O_ABOUT_LINK, L_ABOUT_LINK);
		BStringView *urlDescOthString = new BStringView(O_ABOUT_LINK_DESC, L_ABOUT_OTH_LINK_DESC);
		BeUrlStringView *urlOthString = new BeUrlStringView(O_ABOUT_LINK, L_ABOUT_OTH_LINK);

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
		                              .Add(urlDescOthString)
		                              .Add(urlOthString)
		                              .AddGlue()
		                          .End();
		BeAboutWindow::GetAdditionalBox()->AddChild(boxLayout->View());
	}
};

class GZDoomLauncher : public BeLauncherBase
{
	virtual bool
	CheckCache(void)
	{
		BString path = BeLauncherBase::GetTextControl()->Text();
		BDirectory gameDir(path);
		BEntry entry;
		BPath filePath;
		BString filename;
		bool wadFileExist = false;

		while(gameDir.GetNextEntry(&entry, true) == B_OK) {
			entry.GetPath(&filePath);
			filename = filePath.Leaf();
			printf("%s\n", filename.String());
			if(filename.IFindLast("wad", filename.Length()) != B_ERROR) {
				wadFileExist = true;
				break;
			}
		}
		if(!wadFileExist)
		{
			BString errorMessage(L_ERROR_NO_GAMEINFO_FILE);
			SetStatusString(B_COLOR_RED, errorMessage);
			return false;
		}
		return true;
	}

	virtual bool
	RunGame(void)
	{
		setenv("DOOMWADDIR", BeLauncherBase::GetTextControl()->Text(), 1);
		return BeLauncherBase::RunGameViaExecVe(true, false);
	}

	virtual void
	ShowAboutDialog(void)
	{
		GZDoomAboutWindow *gameAboutWindow = new GZDoomAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                       TITLE, VERSION);
		gameAboutWindow->Show();
	}

public:
	explicit GZDoomLauncher(const char *startPath)
	         : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH_OPT,
	                          startPath, true, true)
	{
		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC, L_DATA_FILES_LINK_DESC);
		BStringView *urlGameListString = new BStringView(O_DATA_SUPPORTED_DESC, L_DATA_SUPPORTED_TEXT);
		
		BeUrlStringView *urlStringSupported = new BeUrlStringView(O_DATA_LINK_SUPPORTED, L_DATA_SUPPORTED_DESC, L_DATA_SUPPORTED_LINK);		

		BStringView *urlDescDoomUltimateString = new BStringView(O_DATA_LINK_DESC, L_DATA_ULTIMATE_TEXT);
		BStringView *urlDescDoom2String = new BStringView(O_DATA_LINK_DESC, L_DATA_DOOM2_TEXT);
		BStringView *urlDescHexenString = new BStringView(O_DATA_LINK_DESC, L_DATA_HEXEN_TEXT);
		BStringView *urlDescHereticString = new BStringView(O_DATA_LINK_DESC, L_DATA_HERETIC_TEXT);

		BeUrlStringView *urlStringDoomUltimate = new BeUrlStringView(O_DATA_LINK_ULTIMATE, L_STEAM, L_DATA_ULTIMATE_LINK);
		BeUrlStringView *urlStringDoomUltimateG = new BeUrlStringView(O_DATA_LINK_ULTIMATE_G, L_GOG, L_DATA_ULTIMATE_LINK_G);
		BeUrlStringView *urlStringDoom2 = new BeUrlStringView(O_DATA_LINK_DOOM2, L_STEAM, L_DATA_DOOM2_LINK);
		BeUrlStringView *urlStringDoom2G = new BeUrlStringView(O_DATA_LINK_DOOM2_G, L_GOG, L_DATA_DOOM2_LINK_G);
		BeUrlStringView *urlStringHexen = new BeUrlStringView(O_DATA_LINK_HEXEN, L_STEAM, L_DATA_HEXEN_LINK);
		BeUrlStringView *urlStringHeretic = new BeUrlStringView(O_DATA_LINK_HERETIC, L_STEAM, L_DATA_HERETIC_LINK);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
									.AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
										.Add(urlGameListString)
										.AddGlue()
									.End()
									.AddGroup(B_HORIZONTAL, 0.0f)
										.Add(new BStringView(O_DOT, L_DOT))
										.Add(urlStringSupported)
										.AddGlue()
									.End()
									.AddGroup(B_HORIZONTAL, 0.0f)
										.Add(urlDescString)
										.AddGlue()
									.End()
									.AddGroup(B_HORIZONTAL, 0.0f)
										.Add(new BStringView(O_DOT, L_DOT))
										.Add(urlDescDoomUltimateString)
										.Add(urlStringDoomUltimate)
										.Add(new BStringView(O_OR, L_OR))
										.Add(urlStringDoomUltimateG)
										.AddGlue()
									.End()
									.AddGroup(B_HORIZONTAL, 0.0f)
										.Add(new BStringView(O_DOT, L_DOT))
										.Add(urlDescDoom2String)
										.Add(urlStringDoom2)
										.Add(new BStringView(O_OR, L_OR))
										.Add(urlStringDoom2G)
										.AddGlue()
									.End()
									.AddGroup(B_HORIZONTAL, 0.0f)
										.Add(new BStringView(O_DOT, L_DOT))
										.Add(urlDescHereticString)
										.Add(urlStringHeretic)
										.AddGlue()
									.End()
									.AddGroup(B_HORIZONTAL, 0.0f)
										.Add(new BStringView(O_DOT, L_DOT))
										.Add(urlDescHexenString)
										.Add(urlStringHexen)
										.AddGlue()
									.End();
		BeLauncherBase::GetAdditionalBox()->AddChild(boxLayout->View());
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	GZDoomLauncher *gzdoomLauncher = new GZDoomLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(gzdoomLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
