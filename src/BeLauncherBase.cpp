#include "BeLauncherBase.h"
#include "BeLauncherView.h"
#include "BeAboutWindow.h"
#include "BeImageView.h"
#include "BeUtils.h"

#include <Rect.h>
#include <GroupLayout.h>
#include <InterfaceDefs.h>
#include <Entry.h>
#include <Messenger.h>
#include <Errors.h>
#include <Alert.h>
#include <SupportDefs.h>
#include <Path.h>
#include <Roster.h>
#include <Window.h>
#include <Errors.h>

#include <Catalog.h>

#include <vector>

#include <unistd.h>

#include <posix/stdlib.h>
#include <posix/sys/types.h>

#include <compat/sys/stat.h>

#define G_START_POINT_X                100.0f
#define G_START_POINT_Y                100.0f
#define G_WINDOW_WIDTH                 (600.0f + G_START_POINT_X)
#define G_WINDOW_HEIGHT                (400.0f + G_START_POINT_Y)
#define G_OFFSET_FOR_SIZE              200.0f

#undef  B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT          "BeGameLauncher"

#define L_BTN_ALERT_OK                 B_TRANSLATE("OK")
#define L_ALERT_EXECUTABLE_ERROR_H     B_TRANSLATE("Executable Error")
#define L_ALERT_EXECUTABLE_ERROR       B_TRANSLATE("Cannot run executable: ")
#define L_ALERT_CACHE_ERROR_H          B_TRANSLATE("Cache Error")
#define L_ALERT_CACHE_ERROR            B_TRANSLATE("Game data check failed.")
#define L_ALERT_WRITE_S_WARNING_H      B_TRANSLATE("Settings Error")
#define L_ALERT_WRITE_S_WARNING        B_TRANSLATE("Cannot write settings file: ")
#define L_RUNNING_VIA_EXECVE           B_TRANSLATE("Running game via execve. Environ: %env%...")
#define L_RUNNING_VIA_ROSTER           B_TRANSLATE("Running game via BRoster. Environ: %env%...")
#define L_ERROR_ENTRY_FUNC             B_TRANSLATE("Error: Function get_ref_for_path() for %exe% failed.")
#define L_ERROR_ENTRY_PATH             B_TRANSLATE("Error: Path entry %exe% is not exist nor file.")
#define L_ERROR_ENTRY_PERMISSIONS      B_TRANSLATE("Error: Cannot get entry %exe% permissions.")
#define L_ERROR_ENTRY_PERMISSIONS_EXEC B_TRANSLATE("Error: File %exe% does not have permission to execute.")
#define L_ERROR_RUN_EXE                B_TRANSLATE("Error: Cannot run %exe% executable. See %func%.")
#define L_SAVING_SETTINGS              B_TRANSLATE("Saving settings to the %file% file...")
#define L_FILE_PANEL_TITLE             B_TRANSLATE("Please choose a game folder")

extern char **environ;

BeLauncherBase::BeLauncherBase(const char *windowTitle,
                               const char *packageName,
                               const char *executableFileName,
                               const char *settingsFileName,
                               const char *dataPathArg,
                               const char *startPath,
                               bool showIcon,
                               bool readSettings)
                : BeMainWindow(BRect(G_START_POINT_X, G_START_POINT_Y, G_WINDOW_WIDTH, G_WINDOW_HEIGHT), windowTitle),
                               sSettingsFileName(settingsFileName), sDataPathArg(dataPathArg), sStartPath(startPath),
                               sShowIcon(showIcon)
{
	sWindowTitle = windowTitle;
	fExecutableFilePath = BeUtils::GetPathToExecutable(packageName, executableFileName);

	fSettings = new BeSettings(sSettingsFileName);
	fDirectoryFilter = NULL;
	fDirectoryFilePanel = NULL;
	fLauncherView = NULL;
	fStatusString = NULL;
	fDataTextControl = NULL;
	fAdditionalBox = NULL;

	CreateForm();

	if(readSettings)
	{
		ReadSettings();
	}
}

void
BeLauncherBase::CreateForm(void)
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

	fDirectoryFilter = new BeDirectoryFilter();
	fDirectoryFilePanel = new BeDirectoryFilePanel(new BMessenger(this),
	                                               new BMessage(MSG_FILE_PANEL_FILE_SELECTED),
	                                               fDirectoryFilter, sStartPath);
	fDirectoryFilePanel->Window()->SetTitle(L_FILE_PANEL_TITLE);

	BSize laySize = GetLayout()->MinSize();
	SetSizeLimits(laySize.Width()   + G_OFFSET_FOR_SIZE,
	              Bounds().Width()  + G_OFFSET_FOR_SIZE,
	              laySize.Height()  + G_OFFSET_FOR_SIZE,
	              Bounds().Height() + G_OFFSET_FOR_SIZE);
}

void
BeLauncherBase::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case MSG_BUTTON_RUN_CLICKED:
		{
			if(RunGame())
			{
				PostMessage(B_QUIT_REQUESTED);
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
			DirectorySelected(fDirectoryFilePanel, fDataTextControl);
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
BeLauncherBase::CheckCache(void)
{
	return true;
}

bool
BeLauncherBase::CheckExecutable(void)
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

	if((permissions & S_IXUSR) == 0)
	{
		BString error(L_ERROR_ENTRY_PERMISSIONS_EXEC);
		error.ReplaceAll("%exe%", executable);
		SetStatusString(B_COLOR_RED, error);
		return false;
	}

	return true;
}

bool
BeLauncherBase::CheckAll()
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

	return true;
}

bool
BeLauncherBase::RunGame(void)
{
	return RunGameViaRoster(false);
}

BeDirectoryFilePanel *
BeLauncherBase::GetDirectoryFilePanel(void) const
{
	return fDirectoryFilePanel;
}

BeSettings *
BeLauncherBase::GetSettings(void) const
{
	return fSettings;
}

BTextControl *
BeLauncherBase::GetTextControl(void) const
{
	return fDataTextControl;
}

BBox *
BeLauncherBase::GetAdditionalBox(void) const
{
	return fAdditionalBox;
}

float
BeLauncherBase::Gap(void)
{
	return G_DEFAULT_GAP;
}

float
BeLauncherBase::BannerWidth(void)
{
	return BeImageView::GetGeneralWidth();
}

float
BeLauncherBase::GetStartPointX(void)
{
	return G_START_POINT_X;
}

float
BeLauncherBase::GetStartPointY(void)
{
	return G_START_POINT_Y;
}

float
BeLauncherBase::GetWindowWidth(void)
{
	return G_WINDOW_WIDTH;
}

float
BeLauncherBase::GetWindowHeight(void)
{
	return G_WINDOW_HEIGHT;
}

float
BeLauncherBase::GetOffsetForSize(void)
{
	return G_OFFSET_FOR_SIZE;
}

void
BeLauncherBase::ShowErrorCacheAlert(void)
{
	BAlert *cacheErrorAlert = new BAlert(L_ALERT_CACHE_ERROR_H, L_ALERT_CACHE_ERROR, L_BTN_ALERT_OK);
	cacheErrorAlert->SetType(B_STOP_ALERT);
	cacheErrorAlert->SetFlags(cacheErrorAlert->Flags() | B_CLOSE_ON_ESCAPE);
	const int32 button_index = cacheErrorAlert->Go();
	if(button_index == 0)
	{
		SetStatusString(B_COLOR_BLACK, BeLauncherView::GetReadyString());
	}
}

void
BeLauncherBase::ShowWarnWriteSettingsAlert(void)
{
	BString settingsWarning(L_ALERT_WRITE_S_WARNING);
	settingsWarning << BeUtils::GetPathToSettingsFile(sSettingsFileName);
	BAlert *writeSettingsWarnAlert = new BAlert(L_ALERT_WRITE_S_WARNING_H, settingsWarning, L_BTN_ALERT_OK);
	writeSettingsWarnAlert->SetType(B_WARNING_ALERT);
	writeSettingsWarnAlert->SetFlags(writeSettingsWarnAlert->Flags() | B_CLOSE_ON_ESCAPE);
	const int32 button_index = writeSettingsWarnAlert->Go();
	if(button_index == 0)
	{
		SetStatusString(B_COLOR_BLACK, BeLauncherView::GetReadyString());
	}
}

void
BeLauncherBase::ShowExecutableCacheAlert(void)
{
	BString executableError(L_ALERT_EXECUTABLE_ERROR);
	executableError << fExecutableFilePath;
	BAlert *executableErrorAlert = new BAlert(L_ALERT_EXECUTABLE_ERROR_H, executableError, L_BTN_ALERT_OK);
	executableErrorAlert->SetType(B_STOP_ALERT);
	executableErrorAlert->SetFlags(executableErrorAlert->Flags() | B_CLOSE_ON_ESCAPE);
	const int32 button_index = executableErrorAlert->Go();
	if(button_index == 0)
	{
		SetStatusString(B_COLOR_BLACK, BeLauncherView::GetReadyString());
	}
}

void
BeLauncherBase::SelectDirectory(void)
{
	fDirectoryFilePanel->Show();
}

void
BeLauncherBase::DirectorySelected(const BeDirectoryFilePanel *filePanel,
                                  BTextControl *textControl)
{
	entry_ref dirRef;
	filePanel->GetPanelDirectory(&dirRef);
	BEntry entry(&dirRef);
	BPath path;
	entry.GetPath(&path);

	textControl->SetText(path.Path());
}

bool
BeLauncherBase::ReadSettings(void)
{
	bool isNotFirstBlood = true;
	if(!fSettings->ReadSettingsFromFile())
	{
		isNotFirstBlood = false;
		SaveSettings(true);
	}

	fDataTextControl->SetText(fSettings->GetSettingsString(sDataPathArg));
	return isNotFirstBlood;
}

void
BeLauncherBase::SaveSettings(bool def)
{
	BeDebug("[Info]: Saving Settings... ");
	fSettings->SetSettingsString(sDataPathArg, (def) ? sStartPath :
	                                        fDataTextControl->Text());

	BString message(L_SAVING_SETTINGS);
	message.ReplaceAll("%file%", BString(BeUtils::GetPathToSettingsFile(sSettingsFileName)));
	SetStatusString(B_COLOR_BLUE, message);

	if(!fSettings->DumpSettingsToFile())
	{
		ShowWarnWriteSettingsAlert();
	}

	SetStatusString(B_COLOR_BLACK, BeLauncherView::GetReadyString());
	BeDebug("done!\n");
}

void
BeLauncherBase::ShowAboutDialog(void)
{
	BeAboutWindow *aboutWindow = new BeAboutWindow(Frame().InsetBySelf(BannerWidth(), -(Gap() * 3)), sWindowTitle);
	aboutWindow->Show();
}

bool
BeLauncherBase::RunGameViaRoster(bool useEnviron)
{
	BString message(L_RUNNING_VIA_ROSTER);
	message.ReplaceAll("%env%", BString() << useEnviron);
	SetStatusString(B_COLOR_BLUE, message);

	if(!CheckAll())
	{
		return false;
	}

	entry_ref ref;

	if(useEnviron)
	{
		setenv(sDataPathArg, fDataTextControl->Text(), 1);
	}

	const char *executable = fExecutableFilePath.String();

	if(get_ref_for_path(executable, &ref) != B_OK)
	{
		BString error(L_ERROR_RUN_EXE);
		error.ReplaceAll("%exe%", executable).ReplaceAll("%func%", __func__);
		SetStatusString(B_COLOR_RED, error);
		return false;
	}

	std::vector<const char *> argv;
	argv.push_back(executable);
	if (!useEnviron)
	{
		argv.push_back(sDataPathArg);
		argv.push_back(fDataTextControl->Text());
	}
	argv.push_back(NULL);
	be_roster->Launch(&ref, static_cast<int>(argv.size() - 1),
	                  const_cast<const char * const *>(argv.data()));

	return true;
}

bool
BeLauncherBase::RunGameViaExecVe(bool useEnviron)
{
	BString message(L_RUNNING_VIA_EXECVE);
	message.ReplaceAll("%env%", BString() << useEnviron);
	SetStatusString(B_COLOR_BLUE, message);

	if(!CheckAll())
	{
		return false;
	}

	if(useEnviron)
	{
		setenv(sDataPathArg, fDataTextControl->Text(), 1);
	}

	if (fork() == 0)
	{
		const char *executable = fExecutableFilePath.String();

		std::vector<const char *> argv;
		argv.push_back(executable);
		if(!useEnviron)
		{
			argv.push_back(sDataPathArg);
			argv.push_back(fDataTextControl->Text());
		}
		argv.push_back(NULL);
		execve(executable, const_cast<char * const *>(argv.data()), environ);

		BString error(L_ERROR_RUN_EXE);
		error.ReplaceAll("%exe%", executable).ReplaceAll("%func%", __func__);
		SetStatusString(B_COLOR_RED, error);
		return false;
	}

	return true;
}

bool
BeLauncherBase::QuitRequested(void)
{
	SaveSettings(false);
	BeMainWindow::QuitRequested();
	return true;
}

BeLauncherBase::~BeLauncherBase(void)
{
	BeDebug("[Info]: Freeing... ");
	delete fDirectoryFilter;
	fDirectoryFilter = NULL;

	delete fDirectoryFilePanel;
	fDirectoryFilePanel = NULL;

	fLauncherView->RemoveSelf();
	delete fLauncherView;
	fLauncherView = NULL;

	delete fSettings;
	fSettings = NULL;
	BeDebug("done!\n");
}
