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
#define B_TRANSLATION_CONTEXT          "GishLauncher"

// Launcher Settings
#define TITLE                          "Gish Launcher"
#define VERSION                        "1.2"
#define PACKAGE_DIR                    "Gish"
#define SETTINGS_FILE                  "GishLauncher.set"
#define EXECUTABLE_FILE                "Gish"
#define DATA_PATH_OPT                  "GISH_DATA_DIR"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("Gish is a side-scrolling platformer video game with some physics" \
                                                   "puzzle elements developed by Cryptic Sea (pseudonym of Alex " \
                                                   "Austin), Edmund McMillen, Josiah Pisciotta and published by " \
                                                   "Chronic Logic in 2004. Cryptic Sea pledged Gish to go open " \
                                                   "source under the GPLv2 on May 29, 2010.\n\n" \
                                                   "This is my port of the Gish game to Haiku OS which uses " \
                                                   "SDL2, OpenAL and Ogg Vorbis libraries.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("3dEyes**\n\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku OS:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("EXL\n")
#define L_ABOUT_LINK                   B_TRANSLATE("http://chroniclogic.com/gish.htm")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official site:")
#define L_ABOUT_SRC_LINK               B_TRANSLATE("http://github.com/EXL/Gish")
#define L_ABOUT_SRC_LINK_DESC          B_TRANSLATE("Source code:")
#define L_ABOUT_OTH_LINK               B_TRANSLATE("http://exlmoto.ru/gish-droid")
#define L_ABOUT_OTH_LINK_DESC          B_TRANSLATE("Information:")
#define L_DATA_LINK                    B_TRANSLATE("https://store.steampowered.com/app/9500/")
#define L_DATA_TEXT                    B_TRANSLATE("Gish game")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy game files on Steam: ")
#define L_DOT                          B_TRANSLATE(".")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("Required data file %file% not found.")

// Object Names
#define O_DOT                          "dotView"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK                    "dataLink"
#define O_DATA_LINK_DESC               "dataLinkDesc"

class GishAboutWindow : public BeAboutWindow
{
public:
	explicit GishAboutWindow(const BRect &frame, const char *title, const char *version)
	         : BeAboutWindow(frame, title, version)
	{
		BStringView *urlDescString = new BStringView(O_ABOUT_LINK_DESC, L_ABOUT_LINK_DESC);
		BeUrlStringView *urlString = new BeUrlStringView(O_ABOUT_LINK, L_ABOUT_LINK);
		BStringView *urlDescSrcString = new BStringView(O_ABOUT_LINK_DESC, L_ABOUT_SRC_LINK_DESC);
		BeUrlStringView *urlSrcString = new BeUrlStringView(O_ABOUT_LINK, L_ABOUT_SRC_LINK);
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
		                              .Add(urlDescSrcString)
		                              .Add(urlSrcString)
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

class GishLauncher : public BeLauncherBase
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
		path << "texture/face.tga";
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
		return BeLauncherBase::RunGameViaRoster(true);
	}

	virtual void
	ShowAboutDialog(void)
	{
		GishAboutWindow *gameAboutWindow = new GishAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                       TITLE, VERSION);
		gameAboutWindow->Show();
	}

public:
	explicit GishLauncher(const char *startPath)
	         : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH_OPT,
	                          startPath, true, true)
	{
		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC, L_DATA_FILES_LINK_D);
		BeUrlStringView *urlString = new BeUrlStringView(O_DATA_LINK, L_DATA_TEXT, L_DATA_LINK);
		BStringView *dot = new BStringView(O_DOT, L_DOT);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlDescString)
		                              .Add(urlString)
		                              .Add(dot)
		                              .AddGlue()
		                          .End();
		BeLauncherBase::GetAdditionalBox()->AddChild(boxLayout->View());
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	GishLauncher *gishLauncher = new GishLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(gishLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
