/************************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2021 3dEyes**
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
#include <Screen.h>
#include <String.h>
#include <StringView.h>
#include <ListView.h>
#include <ScrollView.h>
#include <Entry.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <InterfaceDefs.h>

#include <Catalog.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "RTCWSPLauncher"

// Launcher Settings
#define TITLE                          "Return to Castle Wolfenstein"
#define TITLE_SP                       " (Single Player)"
#define VERSION                        "1.1.0"
#define PACKAGE_DIR                    "RTCW-SP"
#define SETTINGS_FILE                  "RTCWSPLauncher.set"
#define EXECUTABLE_FILE                "RTCW"
#define DATA_PATH_OPT                  "RTCWSP_DATA"
#define SCREEN_MODE_OPT                "SCREEN_MODE"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("World War II rages and nations fall.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("EXL\n\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("3dEyes**\n")
#define L_ABOUT_LINK                   B_TRANSLATE("https://bethesda.net/ru/store/product/WO2CSTPCBG01")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official site:")
#define L_ABOUT_OTH_LINK               B_TRANSLATE("https://github.com/tobydox/rtcw")
#define L_ABOUT_OTH_LINK_DESC          B_TRANSLATE("Sources:")
#define L_DATA_LINK                    B_TRANSLATE("https://steamcommunity.com/app/9010")
#define L_DATA_TEXT                    B_TRANSLATE("Steam")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy game files on ")
#define L_DATA_LINK_G                  B_TRANSLATE("https://www.gog.com/game/return_to_castle_wolfenstein")
#define L_DATA_TEXT_G                  B_TRANSLATE("GOG")
#define L_OR                           B_TRANSLATE(" or ")
#define L_DOT                          B_TRANSLATE(".")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("Required data file %file% not found.")
#define L_MODE_LIST_LABEL              B_TRANSLATE("Video mode:")

// Object Names
#define O_OR                           "orView"
#define O_DOT                          "dotView"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK                    "dataLink"
#define O_DATA_LINK_DESC               "dataLinkDesc"
#define O_DATA_LINK_G                  "dataLinkG"
#define O_DATA_LINK_DESC_G             "dataLinkDescG"
#define O_VIEW_SCROLL                  "viewScroll"
#define O_MODE_LIST_LABEL              "modeListLabel"

// Globals
#define G_MODES_LIST_HEIGHT            128.0f

static int R_MODES_LIST[][2] = { {640, 480}, {800, 600}, {1024, 768}, {1280, 1024}, {0, 0}};

class RTCWSPAboutWindow : public BeAboutWindow
{
public:
	explicit RTCWSPAboutWindow(const BRect &frame, const char *title, const char *version)
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

class RTCWSPLauncher : public BeLauncherBase
{
	enum
	{
		MSG_SELECTED_R_MODE = 'rmod'
	};

	BListView *fModesList;
	BScrollView *fScrollView;

	void
	SelectSomeMode(void)
	{
		int selected = atoi(BeLauncherBase::GetSettings()->GetSettingsString(SCREEN_MODE_OPT));
		if (selected > 0 && selected < fModesList->CountItems())
			fModesList->Select(selected);
		else
			fModesList->Select(0);
	}
	
	void
	FillModesList(void)
	{
		fModesList->MakeEmpty();
		BScreen screen(B_MAIN_SCREEN_ID);

		int idx = 0;
		while (R_MODES_LIST[idx][0] != 0) {
			if (R_MODES_LIST[idx][0] <= screen.Frame().IntegerWidth() + 1 &&
				R_MODES_LIST[idx][1] <= screen.Frame().IntegerHeight() + 1) {
				BString modeName;
				modeName << R_MODES_LIST[idx][0] << "x"	<< R_MODES_LIST[idx][1] << " (windowed)";
				fModesList->AddItem(new BStringItem(modeName));
			}
			idx++;
		}
		BString fullscreenModeName;
		fullscreenModeName << (screen.Frame().IntegerWidth() + 1) << "x" << (screen.Frame().IntegerHeight() + 1);
		fullscreenModeName << " (fullscreen)";
		fModesList->AddItem(new BStringItem(fullscreenModeName));
	}

protected:
	virtual bool
	CheckCache(void)
	{
		BString path = BeLauncherBase::GetTextControl()->Text();
		if(!path.EndsWith("/"))
		{
			path << "/";
		}

		// NOTE: Check some file in the game cache.
		path << "main/sp_pak1.pk3";
		BEntry fileToCheck(path);
		if(!fileToCheck.Exists())
		{
			BString errorMessage(L_ERROR_NO_GAMEINFO_FILE);
			errorMessage.ReplaceAll("%file%", path);
			SetStatusString(B_COLOR_RED, errorMessage);
			return false;
		}
		return true;
	}

	virtual bool
	RunGame(void)
	{
		BScreen screen(B_MAIN_SCREEN_ID);
		int32 selection = fModesList->CurrentSelection();
		int width = R_MODES_LIST[selection][0];
		int height = R_MODES_LIST[selection][1];
		if (width == 0 || height == 0) {
			width = screen.Frame().IntegerWidth() + 2;
			height = screen.Frame().IntegerHeight() + 2;
		}

		BString args = BeUtils::GetPathToExecutable(PACKAGE_DIR, EXECUTABLE_FILE);
		args << " +set fs_cdpath \"" << BeLauncherBase::GetTextControl()->Text() << "\"";
		args << " +set r_mode -1";
		args << " +set r_fullscreen 0";
		args << " +set r_customwidth " << width;
		args << " +set r_customheight " << height;
		args << " +set r_centerWindow 1";
		args << " +set com_recommendedSet 1";
		args << " &";

		if (CheckCache()) {
			printf("%s\n", args.String());
			system(args.String());
			return true;
		}

		return false;
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
		BString selectedStr;

		int selected = fModesList->CurrentSelection();
		if (selected >= 0 && selected < fModesList->CountItems())
			selectedStr << selected;
		else
			selectedStr << 0;

		BeLauncherBase::GetSettings()->SetSettingsString(SCREEN_MODE_OPT, selectedStr);
		BeLauncherBase::SaveSettings(def);
	}

	virtual void
	ShowAboutDialog(void)
	{
		RTCWSPAboutWindow *gameAboutWindow = new RTCWSPAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                       TITLE, VERSION);
		gameAboutWindow->Show();
	}

public:
	explicit RTCWSPLauncher(const char *startPath)
	         : BeLauncherBase(TITLE TITLE_SP, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH_OPT,
	                          startPath, true, true)
	{
		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC, L_DATA_FILES_LINK_D);
		BeUrlStringView *urlString = new BeUrlStringView(O_DATA_LINK, L_DATA_TEXT, L_DATA_LINK);
		BeUrlStringView *urlStringGOG = new BeUrlStringView(O_DATA_LINK_G, L_DATA_TEXT_G, L_DATA_LINK_G);
		BStringView *dotText = new BStringView(O_DOT, L_DOT);
		BStringView *orText = new BStringView(O_OR, L_OR);
		BStringView *modeListLabel = new BStringView(O_MODE_LIST_LABEL, L_MODE_LIST_LABEL);

		fModesList = new BListView();
		fModesList->SetExplicitMinSize(BSize(B_SIZE_UNSET, G_MODES_LIST_HEIGHT));
		fModesList->SetSelectionMessage(new BMessage(MSG_SELECTED_R_MODE));
		fScrollView = new BScrollView(O_VIEW_SCROLL, fModesList, B_WILL_DRAW | B_FRAME_EVENTS,
		                                          false, true, B_PLAIN_BORDER);


		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(modeListLabel)
		                              .AddGlue()
		                          .End()
                                  .Add(fScrollView)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlDescString)
		                              .Add(urlString)
		                              .Add(orText)
		                              .Add(urlStringGOG)
		                              .Add(dotText)
		                              .AddGlue()
		                          .End();                                  
		BeLauncherBase::GetAdditionalBox()->AddChild(boxLayout->View());

		ReadSettings();
		FillModesList();
		SelectSomeMode();
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	RTCWSPLauncher *rtcwspLauncher = new RTCWSPLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(rtcwspLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
