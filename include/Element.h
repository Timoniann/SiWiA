#ifndef ELEMENT_H
#define ELEMENT_H

#define _WIN32_IE 0x0501
#define _WIN32_WINNT 0x0601
#define WINVER 0x0601

#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <string>

using namespace std;

struct ElementInitializer
{
    // WS_EX_ACCEPTFILES
    DWORD style;
    DWORD extendedStyle;
    int x;
    int y;
    int width;
    int height;
    HWND parent;
    LPCTSTR title;
    LPCTSTR className;
};

class Element
{
    public:
        Element();
        Element(char * text, int x, int y, int width, int height);

        virtual void Show();
        void SetFuncClick(void(*func)());
        void Click();
        void Resize(int width, int height);
        void Move(int dx, int dy);
        void ReMove(int x, int y);
        void SetOnTop(bool always = true);
        void SetTransparency(unsigned int degree);
        void Redraw();
        void Hide();
        void SetFont(HFONT font);
        HFONT GetFont();

        POINT GetPosition();
        int GetWidth();
        int GetHeight();
        HWND GetHandle();

        HMENU hMenu;
        HWND parent;
        ElementInitializer initializer;

        virtual ~Element();
        void Init(HWND parent = HWND_DESKTOP);

    protected:
        void (*ClickFunction)() = nullptr;
        static int handle_menu;
        HWND handle;
        virtual LRESULT CALLBACK Procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK StaticProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT (CALLBACK* defaultProc)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    private:
};

#endif // ELEMENT_H
