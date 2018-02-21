#ifndef LABEL_H
#define LABEL_H

#include <Element.h>

class Label : public Element
{
    public:
        Label(char * text, int x, int y, int width = -1, int height = -1);
        void SetText(char * text);

    protected:

    private:
};

#endif // LABEL_H
