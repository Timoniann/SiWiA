#include "TextArea.h"

TextArea::TextArea(int x, int y, int width, int height) :
    Element("", x, y, width, height)
{
    initializer.className = "EDIT";
    initializer.style = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL;
    initializer.extendedStyle = NULL;
}

char * TextArea::GetContent()
{
    int length = SendMessage(handle, WM_GETTEXTLENGTH, 0, 0);
    char * text = new char[max(length, 1024)];
    //GetWindowTextLength(handle)
    GetWindowText(handle, text, length);
    //text[length + 2] = '\n';
    return text;
}

void TextArea::SetContent(char * text)
{
    SendMessage(handle,  WM_SETTEXT, 0, (LPARAM) text);
}

char * TextArea::GetSelectedText()
{
    int e1 = 0, e2 = 0;
    LRESULT lres = SendMessage(handle, EM_GETSEL, (WPARAM)&e1, (LPARAM)&e2);
    char * text = GetContent();
    int len = e2 - e1;
    cout << "Text: " << text << "(" << len << ")" << '\n';
    char * resultText = new char[len];
    for(int i = 0; i < len; i++)
        resultText[i] = text[i + e1];
    cout << "RESULT_TEXT: '" << resultText << "'\n";
    return resultText;
}

void TextArea::SetSelectedText(int start, int end)
{
    SendMessage(handle, EM_SETSEL, start, end);
}

LRESULT CALLBACK TextArea::Procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_KEYDOWN)
    {
        if (GetKeyState(VK_CONTROL) & 0x8000 && wParam == 'A') {
            SendMessage(hwnd, EM_SETSEL, 0, -1);
        }
        if (GetKeyState(VK_CONTROL) & 0x800 && wParam == 'Q'){
            char * selectedText = GetSelectedText();
            cout << '\'' << selectedText << "' - selected text\n";
            delete selectedText;
            //MessageBox(handle, GetSelectedText(), "Content", 0);
        }
        if (GetKeyState(VK_CONTROL) & 0x800 && wParam == 'R'){
            ReplaceSelectedText("Replaced");
        }

    }
    //return DefWindowProc(hwnd, message, wParam, lParam);
    return CallWindowProc(defaultProc, hwnd, message, wParam, lParam);
}

void TextArea::ReplaceSelectedText(char * text, bool canBeUndone)
{
    SendMessage(handle, EM_REPLACESEL, (WPARAM)canBeUndone, (LPARAM)text);
}

void TextArea::SetLimit(int limit)
{
    SendMessage(handle, EM_SETLIMITTEXT, limit, 0);
}

int TextArea::GetLineLength(int charPosition)
{
    return SendMessage(handle, EM_LINELENGTH, charPosition, 0);
}

int TextArea::GetFirstVisibleLine()
{
    return SendMessage(handle, EM_GETFIRSTVISIBLELINE, 0, 0);
}

char * TextArea::GetLine(int index)
{
    char * text = new char[1024];
    SendMessage(handle, EM_GETLINE, index, (LPARAM)text);
    return text;
}

int TextArea::GetLinesCount()
{
    return SendMessage(handle, EM_GETLINECOUNT, 0, 0);
}

int TextArea::GetLineFromChar(char c)
{
    return SendMessage(handle, EM_LINEFROMCHAR, c, 0);
}

int TextArea::GetLineIndex(int lineIndex)
{
    return SendMessage(handle, EM_LINEINDEX, lineIndex, 0);
}

void TextArea::SetMargins(int left, int right)
{
    SendMessage(handle, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(left, right));
}

// left - LOWORD, right HIWORD
LPARAM TextArea::GetMargins()
{
    return SendMessage(handle, EM_GETMARGINS, 0, 0);
}

void TextArea::SetPasswordCharacter(char c)
{
    SendMessage(handle, EM_SETPASSWORDCHAR, c, 0);
}

char TextArea::GetPasswordCharacter()
{
    return SendMessage(handle, EM_GETPASSWORDCHAR, 0, 0);
}

char * GetErrorMessage()
{
    LPTSTR lpszFunction = (LPTSTR)"Error: ";
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    lpDisplayBuf = (LPVOID)((string)lpszFunction + " failed with error " + to_string(dw) + " (" + (string)(char*)lpMsgBuf + ")").c_str();
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    //ExitProcess(dw);
}

// Not working
void TextArea::SetDialogItemInt(int value)
{
    if(SendMessage(parent, (int)hMenu, value, TRUE) == NULL) {
        cout << "Last error: " << GetLastError() << ".\n";
        GetErrorMessage();
    }
}

int TextArea::GetCharacterIndexFromPosition(int x, int y)
{
    return LOWORD(SendMessage(handle, EM_CHARFROMPOS, 0, MAKELPARAM(x, y)));
    //HIWORD - is index of line
}

POINTL TextArea::GetPositionFromCharacterIntex(int index)
{
    LPARAM res = SendMessage(handle, EM_POSFROMCHAR, index, 0);
    POINTL point;
    point.x = LOWORD(res);
    point.y = HIWORD(res);
    return point;
}
