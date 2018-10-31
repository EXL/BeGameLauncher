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
#include <TextControl.h>

#include <Catalog.h>

#include <posix/stdlib.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "SeriousLauncher"

// Launcher Settings
#define TITLE                          "Serious Sam Classic Launcher"
#define VERSION                        "1.10"
#define PACKAGE_DIR                    "Serious"
#define SETTINGS_FILE                  "SeriousLauncher.set"
#define EXECUTABLE_FILE_TSE            "ssam"
#define EXECUTABLE_FILE_TFE            "ssam-tfe"
#define LIBRARY_DIR_TSE                "tse"
#define LIBRARY_DIR_TFE                "tfe"
#define DATA_PATH_OPT                  "SS_DIR_PATH"
#define LIBS_PATH_OPT                  "SS_LIB_PATH"
#define CMD_ARGS_OPT                   "SS_ARGS_CMD"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("The Serious Engine lets you run the \"Serious Sam Classic: " \
                                                   "The First Encounter\" and the \"Serious Sam Classic: The Second " \
                                                   "Encounter\" games. In addition, custom game mods are supported " \
                                                   "also. Croteam pledged Serious Engine to go open " \
                                                   "source under the GPLv2 on March 11, 2016.\n\n" \
                                                   "This is my port of the Serious Engine to Haiku OS which uses " \
                                                   "SDL2 and Ogg Vorbis libraries.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("ptitSeb, Ryan C. Gordon (icculus), 3dEyes**\n\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku OS:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("EXL\n")
#define L_ABOUT_LINK                   B_TRANSLATE("http://www.croteam.com")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official site:")
#define L_ABOUT_SRC_LINK               B_TRANSLATE("http://github.com/EXLMOTODEV/Serious-Engine")
#define L_ABOUT_SRC_LINK_DESC          B_TRANSLATE("Source code:")
#define L_ABOUT_OTH_LINK               B_TRANSLATE("http://exlmoto.ru/haiku-packages/#serious")
#define L_ABOUT_OTH_LINK_DESC          B_TRANSLATE("Information:")
#define L_DATA_TFE_LINK                B_TRANSLATE("https://store.steampowered.com/app/41050/")
#define L_DATA_TFE_TEXT                B_TRANSLATE("Serious Sam Classic: The First Encounter, Steam")
#define L_DATA_TSE_LINK                B_TRANSLATE("https://store.steampowered.com/app/41060/")
#define L_DATA_TSE_TEXT                B_TRANSLATE("Serious Sam Classic: The Second Encounter, Steam")
#define L_DATA_TFE_LINK_G              B_TRANSLATE("https://gog.com/game/serious_sam_the_first_encounter")
#define L_DATA_TFE_TEXT_G              B_TRANSLATE("Serious Sam Classic: The First Encounter, GOG.com")
#define L_DATA_TSE_LINK_G              B_TRANSLATE("https://gog.com/game/serious_sam_the_second_encounter")
#define L_DATA_TSE_TEXT_G              B_TRANSLATE("Serious Sam Classic: The Second Encounter, GOG.com")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy game files on: ")
#define L_TESTED                       B_TRANSLATE("(tested)")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("Required data files %file1% or %file2% are not found.")
#define L_ERROR_MANY_GAMEINFO_FILES    B_TRANSLATE("Game cache contains both files %file1% (TFE) and %file2% (TSE).")

// Object Names
#define O_TESTED                       "testedString"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK_DESC               "dataLinkDesc"
#define O_DATA_TSE_LINK                "dataLinkTSE"
#define O_DATA_TFE_LINK                "dataLinkTFE"
#define O_DATA_TSE_LINK_G              "dataLinkTSEG"
#define O_DATA_TFE_LINK_G              "dataLinkTFEG"

class SeriousAboutWindow : public BeAboutWindow
{
public:
	explicit SeriousAboutWindow(const BRect &frame, const char *title, const char *version)
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

class SeriousLauncher : public BeLauncherBase
{
	virtual bool
	CheckCache(void)
	{
		BString path = BeLauncherBase::GetTextControl()->Text();
		if(!path.EndsWith("/"))
		{
			path << "/";
		}

		// NOTE: Check some files in the cache to determine Serious Same game version.
		bool isTheFirstEncounterData = true;
		bool isTheSecondEncounterData = true;

		BString fileTFEF = path;
		BString fileTFES = path;
		BString fileTFENameF = "1_00c.gro";
		BString fileTFENameS = "1_00.gro";
		fileTFEF << fileTFENameF;
		fileTFES << fileTFENameS;
		BEntry fileTFEFToCheck(fileTFEF);
		BEntry fileTFESToCheck(fileTFES);
		if(!fileTFEFToCheck.Exists() && !fileTFESToCheck.Exists())
		{
			isTheFirstEncounterData = false;
		}

		BString fileTSE = path;
		BString fileTSEName = "SE1_00.gro";
		fileTSE << fileTSEName;
		BEntry fileTSEToCheck(fileTSE);
		if(!fileTSEToCheck.Exists()) 
		{
			isTheSecondEncounterData = false;
		}

		if(!isTheFirstEncounterData && !isTheSecondEncounterData)
		{
			BString errorMessage(L_ERROR_NO_GAMEINFO_FILE);
			errorMessage.ReplaceAll("%file1%", fileTFENameF);
			errorMessage.ReplaceAll("%file2%", fileTSEName);
			SetStatusString(B_COLOR_RED, errorMessage);
			return false;
		}
		else if(isTheFirstEncounterData && isTheSecondEncounterData)
		{
			BString errorMessage(L_ERROR_MANY_GAMEINFO_FILES);
			errorMessage.ReplaceAll("%file1%", fileTFENameF);
			errorMessage.ReplaceAll("%file2%", fileTSEName);
			SetStatusString(B_COLOR_RED, errorMessage);
			return false;
		}

		if(isTheFirstEncounterData)
		{
			BeLauncherBase::SetExecutablePath(BeUtils::GetPathToExecutable(PACKAGE_DIR,
			                                                               EXECUTABLE_FILE_TFE));
		}

		// NOTE: Set library path for Serious Same game version.
		BString libraryPath = BeUtils::GetPathToExecutable(PACKAGE_DIR, LIBRARY_DIR_TSE);
		if(isTheFirstEncounterData)
		{
			libraryPath = BeUtils::GetPathToExecutable(PACKAGE_DIR, LIBRARY_DIR_TFE);
		}
		if(!libraryPath.EndsWith("/"))
		{
			libraryPath << "/";
		}

		setenv(LIBS_PATH_OPT, libraryPath.String(), 1);

		return true;
	}

	virtual bool
	RunGame(void)
	{
		return BeLauncherBase::RunGameViaRoster(true, false);
	}

	virtual void
	ShowAboutDialog(void)
	{
		SeriousAboutWindow *gameAboutWindow = new SeriousAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                             TITLE, VERSION);
		gameAboutWindow->Show();
	}

public:
	explicit SeriousLauncher(const char *startPath)
	         : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE_TSE, SETTINGS_FILE, DATA_PATH_OPT,
	                          startPath, true, true)
	{
		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC, L_DATA_FILES_LINK_D);
		BeUrlStringView *urlStringS = new BeUrlStringView(O_DATA_TSE_LINK, L_DATA_TSE_TEXT, L_DATA_TSE_LINK);
		BeUrlStringView *urlStringF = new BeUrlStringView(O_DATA_TFE_LINK, L_DATA_TFE_TEXT, L_DATA_TFE_LINK);
		BeUrlStringView *urlStringSG = new BeUrlStringView(O_DATA_TSE_LINK_G, L_DATA_TSE_TEXT_G, L_DATA_TSE_LINK_G);
		BeUrlStringView *urlStringFG = new BeUrlStringView(O_DATA_TFE_LINK_G, L_DATA_TFE_TEXT_G, L_DATA_TFE_LINK_G);
		BStringView *testedS = new BStringView(O_TESTED, L_TESTED);
		BStringView *testedF = new BStringView(O_TESTED, L_TESTED);
		BFont font;
		BeLauncherBase::GetAdditionalBox()->GetFont(&font);
		font.SetFace(B_ITALIC_FACE);
		testedS->SetFont(&font, B_FONT_FAMILY_AND_STYLE | B_FONT_FLAGS);
		testedF->SetFont(&font, B_FONT_FAMILY_AND_STYLE | B_FONT_FLAGS);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, 0.0f)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlDescString)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                              .Add(urlStringSG)
		                              .Add(testedS)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                              .Add(urlStringFG)
		                              .Add(testedF)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlStringS)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlStringF)
		                              .AddGlue()
		                          .End();
		BeLauncherBase::GetAdditionalBox()->AddChild(boxLayout->View());
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	SeriousLauncher *seriousLauncher = new SeriousLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(seriousLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
