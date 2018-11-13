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

#include <Catalog.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "VangersLauncher"

// Launcher Settings
#define TITLE                          "Vangers Launcher"
#define VERSION                        "1.0.0"
#define PACKAGE_DIR                    "Vangers"
#define SETTINGS_FILE                  "VangersLauncher.set"
#define EXECUTABLE_FILE_ENG            "engine/Vangers-ENG"
#define EXECUTABLE_FILE_RUS            "engine/Vangers-RUS"
#define DATA_PATH_OPT                  "VANGERS_DATA"
#define LANG_OPT                       "LANG_OPT"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do " \
                                       "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim " \
                                       "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea " \
                                       "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit " \
                                       "esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat " \
                                       "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim " \
                                       "id est laborum.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("- my gf")
#define L_ABOUT_LINK                   B_TRANSLATE("http://exlmoto.ru")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Some useful link: ")
#define L_DATA_LINK                    B_TRANSLATE("https://store.steampowered.com/app/264080/")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy data files on Steam: ")
#define L_RADIO_OPTION_ENG             B_TRANSLATE("English")
#define L_RADIO_OPTION_RUS             B_TRANSLATE("Russian")
#define L_RADIO_OPTION_ENG_TOOLTIP     B_TRANSLATE("Check to activate English version of Vangers.")
#define L_RADIO_OPTION_RUS_TOOLTIP     B_TRANSLATE("Check to activate Russian version of Vangers.")
#define L_RADIO_GROUP_LABEL            B_TRANSLATE("Choose language version:")
#define L_ERROR_NO_GAMEINFO_FILE       B_TRANSLATE("Required data file %file% not found.")
#define L_ERROR_SCRIPT_DIR             B_TRANSLATE("Required script directory %dir% not found.")
#define L_COPYING_MESSAGE              B_TRANSLATE("Copying %dir1% to %dir2%...")

// Object names
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK                    "dataLink"
#define O_DATA_LINK_DESC               "dataLinkDesc"
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

		BeAboutWindow::GetInformationView()->Insert(L_ABOUT_STRING);
		BeAboutWindow::GetInformationView()->SetFontAndColor(be_bold_font);
		BeAboutWindow::GetInformationView()->Insert(L_ABOUT_THANKS_STR_H);
		BeAboutWindow::GetInformationView()->SetFontAndColor(be_plain_font);
		BeAboutWindow::GetInformationView()->Insert(L_ABOUT_THANKS_STR);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
		                          .Add(urlDescString)
		                          .Add(urlString)
		                          .AddGlue();
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

		// NOTE: 1. Check some file in the game cache.
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

		// NOTE: 2. Check two script directories.
		BString iscreenDir = path;
		iscreenDir << "iscreen/";
		BString actintDir = path;
		actintDir << "actint/";
		bool iscreenExist = CheckDirectory(iscreenDir);
		bool actintExist = CheckDirectory(actintDir);

		// NOTE: 3. Copy resource directories from package catalog if needed.
		if(!iscreenExist)
		{
			BString copyingMessage(L_COPYING_MESSAGE);
			BString iscreenDirSys = BeUtils::GetPathToPackage(PACKAGE_DIR);
			iscreenDirSys << "/engine/iscreen/";
			copyingMessage.ReplaceAll("%dir1%", iscreenDirSys);
			copyingMessage.ReplaceAll("%dir2%", iscreenDir);
			SetStatusString(B_COLOR_GREEN, copyingMessage);

			// NOTE: This is ugly. UGLY! Need to rewrite to Haiku API.
			BString copyCommand = "cp -aR ";
			copyCommand << iscreenDirSys;
			copyCommand	<< " ";
			copyCommand << iscreenDir;
			system(copyCommand);
		}
		if(!actintExist)
		{
			BString copyingMessage(L_COPYING_MESSAGE);
			BString actintDirSys = BeUtils::GetPathToPackage(PACKAGE_DIR);
			actintDirSys << "/engine/actint/";
			copyingMessage.ReplaceAll("%dir1%", actintDirSys);
			copyingMessage.ReplaceAll("%dir2%", actintDir);
			SetStatusString(B_COLOR_GREEN, copyingMessage);

			// NOTE: This is ugly. UGLY! Need to rewrite to Haiku API.
			BString copyCommand = "cp -aR ";
			copyCommand << actintDirSys;
			copyCommand	<< " ";
			copyCommand << actintDir;
			system(copyCommand);
		}

		// NOTE: 4. Check script directories again.
		if(!(iscreenExist && actintExist))
		{
			iscreenExist = CheckDirectory(iscreenDir);
			actintExist = CheckDirectory(actintDir);
			if(!(iscreenExist && actintExist))
			{
				BString errorMessage(L_ERROR_SCRIPT_DIR);
				errorMessage.ReplaceAll("%dir%", (!iscreenExist) ? iscreenDir : actintDir);
				SetStatusString(B_COLOR_RED, errorMessage);
				return false;
			}
		}

		return true;
	}

	virtual bool
	RunGame(void)
	{
		if(version == RUSSIAN)
		{
			BeLauncherBase::SetExecutablePath(BeUtils::GetPathToExecutable(PACKAGE_DIR,
			                                                               EXECUTABLE_FILE_RUS));
		}

		return BeLauncherBase::RunGameViaRoster(true, false);
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
		                                                                                    -(Gap() * 3)),
		                                                                TITLE, VERSION);
		vangersAboutWindow->Show();
	}

public:
	explicit VangersLauncher(const char *startPath)
	         : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE_ENG, SETTINGS_FILE, DATA_PATH_OPT,
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
		BeUrlStringView *urlString = new BeUrlStringView(O_DATA_LINK, L_DATA_LINK);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, B_USE_HALF_ITEM_SPACING)
		                              .Add(radioLabel)
		                              .Add(fChooseEngVersion)
		                              .Add(fChooseRusVersion)
		                              .AddGlue()
		                          .End()
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(urlDescString)
		                              .Add(urlString)
		                              .AddGlue()
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
