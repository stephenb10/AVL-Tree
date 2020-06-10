#include <iostream>
#include <string>
#include <stdlib.h>     
#include <time.h>     
#include <sstream>
#include "Utility.h"

using namespace std;
#include "Idea.h"
#include "IdeasBank.h"

int getUserInput();

int main()
{
    srand(time(0));

    IdeasBank ideasbank;
    while (1)
    {
        int input = getUserInput();
        switch (input)
        {
        case 0:
            ideasbank.addIdea();
            break;
        case 1:
            ideasbank.ideasFromFile();
            break;
        case 2:
            ideasbank.display();
            break;
        case 3:
            ideasbank.printData();
            break;
        case 4:
            ideasbank.searchIdeas(false);
            break;
        case 5:
            ideasbank.searchIdeas(true);
            break;
        case 6:
            ideasbank.displayRelatedIdeas();
            break;
        case 7:
            ideasbank.deleteIdea();
            break;
        case 8:
            cout << "Terminating...\n";
            return 0;
            break;
        default:
            break;
        }
    }
}

int getUserInput()
{
    int input;
    do
    {
        cout << "0.\tCreate a new idea\n";
        cout << "1.\tImport ideas from a file\n";
        cout << "2.\tPrint the ideas bank\n";
        cout << "3.\tPrint word indices\n";
        cout << "4.\tSearch for realted ideas from ideas bank\n";
        cout << "5.\tSearch for related ideas from indexing tree\n";
        cout << "6.\tRelated ideas of an idea\n";
        cout << "7.\tDelete an idea\n";
        cout << "8.\tQuit\n";
        cout << "Input: ";
        cin >> input;
        cout << endl;
    } while (input < 0 || input > 8);

    return input;
}