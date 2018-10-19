#include "BeLauncherBase.h"
#include "BeLauncherView.h"
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
#include <GroupLayout.h>

#include <Catalog.h>

#include <unistd.h>
#include <posix/stdlib.h>
#include <compat/sys/stat.h>

#define G_GAP                          10.0f
#define G_STATUS_GAP_HACK              1.0f
#define G_BANNER_W                     64.0f

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "BeGameLauncher"

#define L_BTN_ALERT_OK                 B_TRANSLATE("OK")
#define L_READY                        B_TRANSLATE("Ready.") // ?
#define L_ALERT_EXECUTABLE_ERROR_H     B_TRANSLATE("Executable Error")
#define L_ALERT_EXECUTABLE_ERROR       B_TRANSLATE("Cannot run executable: ")
#define L_ALERT_CACHE_ERROR_H          B_TRANSLATE("Cache Error")
#define L_ALERT_CACHE_ERROR            B_TRANSLATE("Game data check failed.")
#define L_ALERT_WRITE_S_WARNING_H      B_TRANSLATE("Settings Error")
#define L_ALERT_WRITE_S_WARNING        B_TRANSLATE("Cannot write settings file: ")
#define L_RUNNING_VIA_EXECVE           B_TRANSLATE("Running game via execve...")
#define L_RUNNING_VIA_ROSTER           B_TRANSLATE("Running game via BRoster...")
#define L_ERROR_ENTRY_FUNC             B_TRANSLATE("Error: Function get_ref_for_path() for %exe% failed.")
#define L_ERROR_ENTRY_PATH             B_TRANSLATE("Error: Path entry %exe% is not exist nor file.")
#define L_ERROR_ENTRY_PERMISSIONS      B_TRANSLATE("Error: Cannot get entry %exe% permissons.")
#define L_ERROR_ENTRY_PERMISSIONS_EXEC B_TRANSLATE("Error: File %exe% does not have permission to execute.")
#define L_ERROR_RUN_EXE                B_TRANSLATE("Error: Cannot run %exe% executable. See %func%.")
#define L_SAVING_SETTINGS              B_TRANSLATE("Saving settings to the %file% file...")
#define L_FILE_PANEL_TITLE             B_TRANSLATE("Please choose a game folder")

extern char **environ;

BeLauncherBase::BeLauncherBase(const char *windowTitle, const char *packageName,
                               const char *executableFileName, const char *settingsFileName,
                               const char *dataPath, const char *startPath,
                               bool showIcon, bool readSettings, bool useExecVe)
	: BeMainWindow(BRect(100.0f, 100.0f, 700.0f, 500.0f), windowTitle), sSettingsFileName(settingsFileName),
	  sDataPath(dataPath), sStartPath(startPath), sShowIcon(showIcon), sUseExecVe(useExecVe)
{
	sWindowTitle = windowTitle;
	fExecutableFilePath = BeUtils::GetPathToExecutable(packageName, executableFileName);
	fSettings = new BeSettings(sSettingsFileName);
	CreateForm();
	if(readSettings)
	{
		ReadSettings();
	}
}

void
BeLauncherBase::CreateForm()
{
	SetLayout(new BGroupLayout(B_VERTICAL));

	fLauncherView = new BeLauncherView(sShowIcon);

	fStatusString = fLauncherView->GetStatusStringView();
	fDataTextControl = fLauncherView->GetTextControl();
	fAdditionalBox = fLauncherView->GetAdditionalBox();

	fLauncherView->GetBrowseButton()->SetMessage(new BMessage(MSG_BUTTON_BROWSE_CLICKED));
	fLauncherView->GetAboutButton()->SetMessage(new BMessage(MSG_BUTTON_ABOUT_CLICKED));
	fLauncherView->GetRunButton()->SetMessage(new BMessage(MSG_BUTTON_RUN_CLICKED));
	fLauncherView->GetExitButton()->SetMessage(new BMessage(B_QUIT_REQUESTED));

	SetDefaultButton(fLauncherView->GetRunButton());
	AddChild(fLauncherView);

	fDirectotyFilter = new BeDirectoryFilter();
	entry_ref start_point;
	BEntry entry(sStartPath);
	entry.GetRef(&start_point);
	fDirectoryFilePanel = new BeDirectoryFilePanel(B_OPEN_PANEL, new BMessenger(this), &start_point,
	                            B_DIRECTORY_NODE, false, new BMessage(MSG_FILE_PANEL_FILE_SELECTED),
	                            fDirectotyFilter, true);
	fDirectoryFilePanel->Window()->SetTitle(L_FILE_PANEL_TITLE);

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
				SetStatusString(B_COLOR_BLUE, L_RUNNING_VIA_EXECVE);
				result = RunGameViaExecVe();
			}
			else
			{
				SetStatusString(B_COLOR_BLUE, L_RUNNING_VIA_ROSTER);
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
		BString error(L_ERROR_ENTRY_FUNC);
		error.ReplaceAll("%exe%", executable);
		SetStatusString(B_COLOR_RED, error);
		return false;
	}

	BEntry entry(&ref);
	if(!entry.Exists() || !entry.IsFile())
	{
		BString error(L_ERROR_ENTRY_PATH);
		error.ReplaceAll("%exe%", executable);
		SetStatusString(B_COLOR_RED, error);
		return false;
	}

	mode_t permissions;
	if(entry.GetPermissions(&permissions) != B_OK)
	{
		BString error(L_ERROR_ENTRY_PERMISSIONS);
		error.ReplaceAll("%exe%", executable);
		SetStatusString(B_COLOR_RED, error);
		return false;
	}

	if(!(permissions & S_IXUSR))
	{
		BString error(L_ERROR_ENTRY_PERMISSIONS_EXEC);
		error.ReplaceAll("%exe%", executable);
		SetStatusString(B_COLOR_RED, error);
		return false;
	}

	return true;
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

BBox *
BeLauncherBase::GetAdditionalBox(void) const
{
	return fAdditionalBox;
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
	fSettings->SetString(sDataPath, (def) ? sStartPath :
	                                        fDataTextControl->Text());

	BString message(L_SAVING_SETTINGS);
	message.ReplaceAll("%file%", BString(BeUtils::GetPathToSettingsFile(sSettingsFileName)));
	SetStatusString(B_COLOR_BLUE, message);

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
		BString error(L_ERROR_RUN_EXE);
		error.ReplaceAll("%exe%", executable).ReplaceAll("%func%", __func__);
		SetStatusString(B_COLOR_RED, error);
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

		BString error(L_ERROR_RUN_EXE);
		error.ReplaceAll("%exe%", executable).ReplaceAll("%func%", __func__);
		SetStatusString(B_COLOR_RED, error);
		return false;
	}

	return true;
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

BeLauncherBase::~BeLauncherBase()
{
	BeDebug("Freeing...\n");
	delete fDirectotyFilter;
	fDirectotyFilter = NULL;

	delete fDirectoryFilePanel;
	fDirectoryFilePanel = NULL;

	fLauncherView->RemoveSelf();
	delete fLauncherView;
	fLauncherView = NULL;

	delete fSettings;
	fSettings = NULL;
}
