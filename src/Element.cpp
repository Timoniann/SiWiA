#include "Element.h"


LRESULT CALLBACK Element::StaticProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Element * wnd;
    if (message == WM_NCCREATE)
    {
        wnd = static_cast<Element*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetLastError(0);
        if(!SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR> (wnd)))
            if (GetLastError() != 0)
                return false;
    }
    else
        wnd = reinterpret_cast<Element*>(GetWindowLongPtr(hwnd, GWL_USERDATA));

    if (wnd)
        return wnd->Procedure(hwnd, message, wParam, lParam);

    return DefWindowProc (hwnd, message, wParam, lParam);
}

LRESULT CALLBACK Element::Procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return CallWindowProc(defaultProc, hwnd, message, wParam, lParam);
}


Element::Element()
{
    hMenu = (HMENU)handle_menu++;
}

Element::Element(char * text, int x, int y, int width, int height) : Element()
{
    initializer.title = text;
    initializer.x = x;
    initializer.y = y;
    initializer.width = width;
    initializer.height = height;
}

Element::~Element()
{
    DestroyWindow(handle);
}

void Element::Click()
{
    if (ClickFunction != nullptr)
        ClickFunction();
}

void Element::SetFuncClick(void(*func)())
{
    ClickFunction = func;
}

HWND Element::GetHandle()
{
    return handle;
}

void Element::Resize(int width, int height)
{
    SetWindowPos(handle, 0, 0, 0, width, height, SWP_NOMOVE  | SWP_NOZORDER | SWP_NOACTIVATE);
}

POINT Element::GetPosition()
{
    WINDOWPLACEMENT wp;
    GetWindowPlacement(handle, &wp);
    POINT pos;
    pos.x = wp.rcNormalPosition.left;
    pos.y = wp.rcNormalPosition.top;
    return pos;
}

int Element::GetWidth()
{
    WINDOWPLACEMENT wp;
    GetWindowPlacement(handle, &wp);
    return wp.rcNormalPosition.right - wp.rcNormalPosition.left;
}

int Element::GetHeight()
{
    WINDOWPLACEMENT wp;
    GetWindowPlacement(handle, &wp);
    return wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
}

void Element::ReMove(int x, int y)
{
    SetWindowPos(handle, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Element::Move(int dx, int dy)
{
    POINT pos = GetPosition();
    SetWindowPos(handle, 0, pos.x + dx, pos.y + dy, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Element::SetOnTop(bool always)
{
    SetWindowPos(handle, always ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

int Element::handle_menu = 5000;

void Element::SetTransparency(unsigned int degree)
{
    SetWindowLong(handle, GWL_EXSTYLE, GetWindowLong(handle, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(handle, RGB(255, 255, 255), 255 * degree / 100, LWA_ALPHA);
}

void Element::Redraw()
{
    RedrawWindow(handle, NULL, NULL, NULL);
}

void Element::SetFont(HFONT font)
{
    SendMessage(handle, WM_SETFONT, (WPARAM)font, TRUE);
}

HFONT Element::GetFont()
{
    return (HFONT)SendMessage(handle, WM_GETFONT, 0, 0);
}

void Element::Hide()
{
    ShowWindow(handle, SW_HIDE);
}

void Element::Show()
{
    ShowWindow(handle, SW_SHOW);
}

void Element::Init(HWND parent)
{
    handle = CreateWindowEx(
            initializer.extendedStyle,
            initializer.className,
            initializer.title,
            initializer.style,
            initializer.x,
            initializer.y,
            initializer.width,
            initializer.height,
            parent == 0 ? initializer.parent : parent,
            hMenu,
            GetModuleHandle(NULL),
            this
    );

    SetWindowLongPtr(handle, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
    if(parent != HWND_DESKTOP) defaultProc = (WNDPROC)SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(StaticProc));
    this->parent = parent;
}
