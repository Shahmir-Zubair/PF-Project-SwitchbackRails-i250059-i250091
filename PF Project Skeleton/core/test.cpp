#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main()
{   
    fstream file("../data/levels/easy_level.lvl");
    string line;
    if (!file.is_open()) 
    {
        cout<< "Error: could not open .lvl file"<<endl;
        return 1;
    }
    while(getline(file, line))
        cout<<line<<endl;
}