#ifndef BUTTON_H
#define BUTTON_H

#include <Element.h>

class Button : public Element
{
    public:
        Button(char * text, int x, int y, int width, int height);
        SIZE & GetIdealSize();
    protected:

    private:
};

#endif // BUTTON_H
