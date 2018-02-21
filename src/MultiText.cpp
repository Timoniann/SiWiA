#include "MultiText.h"

MultiText::MultiText(vector<string> & data)
{
    lines = *&data;
}

MultiText::~MultiText()
{
    //dtor
}

void MultiText::Add(string str)
{
    lines.push_back(str);
}
