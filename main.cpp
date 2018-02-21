#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <Window.h>
#define ICC_ALL_CLASSES ICC_WIN95_CLASSES | ICC_DATE_CLASSES | ICC_USEREX_CLASSES | ICC_COOL_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS | ICC_NATIVEFNTCTL_CLASS | INFOTIPSIZE
// + ICC_STANDARD_CLASSES | ICC_LINK_CLASS


Window * win_main;
Button * butt_1;
Button * butt_move;
Label * lbl;
TextArea * textArea;

void Click()
{
    //MessageBox(butt_1->GetHandle(), "Clicked", "Test", 0);
    lbl->SetText("Changed label text");
    win_main->Resize(600, 600);
    //textArea->Hide();//~Element();
    //delete textArea;
    //win_main->SetOnTop();
    //textArea->SetTransparency(40);
    //lbl->Redraw();
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
    win_main->AddButton(butt_1);

    butt_move = new Button("Move!", 200, 10, 200, 30);
    butt_move->SetFuncClick(WindMove);

    textArea = new TextArea(50, 50, 400, 200);
    textArea->initializer.title = "Change me)";
    win_main->AddTextArea(textArea);

    lbl = new Label("Hello, k", 200, 200);
    win_main->AddLabel(lbl);

    win_main->AddButton(butt_move);

    win_main->Init(HWND_DESKTOP);
    win_main->Show();

    MSG messages;

    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}
