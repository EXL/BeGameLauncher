#include "BeApp.h"
#include "BeLauncherBase.h"
#include "BeAboutWindow.h"
#include "BeUrlStringView.h"
#include "BeUtils.h"

#include <Rect.h>
#include <String.h>
#include <StringView.h>
#include <CheckBox.h>
#include <Font.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <InterfaceDefs.h>

#include <Catalog.h>

#ifndef SIGNATURE
#error "Application SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "BasedGameLauncher"

// Launcher Settings
#define TITLE                          "Game Launcher"
#define VERSION                        "1.0.0"
#define PACKAGE_DIR                    "Game"
#define SETTINGS_FILE                  "GameLauncher.set"
#define EXECUTABLE_FILE                "GameExe"
#define DATA_PATH_ENV                  "DATA_PATH_ENV"

// Various Strings
#define L_ABOUT_STRING                 B_TRANSLATE("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do " \
                                       "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim" \
                                       "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea" \
                                       "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit" \
                                       "esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat " \
                                       "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim " \
                                       "id est laborum.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("- my gf")
#define L_ABOUT_LINK                   B_TRANSLATE("http://exlmoto.ru")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Some useful link: ")
#define L_DATA_LINK                    B_TRANSLATE("https://store.steampowered.com/")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy data files: ")

// Additional option
#define S_CHECKBOX_OPTION              "GAME_OPTION"
#define L_CHECKBOX_OPTION              B_TRANSLATE("Game option")
#define L_CHECKBOX_OPTION_TOOLTIP      B_TRANSLATE("Check to activate game option.")
#define L_CHECKBOX_SAVED_OPTION        B_TRANSLATE("Saved option: ")
#define O_CHECKBOX_OPTION              "checkBoxOption"

#define O_ABOUT_STRING                 "aboutString"
#define O_ABOUT_LINK                   "aboutLink"
#define O_ABOUT_LINK_DESC              "aboutLinkDesc"
#define O_DATA_LINK                    "dataLink"
#define O_DATA_LINK_DESC               "dataLinkDesc"

class GameAboutWindow : public BeAboutWindow
{
public:
	GameAboutWindow(const BRect &frame, const char *title, const char *version)
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

class BasedGameLauncher : public BeLauncherBase
{
	enum
	{
		MSG_CHECKBOX_STATE_CHANGED = 'chks'
	};

	BCheckBox *fCheckBoxOption;

protected:
	void
	MessageReceived(BMessage *msg)
	{
		switch (msg->what)
		{
			case MSG_CHECKBOX_STATE_CHANGED:
			{
				BCheckBox *checkBox = dynamic_cast<BCheckBox *>(FindView(O_CHECKBOX_OPTION));
				SetStatusString(B_COLOR_BLACK, BString(L_CHECKBOX_OPTION) << ": " << checkBox->Value());
				break;
			}
			default:
			{
				BeLauncherBase::MessageReceived(msg);
				break;
			}
		}
	}

	bool
	CheckCache(void)
	{
		return true;
	}

	bool
	ReadSettings(void)
	{
		if(!BeLauncherBase::ReadSettings())
		{
			BeDebug("[Info]: First run, set default values.");
			fCheckBoxOption->SetValue(1);
		}
		else
		{
			const char *str = BeLauncherBase::GetSettings()->GetString(S_CHECKBOX_OPTION);
			int ASCII_MAGIC = 48;
			int value = static_cast<int>(str[0] - ASCII_MAGIC);
			SetStatusString(B_COLOR_BLACK, BString(L_CHECKBOX_SAVED_OPTION) << value);
			fCheckBoxOption->SetValue(value);
		}
		return true;
	}

	void
	SaveSettings(bool def)
	{
		BString value;
		value << fCheckBoxOption->Value();
		BeLauncherBase::GetSettings()->SetString(S_CHECKBOX_OPTION, value);

		BeLauncherBase::SaveSettings(def);
	}

	void
	ShowAboutDialog(void)
	{
		GameAboutWindow *gameAboutWindow = new GameAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                       TITLE, VERSION);
		gameAboutWindow->Show();
	}

public:
	BasedGameLauncher(const char *startPath)
		: BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH_ENV,
	                     startPath, true, false)
	{
		fCheckBoxOption = new BCheckBox(O_CHECKBOX_OPTION, L_CHECKBOX_OPTION, new BMessage(MSG_CHECKBOX_STATE_CHANGED));
		fCheckBoxOption->SetToolTip(L_CHECKBOX_OPTION_TOOLTIP);

		BStringView *urlDescString = new BStringView(O_DATA_LINK_DESC, L_DATA_FILES_LINK_D);
		BeUrlStringView *urlString = new BeUrlStringView(O_DATA_LINK, L_DATA_LINK);

		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_VERTICAL, B_USE_HALF_ITEM_SPACING)
		                          .AddGroup(B_HORIZONTAL, 0.0f)
		                              .Add(fCheckBoxOption)
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
	BasedGameLauncher *basedGameLauncher = new BasedGameLauncher(BeUtils::GetPathToHomeDir());
	beApp->SetMainWindow(basedGameLauncher);
	beApp->Run();
	delete beApp;
	beApp = NULL;
	return 0;
}
