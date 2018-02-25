#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <Element.h>

class PopupMenu
{
    public:
        PopupMenu();
        virtual ~PopupMenu();
        void Add(LPSTR text);
        void AddSeparator();
        void AddChecked(LPSTR text);
        void AddBitmap(LPSTR text, HBITMAP bitmap);
        void AddSubmenu(LPSTR text, HMENU sub);
        void Show(HWND hwnd);

        HMENU GetHandle();

    protected:
        HMENU handle;

    private:
};

#endif // POPUPMENU_H
