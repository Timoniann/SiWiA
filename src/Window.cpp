#include "Window.h"

Window::Window(char * title, int width, int height, bool isMain) :
    Element(title, 300, 300, width, height)
{
    //static int winCount = 0;
    const char * className = "Window";
    if(!SetWinClass("Window", (HBRUSH)CreateSolidBrush(RGB(255, 255, 0))))
        MessageBox(NULL, "Cant registrate class", "Error", 0);

    initializer.style = WS_SYSMENU | WS_VSCROLL;
    initializer.parent = HWND_DESKTOP;
    //initializer.extendedStyle = WS_EX_COMPOSITED | WS_EX_TRANSPARENT;
    initializer.className = "Window";
    hMenu = NULL;
}

LRESULT CALLBACK Window::Procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hDC, hdcStatic;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
    static int yInc;
    switch(message)
    {
    case WM_NOTIFY:
        //std::cout << ((LPNMHDR)lParam)->code << " - code\n";
        break;
    case WM_CREATE:

        hDC = GetDC(hwnd);
		GetTextMetrics(hDC, &tm);
        cxChar = tm.tmAveCharWidth;
        yStep = tm.tmHeight + tm.tmExternalLeading;
        vsi.nMin = vsi.nPos = 0;
        ReleaseDC(hwnd, hDC);

        vsi.cbSize = sizeof(vsi);
        vsi.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        vsi.nPage = initializer.height / yStep - 1;
        if (multiTexts.size() != 0)
            vsi.nMax = multiTexts[0]->lines.size() - 1;
        if (vsi.nPage > vsi.nMax)
            vsi.nPos = vsi.nMin;
        vertRange = vsi.nMax - vsi.nMin + 1;
        SetScrollInfo(hwnd, SB_VERT, &vsi, TRUE);


        handle = hwnd;
        for(int i = 0; i < buttons.size(); i++){
            buttons[i]->Init(hwnd);
            buttons[i]->Show();
        }
        for(int i = 0; i < textAreas.size(); i++){
            textAreas[i]->Init(hwnd);
            textAreas[i]->Show();
        }
        for(int i = 0; i < listBoxes.size(); i++)
            listBoxes[i]->Init(hwnd);
        for (int i = 0; i < labels.size(); i++)
            labels[i]->Init(hwnd);
        break;

    case WM_CLOSE:
        if (CloseFunction != nullptr)
            if (!CloseFunction()) return 0;
        break;

    case WM_DESTROY:
        //~Window();
        //delete this;
        //Destroy();
        if (main) PostQuitMessage(0);

        break;

    case WM_COMMAND:
        for(int i = 0; i < buttons.size(); i++)
            if((int)buttons[i]->hMenu == (int)LOWORD(wParam))
                buttons[i]->Click();
        return 0;
        break;

    case WM_VSCROLL:
        switch(LOWORD(wParam)) {
		case SB_LINEUP:
			yInc = -1;  break;
		case SB_LINEDOWN:
			yInc = 1;  break;
		case SB_PAGEUP:
			yInc = -(int)vsi.nPage;  break;
		case SB_PAGEDOWN:
			yInc = (int)vsi.nPage;  break;
		case SB_THUMBTRACK:
			yInc = HIWORD(wParam) - vsi.nPos;  break;
		case SB_TOP:
			yInc = -vsi.nPos;  break;
		case SB_BOTTOM:
			yInc = vsi.nMax - vsi.nPos;    break;
		default:
			yInc = 0;
		}
		UpdateScroll(hwnd, yInc);
		break;

    case WM_PAINT:

        hDC = BeginPaint(hwnd, &ps);
        DrawMulti(hDC);
		//PutText(hwnd, hDC);
		EndPaint(hwnd, &ps);

        break;
    static HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);//CreateSolidBrush(TRANSPARENT);//RGB(230,230,230));

    //case WM_CTLCOLORBTN:

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC) wParam;
        //hdcStatic = GetDC(labels[0]->GetHandle());
        SetBkMode(hdcStatic,TRANSPARENT);
        SetBkColor(hdcStatic, TRANSPARENT);
        HBRUSH BGColorBrush = CreateSolidBrush(RGB(0,0,255));
        SetTextColor(hdcStatic, RGB(255,0,0));
        return (LRESULT)BGColorBrush;
        SetBkColor(hdcStatic, TRANSPARENT);// RGB(255,0,0));
        return (INT_PTR)hBrush;
    }
        /*
    case WM_CTLCOLORSTATIC:
        //MessageBox(NULL, "Static", "S", 0);
        hdcStatic = (HDC) wParam;
        SetTextColor(hdcStatic, RGB(0, 0, 0));
        SetBkMode(hdcStatic, TRANSPARENT);
        return (LRESULT)GetStockObject(NULL_BRUSH);
        break;*/
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void Window::DrawMulti(HDC hdc)
{
    if (multiTexts.size() == 0) return;
    SetBkColor(hdc, RGB(0, 0, 0));
    SetTextColor(hdc, RGB(255, 255, 255));

	RECT rect;
	GetClientRect(handle, &rect);
	rect.left += cxChar;
	rect.right -= cxChar;
	HRGN hRgn = CreateRectRgnIndirect(&rect);
	SelectClipRgn(hdc, hRgn);

	int x = cxChar;
	int y = yStep;

	int amountLines = multiTexts[0]->lines.size();
	int iBeg = vsi.nPos;
	int iEnd = (vsi.nPos+vsi.nPage < amountLines)? vsi.nPos+vsi.nPage : amountLines;
	for (int i = iBeg; i < iEnd; ++i) {
        for(int j = 0; j < multiTexts.size(); j++)
            TextOut(hdc, x + multiTexts[j]->x, y, multiTexts[j]->lines[i].c_str(), multiTexts[j]->lines[i].size());

        //TextOut(hdc, x + 350 + 100, y, lines_string[i].c_str(), lines_string[i].size());
        //TextOut(hdc, x + 100, y, lines_data[i].c_str(), lines_data[i].size());
        //TextOut(hdc, x, y, lines_ref[i].c_str(), lines_ref[i].size());

		y += yStep;
	}
	SelectClipRgn(hdc, NULL);

}

void Window::UpdateScroll(HWND hwnd, int yInc)
{
    // ограничение на положительное приращение
	yInc = min(yInc, vertRange - (int)vsi.nPage - vsi.nPos);
	// ограничение на отрицательное приращение
	yInc = max(yInc, vsi.nMin - vsi.nPos);

	if (yInc) {
		ScrollWindow(hwnd, 0, -yStep * yInc, NULL, NULL);
		vsi.nPos += yInc;
		SetScrollInfo(hwnd, SB_VERT, &vsi, TRUE);
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow (hwnd);
	}
}

bool Window::SetWinClass(const char* name, HBRUSH backgroundColor)
{
    wincl.hInstance = GetModuleHandle(NULL);
    wincl.lpszClassName = name;
    wincl.lpfnWndProc = StaticProc;
    wincl.style = CS_DBLCLKS; // | CS_HREDRAW | CS_VREDRAW
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = backgroundColor;

    return RegisterClassEx(&wincl);
}

void Window::AddButton(Button * b)
{
    buttons.push_back(b);
}

void Window::AddTextArea(TextArea * t)
{
    textAreas.push_back(t);
}

void Window::AddListBox(ListBox * l)
{
    listBoxes.push_back(l);
}

void Window::AddLabel(Label * l)
{
    labels.push_back(l);
}

void Window::AddMultiText(MultiText * m)
{
    multiTexts.push_back(m);
}

void Window::SetFuncClose(bool(*func)())
{
    this->CloseFunction = func;
}
/*
void Window::Show(int cmd)
{
    DWORD mask = WS_SYSMENU | WS_EX_COMPOSITED | WS_EX_TRANSPARENT;
    if (hasVScroll)
        mask = mask | WS_VSCROLL;
    handle = CreateWindowEx (
           0,
           wincl.lpszClassName,
           "kakasha",
           mask,//WS_SYSMENU | WS_VSCROLL | WS_EX_COMPOSITED | WS_EX_TRANSPARENT,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           300,
           300,
           HWND_DESKTOP,
           NULL,
           GetModuleHandle(NULL),
           this
           );
    //ShowWindow(handle, cmd);
    ShowWindowAsync(handle, cmd);
}

*/
