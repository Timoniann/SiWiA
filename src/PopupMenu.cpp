#include "PopupMenu.h"

PopupMenu::PopupMenu()
{
    handle = CreatePopupMenu();
}

PopupMenu::~PopupMenu()
{
    DestroyMenu(handle);
}

void PopupMenu::Add(LPSTR text)
{
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ms647578(v=vs.85).aspx
    MENUITEMINFO itemInfo2;
    itemInfo2.cbSize = sizeof(MENUITEMINFO);
    itemInfo2.fMask = MIIM_STRING;
    itemInfo2.dwTypeData = text;
    if(!InsertMenuItem(handle, 0, FALSE, &itemInfo2))
        MessageBox(NULL, "Cant create menu item", "Error", 0);

}

void PopupMenu::Show(HWND hwnd)
{
    POINT p;
    GetCursorPos(&p);
    cout << "Item is " << TrackPopupMenuEx(handle, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | 5 | TPM_NONOTIFY | TPM_RETURNCMD, p.x, p.y, hwnd, NULL) << ";\n";
}

void PopupMenu::AddSeparator()
{
    MENUITEMINFO itemInfo2;
    itemInfo2.cbSize = sizeof(MENUITEMINFO);
    itemInfo2.fMask = MIIM_TYPE;
    itemInfo2.fType = MFT_SEPARATOR;
    if(!InsertMenuItem(handle, 0, FALSE, &itemInfo2))
        MessageBox(NULL, "Cant create menu item", "Error", 0);
}

void PopupMenu::AddChecked(LPSTR text)
{
    MENUITEMINFO itemInfo2;
    itemInfo2.cbSize = sizeof(MENUITEMINFO);
    itemInfo2.fMask = MIIM_STRING | MIIM_STATE | MIIM_ID;
    itemInfo2.wID = 3;
    itemInfo2.fState = MFS_CHECKED | MFS_HILITE; ///  MFS_DEFAULT - bold
    itemInfo2.dwTypeData = text;
    if(!InsertMenuItem(handle, 0, FALSE, &itemInfo2))
        MessageBox(NULL, __FUNCTION__, "Error", 0);
}

void PopupMenu::AddBitmap(LPSTR text, HBITMAP bitmap)
{
    MENUITEMINFO itemInfo2;
    itemInfo2.cbSize = sizeof(MENUITEMINFO);
    itemInfo2.fMask = MIIM_STRING | MIIM_BITMAP;
    itemInfo2.dwTypeData = text;
    itemInfo2.hbmpItem = bitmap;
    if(!InsertMenuItem(handle, 0, FALSE, &itemInfo2))
        MessageBox(NULL, "Cant create menu item", "Error", 0);
}

void PopupMenu::AddSubmenu(LPSTR text, HMENU sub)
{
    MENUITEMINFO itemInfo2;
    itemInfo2.cbSize = sizeof(MENUITEMINFO);
    itemInfo2.fMask = MIIM_STRING | MIIM_SUBMENU;
    itemInfo2.dwTypeData = text;
    itemInfo2.hSubMenu = sub;
    if(!InsertMenuItem(handle, 0, FALSE, &itemInfo2))
        MessageBox(NULL, "Cant create menu item", "Error", 0);
}



HMENU PopupMenu::GetHandle()
{
    return handle;
}
