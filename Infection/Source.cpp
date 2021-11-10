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


class Infection
{
public:
    int curPlayer = 1;
    string startPlace, endPlace;
    Field field;

    Infection(Field f)
    {
        this->field = f;
    }

    void playGame()
    {
        while (true) {
            switch (curPlayer)
            {
            case 1:
            p1:            cout << "\nХодит 1-ый игрок:  ";
                cin >> startPlace >> endPlace; // ход

                placeToInt(startPlace, endPlace);

                if (!isCorrectMove(startPlace, endPlace))
                    goto p1;

                if (abs((startPlace[1] - '0') - (endPlace[1] - '0')) == 2 || abs((startPlace[0] - '0') - (endPlace[0] - '0')) == 2)
                    field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] = 0;
                field.places[endPlace[1] - '0' - 1][endPlace[0] - '0' - 1] = 1;


                break;
            case 2:
            p2:            cout << "\nХодит 2-ой игрок:  ";
                cin >> startPlace >> endPlace; // ход

                placeToInt(startPlace, endPlace);

                if (!isCorrectMove(startPlace, endPlace))
                    goto p2;

                if (abs((startPlace[1] - '0') - (endPlace[1] - '0')) == 2 || abs((startPlace[0] - '0') - (endPlace[0] - '0')) == 2)
                    field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] = 0;
                field.places[endPlace[1] - '0' - 1][endPlace[0] - '0' - 1] = 2;

                break;
            }

            field.printField();

            if (gameOver())
                return;

            curPlayer = curPlayer == 1 ? 2 : 1;
        }
    }

    bool gameOver()
    {
        int countFirst = 0, countSecond = 0;
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
            {
                if (field.places[i][j] == 1)
                    countFirst++;
                if (field.places[i][j] == 2)
                    countSecond++;
                if (field.places[i][j] == 0)
                    return false; // игра не окончена
            }

        cout << "\nИгра окончена! ";
        if (countFirst > countSecond)
            cout << "Победу одержал 1-ый игрок!\n";
        else if (countFirst < countSecond)
            cout << "Победу одержал 2-ой игрок!\n";
        else
            cout << "Ничья!\n";
        return true;
    }

    void placeToInt(string& startPlace, string& endPlace)
    {
        string intStartPlace = "", intEndPlace = "";
        switch (startPlace[0]) {
        case 'a':
            intStartPlace += "1";
            break;
        case 'b':
            intStartPlace += "2";
            break;
        case 'c':
            intStartPlace += "3";
            break;
        case 'd':
            intStartPlace += "4";
            break;
        case 'e':
            intStartPlace += "5";
            break;
        default:
            intStartPlace += "6";
            break;
        }
        intStartPlace += startPlace[1];

        switch (endPlace[0]) {
        case 'a':
            intEndPlace += "1";
            break;
        case 'b':
            intEndPlace += "2";
            break;
        case 'c':
            intEndPlace += "3";
            break;
        case 'd':
            intEndPlace += "4";
            break;
        case 'e':
            intEndPlace += "5";
            break;
        default:
            intEndPlace += "6";
            break;
        }
        intEndPlace += endPlace[1];

        //cout << intStartPlace << " " << intEndPlace << endl;
        startPlace = intStartPlace;
        endPlace = intEndPlace;
    }

    bool isCorrectMove(string s, string e)
    {
        if (!(s[0] == '1' || s[0] == '2' || s[0] == '3' || s[0] == '4' || s[0] == '5' || s[0] == '6')
            || !(s[1] == '1' || s[1] == '2' || s[1] == '3' || s[1] == '4' || s[1] == '5' || s[1] == '6'))
        {
            cout << "\nНекорректно введена начальная точка\n";
            return false;
        }

        if (!(e[0] == '1' || e[0] == '2' || e[0] == '3' || e[0] == '4' || e[0] == '5' || e[0] == '6')
            || !(e[1] == '1' || e[1] == '2' || e[1] == '3' || e[1] == '4' || e[1] == '5' || e[1] == '6'))
        {
            cout << "\nНекорректно введена конечная точка\n";
            return false;
        }


        // проверка начальной точки хода
        switch (curPlayer)
        {
        case 1:
            if (field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] != 1) {
                cout << "\nНачальная точка не принадлежит 1-му игроку\n";
                return false;
            }
            break;
        case 2:
            if (field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] != 2) {
                cout << "\nНачальная точка не принадлежит 2-му игроку\n";
                return false;
            }
            break;
        }

        // проверка конечной точки хода
        if (abs((startPlace[1] - '0') - (endPlace[1] - '0')) > 2 || abs((startPlace[0] - '0') - (endPlace[0] - '0')) > 2 || field.places[endPlace[1] - '0' - 1][endPlace[0] - '0' - 1] != 0)
        {
            cout << "\nНевозможно сделать такой ход\n";
            return false;
        }

        return true;
    }
};

int main()
{
    setlocale(LC_ALL, "rus");

    Field f;
    f.printField(); // вывод начального поля
        
    Infection inf(f);
    inf.playGame();

    return 0;
}