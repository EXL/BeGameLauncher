#include "BeApp.h"
#include "BeLauncherBase.h"

#include <View.h>
#include <StringView.h>
#include <CheckBox.h>

#define SIGNATURE            "application/x-vnd.exl-BasedGameLauncher"
#define TITLE                "Game Launcher"
#define PACKAGE_DIR          "Game"
#define SETTINGS_FILE        "GameLauncher.set"
#define EXECUTABLE_FILE      "GameExe"

class BasedGameLauncher : public BeLauncherBase 
{
public:
	BasedGameLauncher(void) : BeLauncherBase(TITLE, PACKAGE_DIR, EXECUTABLE_FILE, SETTINGS_FILE)
	{
		BeLauncherBase::InitParameters();
		CreateForm();
		BeLauncherBase::ReadSettings();
	}

	void CreateForm()
	{
		BeLauncherBase::CreateForm();

		BView *ui = BeLauncherBase::GetMainView();

		BStringView *strView = new BStringView(BRect(), "testString", "Test");
		strView->ResizeToPreferred();
		strView->MoveTo(200, 200);

		ui->AddChild(strView);
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
