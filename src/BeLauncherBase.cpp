#include "BeLauncherBase.h"
#include "BeUtils.h"

#include <Rect.h>
#include <InterfaceDefs.h>
#include <StringView.h>
#include <Button.h>
#include <Message.h>
#include <Messenger.h>
#include <TextControl.h>
#include <FilePanel.h>
#include <Entry.h>
#include <Directory.h>
#include <Path.h>

#define GAP                        10.0f
#define BANNER_W                   64.0f

#define L_BTN_RUN                  "Run!"
#define L_BTN_EXIT                 "Exit"
#define L_BTN_ABOUT                "About..."
#define L_BTN_BROWSE               "..."

#define O_MAIN_VIEW                "mainView"
#define O_BANNER_VIEW              "bannerView"
#define O_DATA_SVIEW               "dataStringView"
#define O_DATA_TCONTROL            "dataTextControl"
#define O_BTN_BROWSE               "buttonBrowse"
#define O_BTN_ABOUT                "buttonAbout"
#define O_BTN_RUN                  "buttonRun"
#define O_BTN_EXIT                 "buttonExit"

BeLauncherBase::BeLauncherBase(const char *windowTitle, const char *packageName,
                               const char *executableFileName, const char *settingFileName)
	: BeMainWindow(BRect(100, 100, 700, 500), windowTitle), sPackageName(packageName),
	  sExecutableFileName(executableFileName), sSettingsFileName(settingFileName)
{

}

void
BeLauncherBase::InitParameters()
{
	sDataPath = "DATA_PATH";
	sButtonBrowseToolTip = "Click to open the file dialog.";
	sStringViewData = "Please select a directory with game files:";
	sTextControlToolTip = "Path to a directory with game files.";
	sFilePanelTitle = "Open Game Folder";

	fExecutableFilePath = BeUtils::GetPathToExecutable(sPackageName, sExecutableFileName);

	fSettings = new BeSettings(sSettingsFileName);
}

void
BeLauncherBase::CreateForm()
{
	BRect r(Bounds());
	BRect bannerRect(r.left, r.top, BANNER_W, r.bottom);
	BRect stringViewRect(BANNER_W + GAP, BANNER_W + GAP, r.right, r.bottom);

	fMainView = new BView(r, O_MAIN_VIEW, B_FOLLOW_ALL, B_WILL_DRAW);
	fMainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BView *bannerView = new BView(bannerRect, O_BANNER_VIEW, B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW);
	bannerView->SetViewColor(255, 205, 8);

	BStringView *dataStringView = new BStringView(stringViewRect, O_DATA_SVIEW, sStringViewData, B_FOLLOW_LEFT);
	dataStringView->ResizeToPreferred();
	fMainView->AddChild(dataStringView);

	BButton *buttonBrowse = new BButton(BRect(), O_BTN_BROWSE, L_BTN_BROWSE,
	                                    new BMessage(MSG_BUTTON_BROWSE_CLICKED), B_FOLLOW_RIGHT);
	buttonBrowse->ResizeToPreferred();
	buttonBrowse->ResizeTo(buttonBrowse->Bounds().Width() - GAP * 5, buttonBrowse->Bounds().Height());
	buttonBrowse->MoveTo(r.right - buttonBrowse->Bounds().Width() - GAP, BANNER_W + GAP * 3);
	buttonBrowse->SetToolTip(sButtonBrowseToolTip);
	fMainView->AddChild(buttonBrowse);

	BRect dataTextControlRect(BANNER_W + GAP, BANNER_W + GAP * 3, r.right - GAP*4, r.bottom);
	fDataTextControl = new BTextControl(dataTextControlRect, O_DATA_TCONTROL, NULL, NULL, NULL, B_FOLLOW_LEFT_RIGHT);
	fDataTextControl->ResizeToPreferred();
	fDataTextControl->SetToolTip(sTextControlToolTip);
	fMainView->AddChild(fDataTextControl);
	buttonBrowse->ResizeTo(buttonBrowse->Bounds().Width(), fDataTextControl->Bounds().Height());

	BButton *buttonAbout = new BButton(BRect(), O_BTN_ABOUT, L_BTN_ABOUT,
	                                   new BMessage(MSG_BUTTON_ABOUT_CLICKED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	BButton *buttonRun = new BButton(BRect(), O_BTN_RUN, L_BTN_RUN,
	                                 new BMessage(MSG_BUTTON_RUN_CLICKED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	BButton *buttonExit = new BButton(BRect(), O_BTN_EXIT, L_BTN_EXIT,
	                                  new BMessage(MSG_BUTTON_EXIT_CLICKED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	buttonRun->ResizeToPreferred();
	buttonExit->ResizeToPreferred();
	buttonAbout->ResizeToPreferred();
	buttonExit->MoveTo(r.right - buttonExit->Bounds().Width() - GAP, r.bottom - buttonExit->Bounds().Height() - GAP);
	buttonRun->MoveTo(r.right - buttonExit->Bounds().Width() - buttonRun->Bounds().Width() - GAP * 2,
	                  r.bottom - buttonRun->Bounds().Height() - GAP);
	buttonAbout->MoveTo(r.left + BANNER_W + GAP, r.bottom - buttonAbout->Bounds().Height() - GAP);
	fMainView->AddChild(buttonExit);
	fMainView->AddChild(buttonRun);
	fMainView->AddChild(buttonAbout);
	SetDefaultButton(buttonRun);

	AddChild(fMainView);
	AddChild(bannerView);

	fDirectotyFilter = new BeDirectoryFilter();
	entry_ref start_point;
	BEntry entry(BeUtils::GetPathToHomeDir().String());
	entry.GetRef(&start_point);
	fFilePanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(this), &start_point,
	                            B_DIRECTORY_NODE, false, new BMessage(MSG_FILE_PANEL_FILE_SELECTED),
	                            fDirectotyFilter, true);
	fFilePanel->Window()->SetTitle(sFilePanelTitle);

	SetSizeLimits(400.0f, 800.0f, 300.0f, 600.0f);
}

void
BeLauncherBase::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case MSG_BUTTON_RUN_CLICKED:
		{
			SaveSettings(false);
			RunGameViaRoaster();
			BeMainWindow::QuitRequested();
			break;
		}
		case MSG_BUTTON_EXIT_CLICKED:
		{
			BeMainWindow::QuitRequested();
			break;
		}
		case MSG_BUTTON_ABOUT_CLICKED:
		{
			ShowAboutDialog();
			break;
		}
		case MSG_BUTTON_BROWSE_CLICKED:
		{
			SelectDirectory();
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

BView *
BeLauncherBase::GetMainView() const
{
	return fMainView;
}

BeSettings *
BeLauncherBase::GetSettings() const
{
	return fSettings;
}

float
BeLauncherBase::Gap()
{
	return GAP;
}

float
BeLauncherBase::BannerWidth()
{
	return BANNER_W;
}

void
BeLauncherBase::SelectDirectory()
{
	fFilePanel->Show();
}

void
BeLauncherBase::DirectorySelected()
{
	entry_ref dirRef;
	fFilePanel->GetPanelDirectory(&dirRef);
	BEntry entry(&dirRef);
	BPath path;
	entry.GetPath(&path);

	fDataTextControl->SetText(path.Path());
}

void
BeLauncherBase::ReadSettings()
{
	if(!fSettings->ReadSettingsFromFile())
	{
		SaveSettings(true);
	}

	fDataTextControl->SetText(fSettings->GetString(sDataPath));
}

void
BeLauncherBase::SaveSettings(bool def)
{
	fSettings->SetString(sDataPath, (def) ? BeUtils::GetPathToHomeDir().String() :
	                                        fDataTextControl->Text());

	fSettings->DumpSettingsToFile();
}

void
BeLauncherBase::ShowAboutDialog()
{
	BeDebug(__func__);
}

void
BeLauncherBase::RunGameViaRoaster()
{
	BeDebug(__func__);
}

void
BeLauncherBase::RunGameViaExecVe()
{
	BeDebug(__func__);
}
