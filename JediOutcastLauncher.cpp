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
#define B_TRANSLATION_CONTEXT          "JediOutcastLauncher"

// Launcher Settings
#define TITLE                          "StarWars: Jedi Outcast Launcher"
#define VERSION                        "1.0.0"
#define PACKAGE_DIR                    "JediOutcast"
#define SETTINGS_FILE                  "JediOutcastLauncher.set"
#define EXECUTABLE_FILE                "JediOutcast"
#define DATA_PATH_OPT                  "JEDI_OUTCAST_DATA"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("Community-maintained Star Wars Jedi Knight II: Jedi Outcast, story version\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("EXL\n\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("3dEyes**\n")
#define L_ABOUT_LINK                   B_TRANSLATE("https://github.com/JACoders/OpenJK")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official site:")
#define L_ABOUT_OTH_LINK               B_TRANSLATE("http://jkhub.org/forum/51-discussion/")
#define L_ABOUT_OTH_LINK_DESC          B_TRANSLATE("OpenJK Discussion:")
#define L_DATA_LINK                    B_TRANSLATE("https://store.steampowered.com/app/6030")
#define L_DATA_TEXT                    B_TRANSLATE("Steam")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy game files on ")
#define L_DATA_LINK_G                  B_TRANSLATE("https://www.gog.com/game/star_wars_jedi_knight_ii_jedi_outcast")
#define L_DATA_TEXT_G                  B_TRANSLATE("GOG")
#define L_OR                           B_TRANSLATE(" or ")
#define L_DOT                          B_TRANSLATE(".")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("Required data file %file% not found.")

// Object Names
#define O_OR                           "orView"
#define O_DOT                          "dotView"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK                    "dataLink"
#define O_DATA_LINK_DESC               "dataLinkDesc"
#define O_DATA_LINK_G                  "dataLinkG"
#define O_DATA_LINK_DESC_G             "dataLinkDescG"

class JediOutcastAboutWindow : public BeAboutWindow
{
public:
	explicit JediOutcastAboutWindow(const BRect &frame, const char *title, const char *version)
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

class JediOutcastLauncher : public BeLauncherBase
{
	virtual bool
	CheckCache(void)
	{
		BString path = BeLauncherBase::GetTextControl()->Text();
		if(!path.EndsWith("/"))
		{
			path << "/";
		}

		// NOTE: Check some file in the game cache.
		path << "base/game0.pk3";
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
		return BeLauncherBase::RunGameViaExecVe(true, false);
	}

	virtual void
	ShowAboutDialog(void)
	{
		JediOutcastAboutWindow *gameAboutWindow = new JediOutcastAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                       TITLE, VERSION);
		gameAboutWindow->Show();
	}

public:
	explicit JediOutcastLauncher(const char *startPath)
	         : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH_OPT,
	                          startPath, true, true)
	{
		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC, L_DATA_FILES_LINK_D);
		BeUrlStringView *urlString = new BeUrlStringView(O_DATA_LINK, L_DATA_TEXT, L_DATA_LINK);
		BeUrlStringView *urlStringGOG = new BeUrlStringView(O_DATA_LINK_G, L_DATA_TEXT_G, L_DATA_LINK_G);
		BStringView *dotText = new BStringView(O_DOT, L_DOT);
		BStringView *orText = new BStringView(O_OR, L_OR);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlDescString)
		                              .Add(urlString)
		                              .Add(orText)
		                              .Add(urlStringGOG)
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
	JediOutcastLauncher *jediOutcastLauncher = new JediOutcastLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(jediOutcastLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
