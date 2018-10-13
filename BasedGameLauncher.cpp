#include "BeApp.h"
#include "BeLauncherBase.h"
#include "BeAboutWindow.h"
#include "BeUtils.h"
#include "BeMultiStringView.h"

#include <Rect.h>
#include <View.h>
#include <StringView.h>
#include <CheckBox.h>
#include <Font.h>

// Launcher Settings
#define SIGNATURE            "application/x-vnd.exl-BasedGameLauncher"
#define TITLE                "Game Launcher"
#define VERSION              "1.0.0"
#define PACKAGE_DIR          "Game"
#define SETTINGS_FILE        "GameLauncher.set"
#define EXECUTABLE_FILE      "GameExe"
#define DATA_PATH            "DATA_PATH"

// Various Strings
#define L_BUTTON_BROWSE_T    "Click to open the file dialog."
#define L_SV_DATA            "Please select a directory with game files:"
#define L_TC_DATA_T          "Path to a directory with game files."
#define L_FP_TITLE           "Please choose a Game Folder"
#define L_ABOUT_STRING       "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor" \
                             "incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud" \
                             "exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure" \
                             "dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. " \
                             "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt " \
                             "mollit anim id est laborum.\n\n"
#define L_ABOUT_THANKS_STR_H "Thanks to:\n\t"
#define L_ABOUT_THANKS_STR   "- my gf"

// Additional option
#define S_CHECKBOX_OPTION    "GAME_OPTION"
#define L_CHECKBOX_OPTION    "Game Option"
#define O_CHECKBOX_OPTION    "checkBoxOption"

#define O_ABOUT_STRING       "aboutString"

class GameAboutWindow : public BeAboutWindow
{
public:
	GameAboutWindow(const BRect &frame, const char *title, const char *version)
	    : BeAboutWindow(frame, title, version)
	{
		BeAboutWindow::CreateForm();
	}

	void SetAboutText()
	{
		BeAboutWindow::SetAboutText();

		BView *ui = BeAboutWindow::GetTextView();

		BeMultiStringView *aboutView = new BeMultiStringView(O_ABOUT_STRING, ui);
		aboutView->MoveTo(0, 0);
		aboutView->Insert(L_ABOUT_STRING);
		aboutView->SetFontAndColor(be_bold_font);
		aboutView->Insert(L_ABOUT_THANKS_STR_H);
		aboutView->SetFontAndColor(be_plain_font);
		aboutView->Insert(L_ABOUT_THANKS_STR);

		ui->AddChild(aboutView);
	}
};

class BasedGameLauncher : public BeLauncherBase
{
	enum
	{
		MSG_CHECKBOX_STATE_CHANGED = 'chks'
	};

	BCheckBox *fCheckBoxOption;
public:
	BasedGameLauncher(void)
		: BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH)
	{
		BeLauncherBase::InitParameters(L_SV_DATA, L_TC_DATA_T, L_BUTTON_BROWSE_T, L_FP_TITLE);
		CreateForm();
		ReadSettings();
	}

	const char *
	SetDefaultDir()
	{
		return BeUtils::GetPathToHomeDir();
	}

	bool
	CheckCache()
	{
		return true;
	}

	void
	CreateForm()
	{
		BeLauncherBase::CreateForm();

		BView *ui = BeLauncherBase::GetMainView();

		fCheckBoxOption = new BCheckBox(BRect(), O_CHECKBOX_OPTION, L_CHECKBOX_OPTION,
		                                          new BMessage(MSG_CHECKBOX_STATE_CHANGED), B_FOLLOW_LEFT);

		fCheckBoxOption->ResizeToPreferred();

		BRect r = BeLauncherBase::GetTextControl()->Frame();
		fCheckBoxOption->MoveTo(r.left, r.top + Gap() * 3);

		ui->AddChild(fCheckBoxOption);
	}

	bool
	ReadSettings()
	{
		if(!BeLauncherBase::ReadSettings())
		{
			// First run, set default value
			fCheckBoxOption->SetValue(1);
		}
		else
		{
			const char *str = BeLauncherBase::GetSettings()->GetString(S_CHECKBOX_OPTION);
			int ASCII_MAGIC = 48;
			int value = static_cast<int>(str[0] - ASCII_MAGIC);
			SetStatusString(B_COLOR_BLACK, BString("Saved Option ") << value);
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
	MessageReceived(BMessage *msg)
	{
		switch (msg->what)
		{
			case MSG_CHECKBOX_STATE_CHANGED:
			{
				BCheckBox *checkBox = dynamic_cast<BCheckBox *>(FindView(O_CHECKBOX_OPTION));
				SetStatusString(B_COLOR_BLACK, BString("Game Option ") << checkBox->Value());
				break;
			}
			default:
			{
				BeLauncherBase::MessageReceived(msg);
				break;
			}
		}
	}

	void
	ShowAboutDialog()
	{
		GameAboutWindow *gameAboutWindow = new GameAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)),
		                                                       TITLE, VERSION);
		gameAboutWindow->Show();
	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	BasedGameLauncher *beBasedGameLauncher = new BasedGameLauncher();
	beApp->SetMainWindow(beBasedGameLauncher);
	beApp->Run();
	delete beApp;
	return 0;
}
