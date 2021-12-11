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
        // ��������� ���� ��������
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
    bool stepExist = true;

    Infection(Field f)
    {
        this->field = f;
    }

    // ���� ����� ����� ������
    void playGame()
    {
        while (true) {
p:          cout << "\n����� ����� �" << curPlayer << ":  ";
            cin >> startPlace >> endPlace; // ���

            placeToInt(startPlace, endPlace);

            if (!isCorrectMove(startPlace, endPlace))
                goto p;

            if (abs((startPlace[1] - '0') - (endPlace[1] - '0')) == 2 || abs((startPlace[0] - '0') - (endPlace[0] - '0')) == 2)
                field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] = 0;
            field.places[endPlace[1] - '0' - 1][endPlace[0] - '0' - 1] = curPlayer;

            field.printField();

            if (gameOver(field))
            {
                cout << "\n���� ��������! ";
                if (countFirst > countSecond)
                    cout << "������ ������� 1-�� �����!\n";
                else if (countFirst < countSecond)
                    cout << "������ ������� 2-�� �����!\n";
                else if (countFirst == countSecond)
                    cout << "������ ������� 2-�� �����!\n";
                else
                    cout << "�����!\n";
                return;
            }

            curPlayer = curPlayer == 1 ? 2 : 1;
        }
    }

    // ���� ���� �����
    void playWithBot()
    {
        // ��������� ����� ������, ��������� 1-�� ���
        //int r = rand() % (9 + 1);
        //curPlayer = r > 4 ? 2 : 1;

        while (true)
        {
p1:         curPlayer = 1;
            cout << "\n����� ����� �" << curPlayer << ":  ";
            cin >> startPlace >> endPlace; // ���

            placeToInt(startPlace, endPlace);

            if (!isCorrectMove(startPlace, endPlace))
                goto p1;

            if (abs((startPlace[1] - '0') - (endPlace[1] - '0')) == 2 || abs((startPlace[0] - '0') - (endPlace[0] - '0')) == 2)
                field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] = 0;
            field.places[endPlace[1] - '0' - 1][endPlace[0] - '0' - 1] = curPlayer;

            field.printField();

            if (gameOver(field))
            {
                cout << "\n���� ��������! ";
                if (countFirst > countSecond)
                    cout << "������ ������� 1-�� �����!\n";
                else if (countFirst < countSecond)
                    cout << "������ ������� 2-�� �����!\n";
                else
                    cout << "�����!\n";
                return;
            }


            curPlayer = 2;
            Field tempField;
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++)
                    tempField.places[i][j] = field.places[i][j];

            pair<int, pair<string, string>> ai_move = minimax(curPlayer, 4, tempField, -10, 10);

            if (ai_move.second.second == "" || ai_move.second.first == "")
                goto p1;

            field.places[ai_move.second.second[0] - '0'][ai_move.second.second[1] - '0'] = curPlayer;

            startPlace = ai_move.second.first;
            endPlace = ai_move.second.second;
            placeToString(startPlace, endPlace);

            cout << "\n����� ����� �" << curPlayer << ": " << startPlace << " " << endPlace << endl;
            //cout << "\n����� ����� �" << curPlayer << ": " << ai_move.second.first << " " << ai_move.second.second << endl;

            field.printField();

            if (gameOver(field))
            {
                cout << "\n���� ��������! ";
                if (countFirst > countSecond)
                    cout << "������ ������� 1-�� �����!\n";
                else if (countFirst < countSecond)
                    cout << "������ ������� 2-�� �����!\n";
                else
                    cout << "�����!\n";
                return;
            }

            //minimax(10, field, MIN, MAX);
        }
    }

    pair<int, pair<string, string>> minimax(int player, int depth, Field& tempField, int alpha, int beta, pair<string, string> curMove = make_pair("", ""))
    {
        //curPlayer = curPlayer == 1 ? 2 : 1;
        // ������ ��� 

        //pair<string, string> bestMove = make_pair("", "");
        // AI = -10, player = 10
        int bestScore = player == 2 ? -1000 : 1000;
        pair<string, string> bestMove;
        if (gameOver(tempField) || depth == 0)
            return make_pair(bestScore, curMove);


        // ��� ��������� ����
        vector<pair<string, string>> possibleSteps = getPossibleSteps(tempField, player);

        if (possibleSteps.size() == 0)
            stepExist = false;

        for (int i = 0; i < possibleSteps.size(); i++)
        {
            pair<string, string> curMove = make_pair(possibleSteps[i].first, possibleSteps[i].second);
            //field.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = curPlayer;
            if (abs((curMove.first[1] - '0') - (curMove.second[1] - '0')) == 2 || abs((curMove.first[0] - '0') - (curMove.second[0] - '0')) == 2)
                tempField.places[curMove.first[0] - '0'][curMove.first[1] - '0'] = 0;
            tempField.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = player;

            bestMove = possibleSteps[i];
            if (player == 2)
            {
                //curPlayer = 1;
                int score = minimax(1, depth - 1, tempField, alpha, beta, bestMove).first;

                if (bestScore < score)
                {
                    bestScore = score - depth * 10;
                    bestMove = curMove;

                    alpha = max(alpha, bestScore);
                    tempField.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = -1;
                    if (beta <= alpha)
                    {
                        break;
                    }
                }

            } 
            else
            {
                int score = minimax(2, depth - 1, tempField, alpha, beta, bestMove).first;

                if (bestScore > score)
                {
                    bestScore = score + depth * 10;
                    bestMove = curMove;

                    beta = min(beta, bestScore);
                    tempField.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = -1;
                    if (beta <= alpha)
                    {
                        break;
                    }
                }

            }
            tempField.places[curMove.second[0] - '0'][curMove.second[1] - '0'] = -1;
        }
        return make_pair(bestScore, bestMove);
    }

    // ��� ��������� ��������� ����� ����
    vector<string> getPossibleStart(Field tempField, int player)
    {
        vector<string> posStart;
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                bool flag = false;
                if (tempField.places[i][j] == player)
                {
                    for (int ii = 0; ii < 6; ii++)
                    {
                        for (int jj = 0; jj < 6; jj++)
                        {
                            if (!(abs(jj - j) > 2 || abs(ii - i) > 2 || tempField.places[ii][jj] != 0))
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

    // ��� ��������� �������� ����� ����
    vector<pair<string, string>> getPossibleSteps(Field tempField, int player)
    {
        vector<pair<string, string>> posSteps;
        vector<string> possibleStart = getPossibleStart(tempField, player);
        for (string from : possibleStart)
        {
            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 6; j++)
                {
                    if (!(abs((from[1] - '0') - j) > 2 || abs((from[0] - '0') - i) > 2 || tempField.places[i][j] != 0))
                    {
                        string to = to_string(i) + to_string(j);
                        posSteps.push_back(pair<string, string>(from, to));
                    }
                }
        }
        return posSteps;
    }

    bool gameOver(Field field)
    {
        countFirst = 0;
        countSecond = 0;
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
            {
                if (field.places[i][j] == 1)
                    countFirst++;
                if (field.places[i][j] == 2)
                    countSecond++;
                //if (stepExist)
                //    return false;
                if (field.places[i][j] == 0)
                    return false;
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

    void placeToString(string& startPlace, string& endPlace)
    {
        auto t = startPlace[0];
        startPlace[0] = startPlace[1];
        startPlace[1] = t;

        t = endPlace[0];
        endPlace[0] = endPlace[1];
        endPlace[1] = t;

        string intStartPlace = "", intEndPlace = "";
        switch (startPlace[0]) {
        case '0':
            intStartPlace += "a";
            break;
        case '1':
            intStartPlace += "b";
            break;
        case '2':
            intStartPlace += "c";
            break;
        case '3':
            intStartPlace += "d";
            break;
        case '4':
            intStartPlace += "e";
            break;
        default:
            intStartPlace += "f";
            break;
        }
        intStartPlace += startPlace[1];

        switch (endPlace[0]) {
        case '0':
            intEndPlace += "a";
            break;
        case '1':
            intEndPlace += "b";
            break;
        case '2':
            intEndPlace += "c";
            break;
        case '3':
            intEndPlace += "d";
            break;
        case '4':
            intEndPlace += "e";
            break;
        default:
            intEndPlace += "f";
            break;
        }
        intEndPlace += endPlace[1];

        //cout << intStartPlace << " " << intEndPlace << endl;
        startPlace = intStartPlace;
        endPlace = intEndPlace;
        startPlace[1]++;
        endPlace[1]++;
    }

    bool isCorrectMove(string s, string e)
    {
        if (!(s[0] == '1' || s[0] == '2' || s[0] == '3' || s[0] == '4' || s[0] == '5' || s[0] == '6')
            || !(s[1] == '1' || s[1] == '2' || s[1] == '3' || s[1] == '4' || s[1] == '5' || s[1] == '6'))
        {
            cout << "\n����������� ������� ��������� �����\n";
            return false;
        }

        if (!(e[0] == '1' || e[0] == '2' || e[0] == '3' || e[0] == '4' || e[0] == '5' || e[0] == '6')
            || !(e[1] == '1' || e[1] == '2' || e[1] == '3' || e[1] == '4' || e[1] == '5' || e[1] == '6'))
        {
            cout << "\n����������� ������� �������� �����\n";
            return false;
        }


        // �������� ��������� ����� ����
        if (field.places[startPlace[1] - '0' - 1][startPlace[0] - '0' - 1] != curPlayer)
        {
            cout << "\n��������� ����� �� ����������� "<< curPlayer << "-�� ������\n";
            return false;
        }

        // �������� �������� ����� ����
        if (abs((startPlace[1] - '0') - (endPlace[1] - '0')) > 2 || abs((startPlace[0] - '0') - (endPlace[0] - '0')) > 2 || field.places[endPlace[1] - '0' - 1][endPlace[0] - '0' - 1] != 0)
        {
            cout << "\n���������� ������� ����� ���\n";
            return false;
        }

        return true;
    }
};

int main()
{
    setlocale(LC_ALL, "rus");

    Field f;
    f.printField(); // ����� ���������� ����
        
    Infection inf(f);

    // ���� ���� �����
    //inf.playGame();

    // ���� ������ ����
    inf.playWithBot();

    return 0;
}