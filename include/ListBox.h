#ifndef LISTBOX_H
#define LISTBOX_H

#include <Element.h>

class ListBox : public Element
{
    public:
        ListBox(char * text, int x, int y, int width, int height);
        void Insert(char * item, int position = 0);
        void Add(char * item);
        void Clear();
    protected:

    private:
};

#endif // LISTBOX_H
