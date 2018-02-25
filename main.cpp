#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <Window.h>
#include <PopupMenu.h>
#define ICC_ALL_CLASSES ICC_WIN95_CLASSES | ICC_DATE_CLASSES | ICC_USEREX_CLASSES | ICC_COOL_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS | ICC_NATIVEFNTCTL_CLASS | INFOTIPSIZE
// + ICC_STANDARD_CLASSES | ICC_LINK_CLASS


Window * win_main;
Button * butt_1;
Button * butt_move;
Label * lbl;
TextArea * textArea;

WebCameraCapture * webCam;


void Click()
{
    lbl->SetText("Changed label text");

    HMENU hPopupMenu = CreatePopupMenu();

    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0, (LPCSTR)"Exit");
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 1, (LPCSTR)"Play");

    int ID = 2;

    MENUITEMINFO itemInfo;
    itemInfo.cbSize = sizeof(MENUITEMINFO);
    itemInfo.fMask = MIIM_STRING | MIIM_BITMAP;
    itemInfo.hbmpItem = HBMMENU_MBAR_CLOSE;
    itemInfo.dwTypeData = "TestXXXXXXXXXXX";
    if(!InsertMenuItem(hPopupMenu, ID, FALSE, &itemInfo)) MessageBox(NULL, "No", "Error", 0);

    MENUITEMINFO itemInfo2;
    itemInfo2.cbSize = sizeof(MENUITEMINFO);
    itemInfo2.fMask = MIIM_STRING | MIIM_STATE;
    itemInfo2.fState = MFS_CHECKED;
    //itemInfo2.fType = MFT_MENUBARBREAK;
    itemInfo2.dwTypeData = "Yes";
    if(!InsertMenuItem(hPopupMenu, ID, FALSE, &itemInfo2)) MessageBox(NULL, "No", "Error", 0);


    //MIIM_DATA  MFT_STRING;// | MIIM_STRING;
    //itemInfo.fType = MFS_CHECKED;

    //itemInfo.fState = MFS_CHECKED;



    InsertMenu(hPopupMenu, 4, MF_BYPOSITION | MF_STRING, 9, (LPCSTR)"Play8");
    //SetForegroundWindow(win_main->GetHandle());
    //TrackPopupMenuEx(hPopupMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | 0x1000L, win_main->GetPosition().x, win_main->GetPosition().y, win_main->GetHandle(), NULL);

    PopupMenu p = PopupMenu();
    p.Add("@213123");
    p.AddChecked("&I am checked\tCtrl-Y");
    p.AddSeparator();
    p.Add("Oh, no");
    p.AddBitmap("With bitmap", HBMMENU_MBAR_MINIMIZE);
    p.AddSubmenu("SubMenu", hPopupMenu);

    p.Show(win_main->GetHandle());

}

void WindMove()
{
    win_main->Move(1, 1);
    win_main->Resize(win_main->GetWidth() + 1, win_main->GetHeight() - 1);
    win_main->SetTransparency(90);
    lbl->SetTransparency(40);
    //textArea->SetMargins(30, 30);
    SendMessage(textArea->GetHandle(), EM_FMTLINES, FALSE, 0);
}

void Controls()
{
    INITCOMMONCONTROLSEX st;
    st.dwSize = sizeof(INITCOMMONCONTROLSEX);
    st.dwICC = ICC_ALL_CLASSES;
    if(!InitCommonControlsEx(&st))
        MessageBox(NULL, "Oh, no. Can't initialize", "InitCommonControls", 0);
}


int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    Controls();
    win_main = new Window("Main", 1000, 700, true);

    butt_1 = new Button("Click me", 10, 10, 70, 20);
    butt_1->SetFuncClick(Click);
    win_main->AddElement(butt_1);

    butt_move = new Button("Move!", 200, 10, 200, 30);
    butt_move->SetFuncClick(WindMove);

    textArea = new TextArea(50, 50, 400, 200);
    textArea->initializer.title = "Change me)";
    win_main->AddElement(textArea);

    lbl = new Label("Hello, k", 200, 200);
    win_main->AddElement(lbl);

    win_main->AddElement(butt_move);

    webCam = new WebCameraCapture(450, 50, 500, 400);
    win_main->AddElement(webCam);

    win_main->Init(HWND_DESKTOP);
    /// https://msdn.microsoft.com/en-us/library/ms632669(VS.85).aspx
    if(!AnimateWindow(win_main->GetHandle(), 500, AW_CENTER)) MessageBox(NULL, "WinAPI", "Hello", 0);
    //win_main->Show();

    MSG messages;

    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}
