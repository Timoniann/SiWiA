#include "Label.h"

Label::Label(char * text, int x, int y, int width, int height) :
    Element(text, x, y, width < 0 ? strlen(text) * 20 : width, height < 0 ? 20 : height)
{
    initializer.className = "STATIC";
    initializer.extendedStyle = WS_EX_TRANSPARENT;
    initializer.style = WS_CHILD | WS_VISIBLE | SS_SIMPLE;
}

// Static styles https://msdn.microsoft.com/ru-ru/library/windows/desktop/bb760773(v=vs.85).aspx

void Label::SetText(char * text)
{
    SendMessage(handle, WM_SETTEXT, 0, (LPARAM) text);
    Redraw();
}
