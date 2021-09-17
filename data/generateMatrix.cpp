#include <bits/stdc++.h>

using namespace std;

int main(int agrc, char* agrv[])
{
    ofstream fout(agrv[2], ios::out);
    int n = stoi(agrv[1]);
    for(int a = 0; a < n; a++)
    {
        for(int b = 0; b < n - 1; b++)
            fout << a + b <<",";
        fout << a + n - 1 << "\n";
    }
    fout.close();
    return 0;
}