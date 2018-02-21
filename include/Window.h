#ifndef WINDOW_H
#define WINDOW_H

#include <map>
#include <vector>
#include <string>
#include <Button.h>
#include <TextArea.h>
#include <ListBox.h>
#include <Label.h>
#include <MultiText.h>

using namespace std;

class Window : public Element
{
    public:
        bool main = false;
        bool hasVScroll = false;
        Window(char * title, int wigth, int height, bool isMain = false);

        void AddButton(Button * b);
        void AddTextArea(TextArea * t);
        void AddListBox(ListBox * l);
        void AddLabel(Label * l);
        void AddMultiText(MultiText * m);
        void Update();

        bool SetWinClass(const char* name, HBRUSH backgroundColor = (HBRUSH) COLOR_BACKGROUND);

        void DrawMulti(HDC hdc);
        void UpdateScroll(HWND hwnd, int yInc);
        void Hide();

        void SetFuncClose(bool(*func)());

        vector<Button*> buttons;
        vector<TextArea*> textAreas;
        vector<ListBox*> listBoxes;
        vector<Label*> labels;
        vector<MultiText*> multiTexts;

    protected:
        bool (*CloseFunction)() = nullptr;
        WNDCLASSEX wincl;
    private:
        SCROLLINFO vsi;
        int vertRange;
        int yStep;
        int cxChar;
        LRESULT CALLBACK Procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

#endif // WINDOW_H
