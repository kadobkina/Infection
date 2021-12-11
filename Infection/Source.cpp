#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>

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
        places[5][0] = 2;
        places[5][5] = 1;
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
    int countFirst = 0, countSecond = 0;
    const int MAX = 1000;
    const int MIN = -1000;

    Infection(Field f)
    {
        this->field = f;
    }

    // игра между двумя людьми
    void playGame()
    {
        while (true) {
p:          cout << "\nХодит игрок №" << curPlayer << ":  ";
            cin >> startPlace >> endPlace; // ход

            placeToInt(startPlace, endPlace);

            if (!isCorrectMove(startPlace, endPlace))
                goto p;

            if (abs((startPlace[1] - '0') - (endPlace[1] - '0')) == 2 || abs((startPlace[0] - '0') - (endPlace[0] - '0')) == 2)
                field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] = 0;
            field.places[endPlace[1] - '0' - 1][endPlace[0] - '0' - 1] = curPlayer;

            field.printField();

            if (gameOver())
            {
                cout << "\nИгра окончена! ";
                if (countFirst > countSecond)
                    cout << "Победу одержал 1-ый игрок!\n";
                else if (countFirst < countSecond)
                    cout << "Победу одержал 2-ой игрок!\n";
                else
                    cout << "Ничья!\n";
                return;
            }

            curPlayer = curPlayer == 1 ? 2 : 1;
        }
    }

    // игра двух ботов
    void playWithBot()
    {
        // случайный выбор игрока, делающего 1-ый ход
        //int r = rand() % (9 + 1);
        //curPlayer = r > 4 ? 2 : 1;

        while (true)
        {
            curPlayer = 1;
p1:         cout << "\nХодит игрок №" << curPlayer << ":  ";
            cin >> startPlace >> endPlace; // ход

            placeToInt(startPlace, endPlace);

            if (!isCorrectMove(startPlace, endPlace))
                goto p1;

            if (abs((startPlace[1] - '0') - (endPlace[1] - '0')) == 2 || abs((startPlace[0] - '0') - (endPlace[0] - '0')) == 2)
                field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] = 0;
            field.places[endPlace[1] - '0' - 1][endPlace[0] - '0' - 1] = curPlayer;

            field.printField();

            curPlayer = 2;
            Field tempField;
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++)
                    tempField.places[i][j] = field.places[i][j];

            pair<int, pair<string, string>> ai_move = minimax(curPlayer, 0, tempField, -10, 10);
            field.places[ai_move.second.second[1] - '0' - 1][ai_move.second.second[0] - '0' - 1] = curPlayer;

            cout << "\nХодит игрок №" << curPlayer << ": " << ai_move.second.first << " " << ai_move.second.second << endl;
            field.printField();

            if (gameOver())
            {
                cout << "\nИгра окончена! ";
                if (countFirst > countSecond)
                    cout << "Победу одержал 1-ый игрок!\n";
                else if (countFirst < countSecond)
                    cout << "Победу одержал 2-ой игрок!\n";
                else
                    cout << "Ничья!\n";
                return;
            }

            //minimax(10, field, MIN, MAX);
        }
    }

    pair<int, pair<string, string>> minimax(int player, int depth, Field& field, int alpha, int beta)
    {
        //curPlayer = curPlayer == 1 ? 2 : 1;
        // лучший ход 

        pair<string, string> bestMove = make_pair("", "");
        // AI = 10, player = -10
        int bestScore = player == 2 ? 10 : -10;

        if (gameOver())
            return make_pair(bestScore, bestMove);

        // все возможные ходы
        vector<pair<string, string>> possibleSteps = getPossibleSteps(player);

        for (int i = 0; i < possibleSteps.size(); i++)
        {
            pair<string, string> curMove = make_pair(possibleSteps[i].first, possibleSteps[i].second);
            //field.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = curPlayer;
            if (abs((curMove.first[1] - '0') - (curMove.second[1] - '0')) == 2 || abs((curMove.first[0] - '0') - (curMove.second[0] - '0')) == 2)
                field.places[curMove.first[0] - '0'][curMove.first[1] - '0'] = 0;
            field.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = player;

            if (player == 2)
            {
                //curPlayer = 1;
                int score = minimax(1, depth + 1, field, alpha, beta).first;

                if (bestScore < score)
                {
                    bestScore = score - depth * 10;
                    bestMove = curMove;

                    alpha = max(alpha, bestScore);
                    field.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = -1;
                    if (beta <= alpha)
                    {
                        break;
                    }
                }

            } 
            else
            {
                int score = minimax(2, depth + 1, field, alpha, beta).first;

                if (bestScore > score)
                {
                    bestScore = score + depth * 10;
                    bestMove = curMove;

                    beta = min(beta, bestScore);
                    field.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = -1;
                    if (beta <= alpha)
                    {
                        break;
                    }
                }

            }
            field.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = -1;
        }
        return make_pair(bestScore, bestMove);
    }

    // все возможные начальные точки хода
    vector<string> getPossibleStart(int player)
    {
        vector<string> posStart;
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                bool flag = false;
                if (field.places[i][j] == player)
                {
                    for (int ii = 0; ii < 6; ii++)
                    {
                        for (int jj = 0; jj < 6; jj++)
                        {
                            if (!(abs(jj - j) > 2 || abs(ii - i) > 2 || field.places[ii][jj] != 0))
                            {
                                string step = to_string(i) + to_string(j);
                                posStart.push_back(step);
                                flag = true;
                                break;
                            }
                        }
                        if (flag)
                            break;
                    }
                }
            }
        }
        return posStart;
    }

    // все возможные конечные точки хода
    vector<pair<string, string>> getPossibleSteps(int player)
    {
        vector<pair<string, string>> posSteps;
        vector<string> possibleStart = getPossibleStart(player);
        for (string from : possibleStart)
        {
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++)
                {
                    if (!(abs((from[1] - '0') - j) > 2 || abs((from[0] - '0') - i) > 2 || field.places[i][j] != 0))
                    {
                        string to = to_string(i) + to_string(j);
                        posSteps.push_back(pair<string, string>(from, to));
                    }
                }
        }
        return posSteps;
    }

    bool gameOver()
    {
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
        if (field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] != curPlayer)
        {
            cout << "\nНачальная точка не принадлежит "<< curPlayer << "-му игроку\n";
            return false;
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

    // игра двух людей
    //inf.playGame();

    // игра против бота
    inf.playWithBot();

    return 0;
}