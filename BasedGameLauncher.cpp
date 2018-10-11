#include "BeApp.h"
#include "BeMainWindow.h"
#include "BeDirectoryFilter.h"
#include "BeUtils.h"
#include "BeSettings.h"

#include <Rect.h>
#include <View.h>
#include <InterfaceDefs.h>
#include <Button.h>
#include <Message.h>
#include <StringView.h>
#include <TextControl.h>
#include <FilePanel.h>
#include <Directory.h>
#include <Path.h>
#include <String.h>

#include <map>

// For Debug
#include <stdio.h>

#define SIGNATURE            "application/x-vnd.exl-BasedGameLauncher"
#define SETTINGS_FILE        "GameLauncher.set"
#define DATA_PATH            "DATA_PATH"

#define L_APP_NAME           "Game Launcher"
#define L_BTN_RUN            "Run!"
#define L_BTN_EXIT           "Exit"
#define L_BTN_ABOUT          "About..."
#define L_BTN_BROWSE         "..."
#define L_BTN_BROWSE_TOOLTIP "Click to open the file dialog."
#define L_SV_DATA            "Please select a directory with game files:"
#define L_SV_DATA_TOOLTIP    "Path to a directory with game files."
#define L_FP_TITLE           "Open Game Folder"

#define GAP        10.0f
#define BANNER_W   64.0f

class BeBasedWindow : public BeMainWindow
{
	enum
	{
		MSG_BUTTON_RUN_CLICKED        = 'btrn',
		MSG_BUTTON_EXIT_CLICKED       = 'btex',
		MSG_BUTTON_BROWSE_CLICKED     = 'btbr',
		MSG_BUTTON_ABOUT_CLICKED      = 'btab',
		MSG_FILE_PANEL_FILE_SELECTED  = 'fpsc'
	};

	BTextControl *fDataTextControl;
	BFilePanel *fFilePanel;
	BeDirectoryFilter *fDirectotyFilter;
	BeSettings *fSettings;
public:
	BeBasedWindow(void) : BeMainWindow(BRect(100, 100, 700, 500), L_APP_NAME)
	{
		fSettings = new BeSettings(SETTINGS_FILE);
		CreateForm();
		ReadSettings();
	}

	void CreateForm()
	{
		BRect r(Bounds());
		BRect bannerRect(r.left, r.top, BANNER_W, r.bottom);
		BRect stringViewRect(BANNER_W + GAP, BANNER_W + GAP, r.right, r.bottom);

		BView *mainView = new BView(r, "mainView", B_FOLLOW_ALL, B_WILL_DRAW);
		mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

		BView *bannerView = new BView(bannerRect, "bannerView", B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW);
		bannerView->SetViewColor(255, 205, 8);

		BStringView *dataStringView = new BStringView(stringViewRect, "dataStringView", L_SV_DATA, B_FOLLOW_LEFT);
		dataStringView->ResizeToPreferred();
		mainView->AddChild(dataStringView);

		//BRect dataTextControlRect(BANNER_W + GAP, BANNER_W + GAP * 3, r.right - GAP*4, r.bottom);
		BButton *buttonBrowse = new BButton(BRect(), "buttonBrowse", L_BTN_BROWSE, new BMessage(MSG_BUTTON_BROWSE_CLICKED), B_FOLLOW_RIGHT);
		buttonBrowse->ResizeToPreferred();
		buttonBrowse->ResizeTo(buttonBrowse->Bounds().Width() - GAP * 5, buttonBrowse->Bounds().Height());
		buttonBrowse->MoveTo(r.right - buttonBrowse->Bounds().Width() - GAP, BANNER_W + GAP * 3);
		buttonBrowse->SetToolTip(L_BTN_BROWSE_TOOLTIP);
		mainView->AddChild(buttonBrowse);

		BRect dataTextControlRect(BANNER_W + GAP, BANNER_W + GAP * 3, r.right - GAP*4, r.bottom);
		fDataTextControl = new BTextControl(dataTextControlRect, "dataTextControl", NULL, NULL, NULL, B_FOLLOW_LEFT_RIGHT);
		fDataTextControl->ResizeToPreferred();
		fDataTextControl->SetToolTip(L_SV_DATA_TOOLTIP);
		mainView->AddChild(fDataTextControl);
		buttonBrowse->ResizeTo(buttonBrowse->Bounds().Width(), fDataTextControl->Bounds().Height());

		BButton *buttonAbout = new BButton(BRect(), "buttonAbout", L_BTN_ABOUT, new BMessage(MSG_BUTTON_ABOUT_CLICKED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
		BButton *buttonRun = new BButton(BRect(), "buttonRun", L_BTN_RUN, new BMessage(MSG_BUTTON_RUN_CLICKED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
		BButton *buttonExit = new BButton(BRect(), "buttonExit", L_BTN_EXIT, new BMessage(MSG_BUTTON_EXIT_CLICKED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
		buttonRun->ResizeToPreferred();
		buttonExit->ResizeToPreferred();
		buttonAbout->ResizeToPreferred();
		buttonExit->MoveTo(r.right - buttonExit->Bounds().Width() - GAP, r.bottom - buttonExit->Bounds().Height() - GAP);
		buttonRun->MoveTo(r.right - buttonExit->Bounds().Width() - buttonRun->Bounds().Width() - GAP * 2, r.bottom - buttonRun->Bounds().Height() - GAP);
		buttonAbout->MoveTo(r.left + BANNER_W + GAP, r.bottom - buttonAbout->Bounds().Height() - GAP);
		mainView->AddChild(buttonExit);
		mainView->AddChild(buttonRun);
		mainView->AddChild(buttonAbout);
		SetDefaultButton(buttonRun);

		AddChild(mainView);
		AddChild(bannerView);

		fDirectotyFilter = new BeDirectoryFilter();
		entry_ref start_point;
		BEntry entry(BeUtils::GetPathToHomeDir().String());
		entry.GetRef(&start_point);
		fFilePanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(this), &start_point,
		                            B_DIRECTORY_NODE, false, new BMessage(MSG_FILE_PANEL_FILE_SELECTED),
		                            fDirectotyFilter, true);
		fFilePanel->Window()->SetTitle(L_FP_TITLE);

		SetSizeLimits(400.0f, 800.0f, 300.0f, 600.0f);
	}

	void MessageReceived(BMessage *msg)
	{
		switch (msg->what)
		{
			case MSG_BUTTON_RUN_CLICKED:
			{
				SaveSettings(false);
				//BeMainWindow::QuitRequested();
				break;
			}
			case MSG_BUTTON_EXIT_CLICKED:
			{
				BeMainWindow::QuitRequested();
				break;
			}
			case MSG_BUTTON_ABOUT_CLICKED:
			{
				BeMainWindow::QuitRequested();
				break;
			}
			case MSG_BUTTON_BROWSE_CLICKED:
			{
				ChooseDirectory();
				break;
			}
			case MSG_FILE_PANEL_FILE_SELECTED:
			{
				DirectorySelected();
				break;
			}
			default:
			{
				BeMainWindow::MessageReceived(msg);
				break;
			}
		}
	}

	void DirectorySelected(void)
	{
		entry_ref dirRef;
		fFilePanel->GetPanelDirectory(&dirRef);
		BEntry entry(&dirRef);
		BPath path;
		entry.GetPath(&path);
		fDataTextControl->SetText(path.Path());
	}

	void ChooseDirectory(void)
	{
		fFilePanel->Show();
	}

	void ReadSettings(void)
	{
		if(!fSettings->ReadSettingsFromFile())
		{
			SaveSettings(true);
		}

		fDataTextControl->SetText(fSettings->GetString(DATA_PATH));
	}

	void SaveSettings(bool def)
	{
		std::map<BString, BString> settings;
		settings[BString(DATA_PATH)] = (def) ? BString(BeUtils::GetPathToHomeDir()) :
		                                         BString(fDataTextControl->Text());

		fSettings->UpdateSettings(settings);
		fSettings->DumpSettingsToFile();
	}

	void ShowAboutDialog()
	{

	}

	void RunGame()
	{

	}

	void RunGameExecVe()
	{

	}
};

int
main(void)
{
	BeApp *beApp = new BeApp(SIGNATURE);
	BeBasedWindow *beBasedWindow = new BeBasedWindow();
	beApp->SetMainWindow(beBasedWindow);
	beApp->Run();
	delete beApp;
	return 0;
}
