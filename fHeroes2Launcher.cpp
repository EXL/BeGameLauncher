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
#include <String.h>
#include <StringView.h>
#include <Entry.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <InterfaceDefs.h>

#include <Catalog.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "fHeroes2Launcher"

// Launcher Settings
#define TITLE                          "fHeroes2 Launcher"
#define VERSION                        "1.0.0"
#define PACKAGE_DIR                    "fHeroes2"
#define SETTINGS_FILE                  "fHeroes2Launcher.set"
#define EXECUTABLE_FILE                "fHeroes2"
#define DATA_PATH_OPT                  "FHEROES2_DATA"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("Free implementation of Heroes of the Might and Magic II engine.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("EXL\n\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("3dEyes**\n")
#define L_ABOUT_LINK                   B_TRANSLATE("https://ihhub.github.io/fheroes2/")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official site:")
#define L_DATA_LINK_U                  B_TRANSLATE("https://store.ubi.com/ru/game?pid=5902fbba88a7e3f0618b4567")
#define L_DATA_TEXT_U                  B_TRANSLATE("Ubisoft")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy game files on ")
#define L_DATA_LINK_G                  B_TRANSLATE("https://www.gog.com/game/heroes_of_might_and_magic_2_gold_edition")
#define L_DATA_TEXT_G                  B_TRANSLATE("GOG")
#define L_OR                           B_TRANSLATE("or ")
#define L_DOT                          B_TRANSLATE(".")
#define L_COMMA                        B_TRANSLATE(", ")
#define L_DATA_DEMO_TEXT_D             B_TRANSLATE("archive.org")
#define L_DATA_DEMO_LINK_D             B_TRANSLATE("https://archive.org/download/HeroesofMightandMagicIITheSuccessionWars_1020/h2demo.zip")
#define L_DATA_DEMO_TEXT_DESC          B_TRANSLATE("download free demo version from ")

// Object Names
#define O_OR                           "orView"
#define O_DOT                          "dotView"
#define O_COMMA                        "commaView"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK_U                  "dataLink"
#define O_DATA_LINK_DESC_U             "dataLinkDesc"
#define O_DATA_LINK_G                  "dataLinkG"
#define O_DATA_LINK_DESC_G             "dataLinkDescG"
#define O_DATA_DEMO_LINK               "dataDemoLink"
#define O_DATA_DEMO_LINK_DESC          "dataDemoLinkDesc"

class fHeroes2AboutWindow : public BeAboutWindow
{
public:
	explicit fHeroes2AboutWindow(const BRect &frame, const char *title, const char *version)
	         : BeAboutWindow(frame, title, version)
	{
		BStringView *urlDescString = new BStringView(O_ABOUT_LINK_DESC, L_ABOUT_LINK_DESC);
		BeUrlStringView *urlString = new BeUrlStringView(O_ABOUT_LINK, L_ABOUT_LINK);

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
		                          .End();
		BeAboutWindow::GetAdditionalBox()->AddChild(boxLayout->View());
	}
};

class fHeroes2Launcher : public BeLauncherBase
{
protected:
	virtual bool
	RunGame(void)
	{
		return BeLauncherBase::RunGameViaRoster(true, true);
	}

	virtual void
	ShowAboutDialog(void)
	{
		fHeroes2AboutWindow *gameAboutWindow = new fHeroes2AboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                       TITLE, VERSION);
		gameAboutWindow->Show();
	}

public:
	explicit fHeroes2Launcher(const char *startPath)
	         : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH_OPT,
	                          startPath, true, true)
	{
		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC_U, L_DATA_FILES_LINK_D);
		BeUrlStringView *urlStringUBI = new BeUrlStringView(O_DATA_LINK_U, L_DATA_TEXT_U, L_DATA_LINK_U);
		BeUrlStringView *urlStringGOG = new BeUrlStringView(O_DATA_LINK_G, L_DATA_TEXT_G, L_DATA_LINK_G);
		BStringView *dotText = new BStringView(O_DOT, L_DOT);
		BStringView *commaText = new BStringView(O_COMMA, L_COMMA);
		BStringView *orText = new BStringView(O_OR, L_OR);
		
		BStringView *demoDataDescString = new BStringView(O_DATA_DEMO_LINK_DESC, L_DATA_DEMO_TEXT_DESC);
		BeUrlStringView *demoDataString = new BeUrlStringView(O_DATA_DEMO_LINK, L_DATA_DEMO_TEXT_D, L_DATA_DEMO_LINK_D);
		
		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlDescString)
		                              .Add(urlStringUBI)
		                              .Add(commaText)
		                              .Add(urlStringGOG)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(orText)
		                              .Add(demoDataDescString)
		                              .Add(demoDataString)
		                              .Add(dotText)
		                              .AddGlue()
		                          .End();
		BeLauncherBase::GetAdditionalBox()->AddChild(boxLayout->View());
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	fHeroes2Launcher *fheroes2launcher = new fHeroes2Launcher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(fheroes2launcher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
