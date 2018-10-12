#include "BeApp.h"
#include "BeLauncherBase.h"
#include "BeUtils.h"

#include <View.h>
#include <StringView.h>
#include <CheckBox.h>

// Launcher Settings
#define SIGNATURE            "application/x-vnd.exl-BasedGameLauncher"
#define TITLE                "Game Launcher"
#define PACKAGE_DIR          "Game"
#define SETTINGS_FILE        "GameLauncher.set"
#define EXECUTABLE_FILE      "GameExe"
#define DATA_PATH            "DATA_PATH"

// Various Strings
#define L_BUTTON_BROWSE_T    "Click to open the file dialog."
#define L_SV_DATA            "Please select a directory with game files:"
#define L_TC_DATA_T          "Path to a directory with game files."
#define L_FP_TITLE           "Please choose a Game Folder"

// Additional option
#define S_CHECKBOX_OPTION    "GAME_OPTION"
#define L_CHECKBOX_OPTION    "Game Option"
#define O_CHECKBOX_OPTION    "checkBoxOption"

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
			SetStatusString(COLOR_BLACK, BString("Saved Option ") << value);
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
				SetStatusString(COLOR_BLACK, BString("Game Option ") << checkBox->Value());
				break;
			}
			default:
			{
				BeLauncherBase::MessageReceived(msg);
				break;
			}
		}
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
