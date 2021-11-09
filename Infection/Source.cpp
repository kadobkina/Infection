#include <iostream>
#include <vector>

using namespace std;

class Board
{
public:
    vector<int> places;

    Board(vector<int> pl)
    {
        this->places = pl;
    }

    void printBoard()
    {
        cout << "   a b c d e f\n1  ";
        int col = 0;

        for (int p : places)
        {
            cout << p << " ";
            col++;
            if (col!= 36 && col % 6 == 0)
                cout << endl << col / 6 + 1 << "  ";
        }
    }
};

int main()
{
    setlocale(LC_ALL, "rus");

    vector<int> pl = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    Board b(pl);
    b.printBoard();
        

    return 0;
}