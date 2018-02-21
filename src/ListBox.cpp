#include "ListBox.h"

ListBox::ListBox(char * text, int x, int y, int width, int height) :
    Element(text, x, y, width, height)
{
    initializer.className = "LISTBOX";
    initializer.style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_LEFT;
    initializer.extendedStyle = NULL;
}

void ListBox::Insert(char * item, int position)
{
    int pos = (int) SendMessage(handle, LB_INSERTSTRING, position, (LPARAM)item);
    SendMessage(handle, LB_SETITEMDATA, pos, 0);
}

void ListBox::Add(char * item)
{
    int pos = (int) SendMessage(handle, LB_ADDSTRING, 0, (LPARAM)item);
    SendMessage(handle, LB_SETITEMDATA, pos, 0);
}

void ListBox::Clear()
{
    SendMessage(handle, LB_RESETCONTENT, NULL, NULL);
}
