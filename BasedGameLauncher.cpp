#include "BeApp.h"
#include "BeLauncherBase.h"
#include "BeAboutWindow.h"
#include "BeUtils.h"
#include "BeMultiStringView.h"
#include "BeUrlStringView.h"

#include <Rect.h>
#include <View.h>
#include <StringView.h>
#include <CheckBox.h>
#include <Font.h>
#include <Box.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>

#include <Catalog.h>

#ifndef SIGNATURE
#error "App SIGNATURE is not defined. Check your build system."
#endif // !SIGNATURE

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "BasedGameLauncher"

// Launcher Settings
#define TITLE                          "Game Launcher"
#define VERSION                        "1.0.0"
#define PACKAGE_DIR                    "Game"
#define SETTINGS_FILE                  "GameLauncher.set"
#define EXECUTABLE_FILE                "GameExe"
#define DATA_PATH                      "DATA_PATH"

// Various Strings
#define L_BUTTON_BROWSE_T              B_TRANSLATE("Click to open the file dialog.")
#define L_SV_DATA                      B_TRANSLATE("Please select a directory with game files: ")
#define L_TC_DATA_T                    B_TRANSLATE("Path to a directory with game files.")
#define L_FP_TITLE                     B_TRANSLATE("Please choose a Game Folder")
#define L_ABOUT_STRING                 B_TRANSLATE("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do " \
                                       "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim" \
                                       "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea" \
                                       "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit" \
                                       "esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat " \
                                       "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim " \
                                       "id est laborum.\n\n")
#define L_ABOUT_THANKS_STR_H           B_TRANSLATE("Thanks to:\n\t")
#define L_ABOUT_THANKS_STR             B_TRANSLATE("- my gf\n")
#define L_ABOUT_LINK                   B_TRANSLATE("http://exlmoto.ru")
#define L_ABOUT_LINK_DESC              B_TRANSLATE("Some useful link: ")
#define L_DATA_LINK                    B_TRANSLATE("https://store.steampowered.com/")
#define L_DATA_FILES_LINK_D            B_TRANSLATE("Buy data files: ")

// Additional option
#define S_CHECKBOX_OPTION              "GAME_OPTION"
#define L_CHECKBOX_OPTION              B_TRANSLATE("Game Option")
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

		BeMultiStringView *informationText = BeAboutWindow::GetInformationView();
		informationText->Insert(L_ABOUT_STRING);
		informationText->SetFontAndColor(be_bold_font);
		informationText->Insert(L_ABOUT_THANKS_STR_H);
		informationText->SetFontAndColor(be_plain_font);
		informationText->Insert(L_ABOUT_THANKS_STR);

		BBox *additionalBox = BeAboutWindow::GetAdditionalBox();
		BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
		                          .Add(urlDescString)
		                          .Add(urlString)
		                          .AddGlue();
		additionalBox->AddChild(boxLayout->View());
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
		: BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE, DATA_PATH, true)
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

		BStringView *urlDescString = new BStringView(BRect(), O_DATA_LINK_DESC,
		                                             L_DATA_FILES_LINK_D, B_FOLLOW_LEFT);
		urlDescString->ResizeToPreferred();
		urlDescString->MoveTo(r.left, r.top + Gap() * 6);

		BeUrlStringView *urlString = new BeUrlStringView(O_DATA_LINK, L_DATA_LINK);
		urlString->ResizeToPreferred();
		urlString->MoveTo(r.left + urlDescString->Bounds().Width() + BeLauncherBase::Gap(),
		                  r.top + Gap() * 6);

		ui->AddChild(fCheckBoxOption);
		ui->AddChild(urlDescString);
		ui->AddChild(urlString);
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
