#ifndef MULTITEXT_H
#define MULTITEXT_H

#include <string>
#include <vector>

using namespace std;

class MultiText
{
    public:
        MultiText(vector<string> & data);
        ~MultiText();
        int x;
        void Add(string str);
        vector<string> lines;
    protected:

    private:
};

#endif // MULTITEXT_H
