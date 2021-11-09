#include <iostream>
#include <vector>

using namespace std;

class Field
{
public:
    int places[6][6];

    Field()
    {
        // начальное поле инфекции
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                places[i][j] = 0;
        places[0][0] = 1;
        places[0][5] = 2;
        places[5][0] = 1;
        places[5][5] = 2;
    }

    void printField()
    {
        cout << "   a b c d e f\n1  ";

        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
                cout << places[i][j] << " ";
            if (i != 5)
                cout << endl << i + 2 << "  ";
        }

        cout << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "rus");

    Field f;
    f.printField();
        

    return 0;
}