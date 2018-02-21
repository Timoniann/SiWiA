#ifndef TEXTAREA_H
#define TEXTAREA_H

#include <Element.h>

class TextArea : public Element
{
    public:
        TextArea(int x, int y, int width, int height);
        char * GetContent();
        char * GetSelectedText();
        void SetSelectedText(int start, int end);
        void SetContent(char * text);
        void ReplaceSelectedText(char * text, bool canBeUndone = true);
        void SetLimit(int limit);
        int GetLineLength(int charPosition);
        int GetFirstVisibleLine();
        char * GetLine(int index);
        int GetLinesCount();
        int GetLineFromChar(char c);
        int GetLineIndex(int lineIndex);
        void SetMargins(int left, int right);
        LPARAM GetMargins();
        void SetPasswordCharacter(char c);
        char GetPasswordCharacter();
        void SetDialogItemInt(int value);
        int GetCharacterIndexFromPosition(int x, int y);
        POINTL GetPositionFromCharacterIntex(int index);

        HWND parent;
    protected:
        LRESULT CALLBACK Procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

#endif // TEXTAREA_H
