#include "BeApp.h"
#include "BeMainWindow.h"

#include <Rect.h>
#include <View.h>
#include <InterfaceDefs.h>
#include <Button.h>
#include <Message.h>
#include <StringView.h>
#include <TextControl.h>

// For Debug
#include <stdio.h>

#define SIGNATURE  "application/x-vnd.exl-BasedGameLauncher"

#define L_APP_NAME           "Game Launcher"
#define L_BTN_RUN            "Run!"
#define L_BTN_EXIT           "Exit"
#define L_BTN_ABOUT          "About..."
#define L_BTN_BROWSE         "..."
#define L_BTN_BROWSE_TOOLTIP "Click to open file dialog"
#define L_SV_DATA            "Please select a directory with game files:"
#define L_SV_DATA_TOOLTIP    "Path to a directory with game files."

#define GAP        10.0f
#define BANNER_W   64.0f

class BeBasedWindow : public BeMainWindow
{
	enum
	{
		MSG_BUTTON_RUN_CLICKED =     'btrn',
		MSG_BUTTON_EXIT_CLICKED =    'btex',
		MSG_BUTTON_BROWSE_CLICKED =  'btbr',
		MSG_BUTTON_ABOUT_CLICKED =   'btab',
		MSG_TEXT_CONTROL_EDITED =    'tced'
	};

	BTextControl *fDataTextControl;
public:
	BeBasedWindow(void) : BeMainWindow(BRect(100, 100, 700, 500), L_APP_NAME)
	{
		CreateForm();
	}

	void CreateForm()
	{
		BRect r(Bounds());
		BRect bannerRect(r.left, r.top, BANNER_W, r.bottom);
		BRect stringViewRect(BANNER_W + GAP, BANNER_W + GAP, r.right, r.bottom);

		BView *mainView = new BView(r, "mainView", B_FOLLOW_ALL, B_WILL_DRAW);
		mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

		BView *bannerView = new BView(bannerRect, "bannerView", B_FOLLOW_NONE, B_WILL_DRAW);
		bannerView->SetViewColor(200, 0, 0);

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
		fDataTextControl = new BTextControl(dataTextControlRect, "dataTextControl", NULL, NULL, new BMessage(MSG_TEXT_CONTROL_EDITED), B_FOLLOW_LEFT_RIGHT);
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

		AddChild(mainView);
		AddChild(bannerView);
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
