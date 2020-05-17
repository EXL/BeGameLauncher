/************************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2020 3dEyes**
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
#include <ScrollView.h>
#include <StringView.h>
#include <StringList.h>
#include <ListView.h>
#include <Entry.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <InterfaceDefs.h>

#include <Catalog.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "JediAcademyLauncher"

// Launcher Settings
#define TITLE                          "StarWars: Jedi Academy Launcher"
#define VERSION                        "1.0.0"
#define PACKAGE_DIR                    "JediAcademy"
#define SETTINGS_FILE                  "JediAcademyLauncher.set"
#define EXECUTABLE_FILE_SP             "JediAcademy"
#define EXECUTABLE_FILE_MP             "JediAcademyMP"
#define EXECUTABLE_FILE_TM             "Terminal"
#define EXECUTABLE_FILE_DS             "openjkded"
#define DATA_PATH_OPT                  "JEDI_ACADEMY_DATA"

// Globals
#define G_GAMES_LIST_HEIGHT            64.0f
#define G_SERVER_INFO_HEIGHT           64.0f

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("Community-maintained Star Wars Jedi Knight: Jedi Academy, story version\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("EXL\n\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("3dEyes**\n")
#define L_ABOUT_LINK                   B_TRANSLATE("https://github.com/JACoders/OpenJK")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official site:")
#define L_ABOUT_OTH_LINK               B_TRANSLATE("http://jkhub.org/forum/51-discussion/")
#define L_ABOUT_OTH_LINK_DESC          B_TRANSLATE("OpenJK Discussion:")
#define L_DATA_LINK                    B_TRANSLATE("https://store.steampowered.com/app/6020")
#define L_DATA_TEXT                    B_TRANSLATE("Steam")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy game files on ")
#define L_DATA_LINK_G                  B_TRANSLATE("https://www.gog.com/game/star_wars_jedi_knight_jedi_academy")
#define L_DATA_TEXT_G                  B_TRANSLATE("GOG")
#define L_OR                           B_TRANSLATE(" or ")
#define L_DOT                          B_TRANSLATE(".")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("Required data file %file% not found.")
#define L_GAME_LIST_LABEL              B_TRANSLATE("Please select:")
#define L_GAME_LIST_SINGLEPLAYER_DESC  B_TRANSLATE("Star Wars: Jedi Knight: Jedi Academy (Singleplayer)")
#define L_GAME_LIST_MULTIPLAYER_DESC   B_TRANSLATE("Star Wars: Jedi Knight: Jedi Academy (Multiplayer)")
#define L_GAME_LIST_SERVER_DESC        B_TRANSLATE("Dedicated Server")
#define L_GAMESP_INFORMATION           B_TRANSLATE("In single-player mode, the player takes control of the \
character Jaden Korr, a new student at Luke Skywalker's Jedi Academy under the tutelage of Kyle Katarn.")
#define L_GAMEMP_INFORMATION           B_TRANSLATE("In multiplayer mode, one can play online or via a local \
area network (LAN) with other players, as well as computer-controlled bots.")
#define L_SERVER_INFORMATION           B_TRANSLATE("Jedi Academy dedicated server.")

// Object Names
#define O_OR                           "orView"
#define O_DOT                          "dotView"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK                    "dataLink"
#define O_DATA_LINK_DESC               "dataLinkDesc"
#define O_DATA_LINK_G                  "dataLinkG"
#define O_DATA_LINK_DESC_G             "dataLinkDescG"
#define O_GAME_LIST_LABEL              "gameListLabel"
#define O_VIEW_SCROLL                  "viewScroll"
#define O_SERVER_INFORMATION           "serverInformation"

class JediAcademyAboutWindow : public BeAboutWindow
{
public:
	explicit JediAcademyAboutWindow(const BRect &frame, const char *title, const char *version)
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

class JediAcademyLauncher : public BeLauncherBase
{
	enum
	{
		MSG_LIST_ITEM_SELECTED = 'ssgl'
	};

	BListView *fGamesList;
	BScrollView *fScrollView;
	BTextView *fInformationView;

	void
	FillGamesList(void)
	{
		fGamesList->MakeEmpty();

		fGamesList->AddItem(new BStringItem(L_GAME_LIST_SINGLEPLAYER_DESC));
		fGamesList->AddItem(new BStringItem(L_GAME_LIST_MULTIPLAYER_DESC));
		fGamesList->AddItem(new BStringItem(L_GAME_LIST_SERVER_DESC));

		fGamesList->Select(0);
	}

protected:
	virtual void
	MessageReceived(BMessage *msg)
	{
		switch (msg->what)
		{
			case MSG_LIST_ITEM_SELECTED:
			{
				int32 selection = fGamesList->CurrentSelection();
				switch (selection)
				{
					case 0:
						fInformationView->SetText(L_GAMESP_INFORMATION);
						break;
					case 1:
						fInformationView->SetText(L_GAMEMP_INFORMATION);
						break;
					case 2:
						fInformationView->SetText(L_SERVER_INFORMATION);
						break;
					default:
						fInformationView->SetText("");
						break;
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
		BString path = BeLauncherBase::GetTextControl()->Text();
		if(!path.EndsWith("/"))
		{
			path << "/";
		}

		// NOTE: Check some file in the game cache.
		path << "base/assets0.pk3";
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
		setenv("OPENJK_GAME_DATA", BeLauncherBase::GetTextControl()->Text(), 1);

		if (fGamesList->CurrentSelection() == 0) {
			BString executableFilePath = BeUtils::GetPathToExecutable(PACKAGE_DIR, EXECUTABLE_FILE_SP);
			BeLauncherBase::SetExecutablePath(executableFilePath);
			return BeLauncherBase::RunGameViaExecVe(true, false);
		}
		if (fGamesList->CurrentSelection() == 1) {
			BString executableFilePath = BeUtils::GetPathToExecutable(PACKAGE_DIR, EXECUTABLE_FILE_MP);
			BeLauncherBase::SetExecutablePath(executableFilePath);
			return BeLauncherBase::RunGameViaExecVe(true, false);
		}
		if (fGamesList->CurrentSelection() == 2) {
			BString executableFilePath = BeUtils::GetPathToExecutable("", EXECUTABLE_FILE_TM);
			chdir(BeLauncherBase::GetTextControl()->Text());
			executableFilePath << " " << EXECUTABLE_FILE_DS << " &";
			system(executableFilePath.String());
			return false;
		}
		return false;
	}

	virtual void
	ShowAboutDialog(void)
	{
		JediAcademyAboutWindow *gameAboutWindow = new JediAcademyAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                       TITLE, VERSION);
		gameAboutWindow->Show();
	}

public:
	explicit JediAcademyLauncher(const char *startPath)
	         : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE_SP, SETTINGS_FILE, DATA_PATH_OPT,
	                          startPath, true, true)
	{
		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC, L_DATA_FILES_LINK_D);
		BeUrlStringView *urlString = new BeUrlStringView(O_DATA_LINK, L_DATA_TEXT, L_DATA_LINK);
		BeUrlStringView *urlStringGOG = new BeUrlStringView(O_DATA_LINK_G, L_DATA_TEXT_G, L_DATA_LINK_G);
		BStringView *dotText = new BStringView(O_DOT, L_DOT);
		BStringView *orText = new BStringView(O_OR, L_OR);

		BStringView *gameListLabel = new BStringView(O_GAME_LIST_LABEL, L_GAME_LIST_LABEL);
		fGamesList = new BListView();
		fGamesList->SetExplicitMinSize(BSize(B_SIZE_UNSET, G_GAMES_LIST_HEIGHT));
		fGamesList->SetSelectionMessage(new BMessage(MSG_LIST_ITEM_SELECTED));
		fScrollView = new BScrollView(O_VIEW_SCROLL, fGamesList, B_WILL_DRAW | B_FRAME_EVENTS,
		                                          false, true, B_PLAIN_BORDER);
		fInformationView = new BTextView(O_SERVER_INFORMATION);
		fInformationView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
		fInformationView->SetExplicitMinSize(BSize(B_SIZE_UNSET, G_SERVER_INFO_HEIGHT));
		fInformationView->SetText(L_SERVER_INFORMATION);
		fInformationView->MakeEditable(false);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(gameListLabel)
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
		                          .End()
		                          .AddStrut(B_USE_SMALL_SPACING)
	                              .Add(fInformationView);
		BeLauncherBase::GetAdditionalBox()->AddChild(boxLayout->View());

		FillGamesList();
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	JediAcademyLauncher *jediAcademyLauncher = new JediAcademyLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(jediAcademyLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
