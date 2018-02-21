#include "Button.h"

Button::Button(char * text, int x, int y, int width, int height) :
    Element(text, x, y, width, height)
{
    initializer.className = "BUTTON";
    initializer.extendedStyle = NULL;
    initializer.style = WS_CHILD | WS_VISIBLE | BS_TOP | BS_RIGHT | BS_PUSHLIKE;
}


SIZE & Button::GetIdealSize()
{
    SIZE size;
    size.cx = 0;
    size.cy = 0;
    //SendMessage(handle, BCM_GETIDEALSIZE, 0, &size);
    return size;
}


//Debug::Console::RegisterCommand( «my_command», [](){ std::cout << «It's working!» << std::endl; } );
/*
BS_3STATE
BS_AUTO3STATE
BS_AUTOCHECKBOX
BS_AUTORADIOBUTTON
BS_BITMAP
BS_BOTTOM
BS_CENTER
BS_CHECKBOX
BS_COMMANDLINK
BS_DEFCOMMANDLINK
BS_DEFPUSHBUTTON
BS_DEFSPLITBUTTON
BS_GROUPBOX
BS_ICON
BS_FLAT
BS_LEFT
BS_LEFTTEXT
BS_MULTILINE
BS_NOTIFY
BS_OWNERDRAW
BS_PUSHBUTTON
BS_PUSHLIKE
BS_RADIOBUTTON
BS_RIGHT
BS_RIGHTBUTTON
BS_SPLITBUTTON
BS_TEXT
BS_TOP
BS_VCENTER*/
