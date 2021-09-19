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

#include "BeApp.h"
#include "BeLauncherBase.h"
#include "BeAboutWindow.h"
#include "BeUrlStringView.h"
#include "BeUtils.h"

#include <Rect.h>
#include <String.h>
#include <StringView.h>
#include <RadioButton.h>
#include <Font.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <InterfaceDefs.h>
#include <Directory.h>
#include <Font.h>

#include <Catalog.h>
#include <unistd.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "VangersLauncher"

// Launcher Settings
#define TITLE                          "Vangers Launcher"
#define VERSION                        "1.46.0"
#define PACKAGE_DIR                    "Vangers"
#define SETTINGS_FILE                  "VangersLauncher.set"
#define EXECUTABLE_FILE                "engine/Vangers"
#define DATA_PATH_OPT                  "VANGERS_DATA"
#define LANG_OPT                       "LANG_OPT"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("Mind-altering retro (1998) sandbox game that can be described as " \
                                                   "a racing role-playing adventure game with a complicated storyline. " \
                                                   "You’ll find a unique voxel atmosphere, complete freedom and lots " \
                                                   "of gameplay hours. " \
                                                   "KranX Productions pledged Vangers to go open " \
                                                   "source under the GPLv3 on March 12, 2016.\n\n" \
                                                   "This is my port of the Vangers to Haiku OS which uses " \
                                                   "SDL2, SDL2_net, Clunk and Ogg Vorbis libraries.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("stalkerg, K-D Lab, KranX Productions, 3dEyes**," \
                                                   "\n\tTK-313, Pavel Babayev, anttv\n\n")
#define L_ABOUT_PORT_STR_H             B_TRANSLATE("Port to Haiku OS:\n\t")
#define L_ABOUT_PORT_STR               B_TRANSLATE("EXL\n")
#define L_ABOUT_LINK                   B_TRANSLATE("http://kranx.com")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Official site:")
#define L_ABOUT_SRC_LINK               B_TRANSLATE("http://github.com/KranX/Vangers")
#define L_ABOUT_SRC_LINK_DESC          B_TRANSLATE("Source code:")
#define L_ABOUT_OTH_LINK               B_TRANSLATE("http://exlmoto.ru/haiku-packages/#vangers")
#define L_ABOUT_OTH_LINK_DESC          B_TRANSLATE("Information:")
#define L_DATA_LINK                    B_TRANSLATE("https://store.steampowered.com/app/264080/")
#define L_DATA_TEXT                    B_TRANSLATE("The Vangers on Steam")
#define L_DATA_LINK_G                  B_TRANSLATE("https://gog.com/game/vangers")
#define L_DATA_TEXT_G                  B_TRANSLATE("The Vangers on GOG.com")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy Vangers game files: ")
#define L_TESTED                       B_TRANSLATE("(tested)")

#define L_RADIO_OPTION_ENG             B_TRANSLATE("English")
#define L_RADIO_OPTION_RUS             B_TRANSLATE("Russian")
#define L_RADIO_OPTION_ENG_TOOLTIP     B_TRANSLATE("Check to activate English version of Vangers.")
#define L_RADIO_OPTION_RUS_TOOLTIP     B_TRANSLATE("Check to activate Russian version of Vangers.")
#define L_RADIO_GROUP_LABEL            B_TRANSLATE("Choose language version:")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("Required data file %file% not found.")
#define L_ERROR_SCRIPT_DIR             B_TRANSLATE("Required script directory %dir% not found.")
#define L_COPYING_MESSAGE              B_TRANSLATE("Copying %dir1% to %dir2%...")

// Object names
#define O_TESTED                       "testedString"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK                    "dataLink"
#define O_DATA_LINK_DESC               "dataLinkDesc"
#define O_DATA_LINK_G                  "dataLinkG"
#define O_DATA_LINK_DESC_G             "dataLinkDescG"
#define O_RADIO_OPTION_ENG             "radioOptionEng"
#define O_RADIO_OPTION_RUS             "radioOptionRus"
#define O_RADIO_GROUP_LABEL            "radioGroupLabel"

class VangersAboutWindow : public BeAboutWindow
{
public:
	explicit VangersAboutWindow(const BRect &frame, const char *title, const char *version)
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

class VangersLauncher : public BeLauncherBase
{
	enum LangVersion
	{
		ENGLISH,
		RUSSIAN
	};

	enum
	{
		MSG_RADIO_OPT_RUS_STATE_CHANGED = 'rors',
		MSG_RADIO_OPT_ENG_STATE_CHANGED = 'roes'
	};

	BRadioButton *fChooseEngVersion;
	BRadioButton *fChooseRusVersion;

	LangVersion version;

	bool
	CheckDirectory(const BString &path)
	{
		BEntry directoryPath(path);
		entry_ref ref;
		directoryPath.GetRef(&ref);
		BDirectory directory;
		if(directory.SetTo(&ref) != B_OK)
		{
			return false;
		}
		return true;
	}

protected:
	virtual void
	MessageReceived(BMessage *msg)
	{
		switch (msg->what)
		{
			case MSG_RADIO_OPT_RUS_STATE_CHANGED:
			{
				fChooseEngVersion->SetValue(B_CONTROL_OFF);
				version = RUSSIAN;
				break;
			}
			case MSG_RADIO_OPT_ENG_STATE_CHANGED:
			{
				fChooseRusVersion->SetValue(B_CONTROL_OFF);
				version = ENGLISH;
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
		BString filePath = path;
		filePath << "resource/video/mech00.avi";
		BEntry fileToCheck(filePath);
		if(!fileToCheck.Exists())
		{
			BString errorMessage(L_ERROR_NO_GAMEINFO_FILE);
			errorMessage.ReplaceAll("%file%", filePath);
			SetStatusString(B_COLOR_RED, errorMessage);
			return false;
		}
		return true;
	}

	virtual bool
	RunGame(void)
	{
		if(version == RUSSIAN)
		{
			BeLauncherBase::SetCustomArgs("-russian");
		}
		chdir(GetTextControl()->Text());
		return BeLauncherBase::RunGameViaRoster(false, true);
	}

	virtual bool
	ReadSettings(void)
	{
		if(!BeLauncherBase::ReadSettings())
		{
			BeDebug("[Info]: First run, set default values.\n");
			fChooseEngVersion->SetValue(B_CONTROL_ON);
			fChooseRusVersion->SetValue(B_CONTROL_OFF);
			version = ENGLISH;
		}
		else
		{
			const char *str = BeLauncherBase::GetSettings()->GetSettingsString(LANG_OPT);
			int ASCII_MAGIC = 48;
			int value = static_cast<int>(str[0] - ASCII_MAGIC);
			if(value == RUSSIAN)
			{
				fChooseRusVersion->SetValue(B_CONTROL_ON);
				fChooseEngVersion->SetValue(B_CONTROL_OFF);
			}
			else
			{
				fChooseRusVersion->SetValue(B_CONTROL_OFF);
				fChooseEngVersion->SetValue(B_CONTROL_ON);
			}
			version = static_cast<LangVersion>(value);
		}
		return true;
	}

	virtual void
	SaveSettings(bool def)
	{
		BString value;
		if(fChooseRusVersion->Value() == B_CONTROL_ON)
		{
			value << RUSSIAN;
		}
		else
		{
			value << ENGLISH;
		}
		BeLauncherBase::GetSettings()->SetSettingsString(LANG_OPT, value);

		BeLauncherBase::SaveSettings(def);
	}

	virtual void
	ShowAboutDialog(void)
	{
		VangersAboutWindow *vangersAboutWindow = new VangersAboutWindow(Frame().InsetBySelf(BannerWidth(),
		                                                                                    -(Gap() * 4)),
		                                                                TITLE, VERSION);
		vangersAboutWindow->Show();
	}

public:
	explicit VangersLauncher(const char *startPath)
	         : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH_OPT,
	                          startPath, true, false)
	{
		fChooseEngVersion = new BRadioButton(O_RADIO_OPTION_ENG, L_RADIO_OPTION_ENG,
		                                     new BMessage(MSG_RADIO_OPT_ENG_STATE_CHANGED));
		fChooseRusVersion = new BRadioButton(O_RADIO_OPTION_RUS, L_RADIO_OPTION_RUS,
		                                     new BMessage(MSG_RADIO_OPT_RUS_STATE_CHANGED));
		fChooseEngVersion->SetToolTip(L_RADIO_OPTION_ENG_TOOLTIP);
		fChooseRusVersion->SetToolTip(L_RADIO_OPTION_RUS_TOOLTIP);

		BStringView *radioLabel = new BStringView(O_RADIO_GROUP_LABEL, L_RADIO_GROUP_LABEL);

		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC, L_DATA_FILES_LINK_D);
		BeUrlStringView *urlString = new BeUrlStringView(O_DATA_LINK, L_DATA_TEXT, L_DATA_LINK);
		BeUrlStringView *urlStringGOG = new BeUrlStringView(O_DATA_LINK_G, L_DATA_TEXT_G, L_DATA_LINK_G);

		BStringView *testedString = new BStringView(O_TESTED, L_TESTED);
		BFont font;
		BeLauncherBase::GetAdditionalBox()->GetFont(&font);
		font.SetFace(B_ITALIC_FACE);
		testedString->SetFont(&font, B_FONT_FAMILY_AND_STYLE | B_FONT_FLAGS);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                              .Add(radioLabel)
		                              .Add(fChooseEngVersion)
		                              .Add(fChooseRusVersion)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_VERTICAL, 0.0f)
		                              .AddGroup(B_HORIZONTAL, 0.0f)
		                                  .Add(urlDescString)
		                                  .AddGlue()
		                              .End()
		                              .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                                  .Add(urlStringGOG)
		                                  .Add(testedString)
		                                  .AddGlue()
		                              .End()
		                              .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                                  .Add(urlString)
		                                  .AddGlue()
		                              .End()
		                          .End();
		BeLauncherBase::GetAdditionalBox()->AddChild(boxLayout->View());

		// NOTE: Be sure to call read settings function.
		ReadSettings();
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	VangersLauncher *vangersLauncher = new VangersLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(vangersLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
