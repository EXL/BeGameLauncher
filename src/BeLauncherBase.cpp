#include "BeLauncherBase.h"
#include "BeAboutWindow.h"
#include "BeImageView.h"
#include "BeUtils.h"

#include <Rect.h>
#include <InterfaceDefs.h>
#include <Button.h>
#include <Message.h>
#include <Messenger.h>
#include <TextControl.h>
#include <Entry.h>
#include <Directory.h>
#include <Path.h>
#include <Roster.h>
#include <Alert.h>

#include <unistd.h>
#include <posix/stdlib.h>
#include <compat/sys/stat.h>

#define G_GAP                      10.0f
#define G_STATUS_GAP_HACK          1.0f
#define G_BANNER_W                 64.0f

#define L_BTN_RUN                  "Run!"
#define L_BTN_EXIT                 "Exit"
#define L_BTN_ABOUT                "About..."
#define L_BTN_BROWSE               "..."
#define L_BTN_ALERT_OK             "OK"
#define L_READY                    "Ready."
#define L_ALERT_EXECUTABLE_ERROR_H "Executable Error"
#define L_ALERT_EXECUTABLE_ERROR   "Cannot run executable: "
#define L_ALERT_CACHE_ERROR_H      "Cache Error"
#define L_ALERT_CACHE_ERROR        "Game data check failed."
#define L_ALERT_WRITE_S_WARNING_H  "Settings Error"
#define L_ALERT_WRITE_S_WARNING    "Cannot write settings file: "

#define O_MAIN_VIEW                "mainView"
#define O_BANNER_VIEW              "bannerView"
#define O_STATUS_VIEW              "statusView"
#define O_STATUS_STRING            "statusString"
#define O_DATA_SVIEW               "dataStringView"
#define O_DATA_TCONTROL            "dataTextControl"
#define O_BTN_BROWSE               "buttonBrowse"
#define O_BTN_ABOUT                "buttonAbout"
#define O_BTN_RUN                  "buttonRun"
#define O_BTN_EXIT                 "buttonExit"

extern char **environ;

BeLauncherBase::BeLauncherBase(const char *windowTitle, const char *packageName,
                               const char *executableFileName, const char *settingsFileName,
                               const char *dataPath, bool useExecVe)
	: BeMainWindow(BRect(100, 100, 700, 500), windowTitle), sPackageName(packageName),
	  sExecutableFileName(executableFileName), sSettingsFileName(settingsFileName),
	  sDataPath(dataPath), sUseExecVe(useExecVe)
{
	sWindowTitle = windowTitle;
}

void
BeLauncherBase::InitParameters(const char *stringViewData, const char *textControlToolTip,
                               const char *buttonBrowseToolTip, const char* filePanelTitle)
{
	sStringViewData = stringViewData;
	sTextControlToolTip = textControlToolTip;
	sButtonBrowseToolTip = buttonBrowseToolTip;
	sFilePanelTitle = filePanelTitle;

	fExecutableFilePath = BeUtils::GetPathToExecutable(sPackageName, sExecutableFileName);

	fSettings = new BeSettings(sSettingsFileName);
}

void
BeLauncherBase::CreateForm()
{
	BRect r(Bounds());
	BRect bannerRect(r.left, r.top, G_BANNER_W, r.bottom);
	BRect stringViewRect(G_BANNER_W + G_GAP, G_BANNER_W + G_GAP, r.right, r.bottom);

	fMainView = new BView(r, O_MAIN_VIEW, B_FOLLOW_ALL, B_WILL_DRAW);
	fMainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BeImageView *bannerView	= new BeImageView(bannerRect, O_BANNER_VIEW, K_BANNER, B_FOLLOW_TOP_BOTTOM);
	//BView *bannerView = new BView(bannerRect, O_BANNER_VIEW, B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW);
	//bannerView->SetViewColor(255, 205, 8);

	BStringView *dataStringView = new BStringView(stringViewRect, O_DATA_SVIEW, sStringViewData, B_FOLLOW_LEFT);
	dataStringView->ResizeToPreferred();
	fMainView->AddChild(dataStringView);

	BButton *buttonBrowse = new BButton(BRect(), O_BTN_BROWSE, L_BTN_BROWSE,
	                                    new BMessage(MSG_BUTTON_BROWSE_CLICKED), B_FOLLOW_RIGHT);
	buttonBrowse->ResizeToPreferred();
	buttonBrowse->ResizeTo(buttonBrowse->Bounds().Width() - G_GAP * 5, buttonBrowse->Bounds().Height());
	buttonBrowse->MoveTo(r.right - buttonBrowse->Bounds().Width() - G_GAP, G_BANNER_W + G_GAP * 3);
	buttonBrowse->SetToolTip(sButtonBrowseToolTip);
	fMainView->AddChild(buttonBrowse);

	BRect dataTextControlRect(G_BANNER_W + G_GAP, G_BANNER_W + G_GAP * 3, r.right - G_GAP*4, r.bottom);
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
	                                  new BMessage(B_QUIT_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	buttonRun->ResizeToPreferred();
	buttonExit->ResizeToPreferred();
	buttonAbout->ResizeToPreferred();
	buttonExit->MoveTo(r.right - buttonExit->Bounds().Width() - G_GAP,
	                   r.bottom - buttonExit->Bounds().Height() - G_GAP);
	buttonRun->MoveTo(r.right - buttonExit->Bounds().Width() - buttonRun->Bounds().Width() - G_GAP * 2,
	                  r.bottom - buttonRun->Bounds().Height() - G_GAP);
	buttonAbout->MoveTo(r.left + G_BANNER_W + G_GAP, r.bottom - buttonAbout->Bounds().Height() - G_GAP);
	fMainView->AddChild(buttonExit);
	fMainView->AddChild(buttonRun);
	fMainView->AddChild(buttonAbout);
	SetDefaultButton(buttonRun);

	BRect statusRect(G_BANNER_W + G_STATUS_GAP_HACK, r.bottom - buttonAbout->Bounds().Height() - G_GAP * 3,
	                 r.right, r.bottom);
	BView *statusView = new BView(statusRect, O_STATUS_VIEW, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	fStatusString = new BStringView(BRect(), O_STATUS_STRING, L_READY, B_FOLLOW_LEFT);
	statusView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fStatusString->SetFontSize(10.0f);
	fStatusString->ResizeToPreferred();
	fStatusString->MoveTo(G_GAP, 0.0f);
	statusView->AddChild(fStatusString);

	AddChild(fMainView);
	AddChild(bannerView);
	AddChild(statusView);

	fDirectotyFilter = new BeDirectoryFilter();
	entry_ref start_point;
	BEntry entry(SetDefaultDir());
	entry.GetRef(&start_point);
	fDirectoryFilePanel = new BeDirectoryFilePanel(B_OPEN_PANEL, new BMessenger(this), &start_point,
	                            B_DIRECTORY_NODE, false, new BMessage(MSG_FILE_PANEL_FILE_SELECTED),
	                            fDirectotyFilter, true);
	fDirectoryFilePanel->Window()->SetTitle(sFilePanelTitle);

	SetSizeLimits(Bounds().Width() - 200.0f, Bounds().Width() + 200.0f,
	              Bounds().Height() - 100.0f, Bounds().Height() + 200.0f);
}

void
BeLauncherBase::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case MSG_BUTTON_RUN_CLICKED:
		{
			bool result = false;
			if(sUseExecVe)
			{
				SetStatusString(B_COLOR_BLUE, "Running game via execve...\n");
				result = RunGameViaExecVe();
			}
			else
			{
				SetStatusString(B_COLOR_BLUE, "Running game via BRoster...\n");
				result = RunGameViaRoster();
			}
			if(result)
			{
				QuitRequestedSub();
			}
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

void
BeLauncherBase::SetStatusString(color_msg_t type, const BString &str)
{
	switch(type)
	{
		case B_COLOR_RED:
		{
			fStatusString->SetHighColor(K_RED);
			break;
		}
		case B_COLOR_GREEN:
		{
			fStatusString->SetHighColor(K_GREEN);
			break;
		}
		case B_COLOR_BLUE:
		{
			fStatusString->SetHighColor(K_BLUE);
			break;
		}
		default:
		{
			fStatusString->SetHighColor(K_BLACK);
			break;
		}

	}

	fStatusString->SetText(str.String());
	fStatusString->ResizeToPreferred();
}

bool
BeLauncherBase::CheckCache()
{
	return true;
}

bool
BeLauncherBase::CheckExecutable()
{
	const char *executable = fExecutableFilePath.String();
	entry_ref ref;
	if(get_ref_for_path(executable, &ref) != B_OK)
	{
		SetStatusString(B_COLOR_RED, BString("Error: Function get_ref_for_path for ")
		                << BString(executable) << BString(" failed."));
		return false;
	}

	BEntry entry(&ref);
	if(!entry.Exists() || !entry.IsFile())
	{
		SetStatusString(B_COLOR_RED, BString("Error: Path entry ")
		                << BString(executable) << BString(" is not exist nor file."));
		return false;
	}

	mode_t permissions;
	if(entry.GetPermissions(&permissions) != B_OK)
	{
		SetStatusString(B_COLOR_RED, BString("Error: Cannot get entry ")
		                << BString(executable) << BString(" permissons."));
		return false;
	}

	if(!(permissions & S_IXUSR))
	{
		SetStatusString(B_COLOR_RED, BString("Error: File ")
		                << BString(executable) << BString(" does not have permission to execute"));
		return false;
	}

	return true;
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

BTextControl *
BeLauncherBase::GetTextControl() const
{
	return fDataTextControl;
}

float
BeLauncherBase::Gap()
{
	return G_GAP;
}

float
BeLauncherBase::BannerWidth()
{
	return G_BANNER_W;
}

float
BeLauncherBase::StatusGapHack()
{
	return G_STATUS_GAP_HACK;
}

void
BeLauncherBase::ShowErrorCacheAlert()
{
	BAlert *cacheErrorAlert = new BAlert(L_ALERT_CACHE_ERROR_H, L_ALERT_CACHE_ERROR, L_BTN_ALERT_OK);
	cacheErrorAlert->SetType(B_STOP_ALERT);
	cacheErrorAlert->SetFlags(cacheErrorAlert->Flags() | B_CLOSE_ON_ESCAPE);
	int32 button_index = cacheErrorAlert->Go();
	if(button_index == 0)
	{
		SetStatusString(B_COLOR_BLACK, L_READY);
	}
}

void
BeLauncherBase::ShowWarnWriteSettingsAlert()
{
	BString settingsWarning(L_ALERT_WRITE_S_WARNING);
	settingsWarning << BeUtils::GetPathToSettingsFile(sSettingsFileName);
	BAlert *writeSettingsWarnAlert = new BAlert(L_ALERT_WRITE_S_WARNING_H, settingsWarning, L_BTN_ALERT_OK);
	writeSettingsWarnAlert->SetType(B_WARNING_ALERT);
	writeSettingsWarnAlert->SetFlags(writeSettingsWarnAlert->Flags() | B_CLOSE_ON_ESCAPE);
	int32 button_index = writeSettingsWarnAlert->Go();
	if(button_index == 0)
	{
		SetStatusString(B_COLOR_BLACK, L_READY);
	}
}

void
BeLauncherBase::ShowExecutableCacheAlert()
{
	BString executableError(L_ALERT_EXECUTABLE_ERROR);
	executableError << fExecutableFilePath;
	BAlert *executableErrorAlert = new BAlert(L_ALERT_EXECUTABLE_ERROR_H, executableError, L_BTN_ALERT_OK);
	executableErrorAlert->SetType(B_STOP_ALERT);
	executableErrorAlert->SetFlags(executableErrorAlert->Flags() | B_CLOSE_ON_ESCAPE);
	int32 button_index = executableErrorAlert->Go();
	if(button_index == 0)
	{
		SetStatusString(B_COLOR_BLACK, L_READY);
	}
}

void
BeLauncherBase::SelectDirectory()
{
	fDirectoryFilePanel->Show();
}

void
BeLauncherBase::DirectorySelected()
{
	entry_ref dirRef;
	fDirectoryFilePanel->GetPanelDirectory(&dirRef);
	BEntry entry(&dirRef);
	BPath path;
	entry.GetPath(&path);

	fDataTextControl->SetText(path.Path());
}

bool
BeLauncherBase::ReadSettings()
{
	bool isNotFirstBlood = true;
	if(!fSettings->ReadSettingsFromFile())
	{
		isNotFirstBlood = false;
		SaveSettings(true);
	}

	fDataTextControl->SetText(fSettings->GetString(sDataPath));
	return isNotFirstBlood;
}

void
BeLauncherBase::SaveSettings(bool def)
{
	fSettings->SetString(sDataPath, (def) ? SetDefaultDir() :
	                                        fDataTextControl->Text());

	SetStatusString(B_COLOR_BLUE, BString("Saving settings to ")
	                << BString(BeUtils::GetPathToSettingsFile(sSettingsFileName)) << BString(" file..."));

	if(!fSettings->DumpSettingsToFile())
	{
		ShowWarnWriteSettingsAlert();
	}

	SetStatusString(B_COLOR_BLACK, L_READY);
}

void
BeLauncherBase::ShowAboutDialog()
{
	BeAboutWindow *aboutWindow = new BeAboutWindow(Frame().InsetBySelf(G_BANNER_W, -(G_GAP * 3)), sWindowTitle);
	aboutWindow->Show();
}

bool
BeLauncherBase::RunGameViaRoster()
{
	if(!CheckExecutable())
	{
		ShowExecutableCacheAlert();
		return false;
	}
	if(!CheckCache())
	{
		ShowErrorCacheAlert();
		return false;
	}

	entry_ref ref;

	setenv(sDataPath, fDataTextControl->Text(), 1);

	const char *executable = fExecutableFilePath.String();

	if(get_ref_for_path(executable, &ref) != B_OK)
	{
		SetStatusString(B_COLOR_RED, BString("Cannot run ") << BString(executable)
		                << BString(" executable. See ") << BString(__func__));
		return false;
	}
	else
	{
		const char *argv[] = { executable, NULL };
		be_roster->Launch(&ref, 1, argv);
	}

	return true;
}

bool
BeLauncherBase::RunGameViaExecVe()
{
	if(!CheckExecutable())
	{
		ShowExecutableCacheAlert();
		return false;
	}
	if(!CheckCache())
	{
		ShowErrorCacheAlert();
		return false;
	}

	setenv(sDataPath, fDataTextControl->Text(), 1);

	if (!fork())
	{
		const char *executable = fExecutableFilePath.String();
		const char *argv[] = { executable, NULL };
		execve(executable, const_cast<char * const *>(argv), environ);
		SetStatusString(B_COLOR_RED, BString("Cannot run ") << BString(executable)
		                << BString(" executable. See ") << BString(__func__));
		return false;
	}

	return true;
}

const char *
BeLauncherBase::SetDefaultDir()
{
	return BeUtils::GetPathToUserNonPackedDataDir().String();
}

bool
BeLauncherBase::QuitRequested()
{
	BeDebug("AAAAAAAAAA\n");
	SaveSettings(false);
	BeMainWindow::QuitRequested();
	return true;
}

bool
BeLauncherBase::QuitRequestedSub()
{
	BeMainWindow::QuitRequested();
	return true;
}
