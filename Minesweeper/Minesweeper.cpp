#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>

using namespace std;

int gm, row, col, mineNum, slct;

int A[6] = { 0,1,2,3,4,5 };
int S[3] = { 0,1,5 };
int O[5] = { 0,1,2,3,5 };
short keys[7][2] = {
            0x44,0,//D
            0x41,0,//A	
            0x53,0,//S
            0x57,0,//W
            0x20,0,//Space
            0x0D,0//Enter
};

vector<vector<string> >ground;
vector<vector<int> >mines;

struct point
{
    int x;
    int y;
};

point P = { 0, 0 };

template <int I>
short check(int(&)[I], short(*)[2] = keys);

void numGround(int& = slct);
int numInput(int& = slct, int(&)[5] = O);

void screenClear(HANDLE);

void print(int, vector<vector<string> > = ground, vector<vector<int> > = mines, point = P);
//0:正常游戏    1：雷区    2：胜利    3：失败

int main()
{
    COORD coord = { 0,0 };

    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );

    SetConsoleActiveScreenBuffer(hOutBuf);

    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOutput, &cci);
    SetConsoleCursorInfo(hOutBuf, &cci);

    DWORD bytes = 0;
    char data[9999];

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int flag = -2;

    while (1)
    {
        screenClear(hOutput);

        switch (flag)
        {
        case -2://欢迎界面
            cout << "//////////////////////////////" << '\n'
                << "//  Press Enter To Start    //        <- 欢迎界面" << '\n'
                << "//////////////////////////////" << endl;
            break;
        case -1://初始化
            cout << "你卡住了~~~~";

            break;
        case 0://行数
            cout << "输入场地行数：" << row << '\n' << endl;

            numGround();
            break;
        case 1://列数
            cout << "输入场地列数：" << col << '\n' << endl;

            numGround();
            break;
        case 2://雷数
            cout << "输入地雷数：" << mineNum << '\n' << endl;

            numGround();
            break;
        case 3://判断雷数
            if (mineNum > (row * col * 0.5))
            {
                mineNum = 0;

                flag = 4;
            }
            else
            {
                flag = 5;
            }
            continue;
        case 4://重设雷数
            cout << "地雷数过多，请重新输入" << endl;

            numGround();
            break;
        case 5://显示雷区
            cout << "显示雷区？(A/D)：" << '\n' << endl;

            if (slct)
            {
                cout << " -> 是 /    否";
            }
            else
            {
                cout << "    是 / -> 否";
            }
            break;
        case 6://生成
            cout << "生成中";

            break;
        case 7://主循环
            cout << "剩余雷数：" << mineNum << '\n' << '\n';

            print(0);

            cout << '\n' << "当前位置：" << ground[P.y][P.x] << endl;

            if (gm)
            {
                print(1);
            }

            break;
        case 8://输
            cout << "GAME OVER!!!!!" << '\n' << endl;

            print(3);

            cout << '\n' << "再来一局？(A/D)" << endl;
            if (slct)
            {
                cout << " -> 是 /   否";
            }
            else
            {
                cout << "   是 / -> 否";
            }

            break;
        case 9://赢
            cout << "Congratulations!!!" << '\n' << endl;

            print(2);

            cout << '\n' << "再来一局？(A/D)" << endl;
            if (slct)
            {
                cout << " -> 是 /   否";
            }
            else
            {
                cout << "   是 / -> 否";
            }

            break;
        }

        ReadConsoleOutputCharacterA(hOutput, data, 9999, coord, &bytes);
        WriteConsoleOutputCharacterA(hOutBuf, data, 9999, coord, &bytes);

        switch (flag)
        {
        case -2://欢迎界面
            if (check(S) == 0x0D)
            {
                flag = -1;
            }
            break;
        case -1://初始化
            srand((unsigned)time(0));
            row = 0;
            col = 0;
            mineNum = 0;

            flag = 0;

            break;
        case 0://行数
        {
            int i = numInput();

            screenClear(hOutput);

            if (i >= 0)
            {
                row = row * 10 + i;
            }
            else if (i == -1)
            {
                flag = 1;

            }
            else if (i == -2)
            {
                row /= 10;
            }
        }
        break;
        case 1://列数
        {
            int i = numInput();

            if (i >= 0)
            {
                col = col * 10 + i;
            }
            else if (i == -1)
            {
                flag = 2;
            }
            else if (i == -2)
            {
                col /= 10;
            }
        }
        break;
        case 2://雷数
        {
            int i = numInput();

            if (i >= 0)
            {
                mineNum = mineNum * 10 + i;
            }
            else if (i == -1)
            {
                flag = 3;
            }
            else if (i == -2)
            {
                mineNum /= 10;
            }
        }
        break;
        case 3:
            break;
        case 4://重设雷数
        {
            int i = numInput();

            if (i >= 0)
            {
                mineNum = mineNum * 10 + i;
            }
            else if (i == -1)
            {
                flag = 3;
            }
            else if (i == -2)
            {
                mineNum /= 10;
            }
        }
        break;
        case 5://显示雷区
            slct = 0;

            switch (check(S))
            {
            case 0x44:
                if (slct)
                {
                    slct--;
                }

                break;

            case 0x41:
                if (!slct)
                {
                    slct++;
                }

                break;

            case 0x0D:
                if (slct)
                {
                    gm = 1;
                }
                else
                {
                    gm = 0;
                }
                flag = 6;

                break;
            }
            break;
        case 6://生成
            ground.insert(ground.end(), row, vector<string>(col, "■"));
            mines.insert(mines.end(), row, vector<int>(col, 0));

            for (int i = 0; i < mineNum; i++) {
                if (mines[rand() % row][rand() % col])
                {
                    i--;
                }
                else
                {
                    mines[rand() % row][rand() % col] = 1;
                }
            }

            flag = 7;
            break;
        case 7://主循环
            switch (check(A))
            {
            case 0x44:
                if (P.x < col - 1)
                {
                    P.x++;
                }
                break;

            case 0x57:
                if (P.y > 0)
                {
                    P.y--;
                }
                break;

            case 0x41:
                if (P.x > 0)
                {
                    P.x--;
                }
                break;

            case 0x53:
                if (P.y < row - 1)
                {
                    P.y++;
                }
                break;

            case 0x0D:
            { //翻开
                if (mines[P.y][P.x] && (ground[P.y][P.x] == "■"))
                {
                    flag = 8;

                    continue;
                }
                else if (ground[P.y][P.x] == "■")
                {
                    ground[P.y][P.x] = "?";

                    while (1)
                    {
                        int mark = 0;

                        for (int m = 0; m < row; m++)
                        {
                            for (int n = 0; n < col; n++)
                            {
                                if (ground[m][n] == "?")
                                {
                                    mark = 1;

                                    int mine = 0;

                                    for (int i = m - 1; i < m + 2; i++)
                                    {
                                        if (i < 0 || i >= row)
                                        {
                                            continue;
                                        }
                                        else
                                        {
                                            for (int j = n - 1; j < n + 2; j++)
                                            {
                                                if (j < 0 || j >= col)
                                                {
                                                    continue;
                                                }
                                                else
                                                {
                                                    if (mines[i][j])
                                                    {
                                                        mine++;
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (mine)
                                    {
                                        ground[m][n] = '0' + mine;
                                    }
                                    else
                                    {
                                        ground[m][n] = "□";

                                        for (int i = m - 1; i < m + 2; i++)
                                        {
                                            if (i < 0 || i >= row)
                                            {
                                                continue;
                                            }
                                            else
                                            {
                                                for (int j = n - 1; j < n + 2; j++)
                                                {
                                                    if (j < 0 || j >= col)
                                                    {
                                                        continue;
                                                    }
                                                    else
                                                    {
                                                        if (ground[i][j] == "■")
                                                        {
                                                            ground[i][j] = '?';
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    continue;
                                }
                            }
                        }
                        if (!mark)
                        {
                            break;
                        }
                    }
                }

                int mark = 0;

                for (int i = 0; i < row; i++)
                {
                    for (int j = 0; j < col; j++)
                    {
                        if (ground[i][j] == "■" && !mines[i][j])
                        {
                            mark = 1;
                            break;
                        }
                    }
                    if (mark)
                    {
                        break;
                    }
                }
                if (!mark)
                {
                    flag = 9;
                }
                break;
            }
            case 0x20:
                if (ground[P.y][P.x] == "■")
                {
                    ground[P.y][P.x] = "※";

                    if (mines[P.y][P.x] == 1)
                    {
                        mineNum--;
                    }
                }
                else if (ground[P.y][P.x] == "※")
                {
                    ground[P.y][P.x] = "■";

                    if (mines[P.y][P.x] == 1)
                    {
                        mineNum++;
                    }
                }
                break;
            }

            break;
        case 8://输
            slct = 1;

            switch (check(S))
            {
            case 0x44:
                if (slct)
                {
                    slct--;
                }

                break;

            case 0x41:
                if (!slct)
                {
                    slct++;
                }

                break;

            case 0x0D:
                if (slct)
                {
                    flag = 0;
                }
                else
                {
                    exit(0);
                }
            }

            break;
        case 9://赢
            slct = 1;

            switch (check(S))
            {
            case 0x44:
                if (slct)
                {
                    slct--;
                }

                break;

            case 0x41:
                if (!slct)
                {
                    slct++;
                }

                break;

            case 0x0D:
                if (slct)
                {
                    flag = 0;
                }
                else
                {
                    exit(0);
                }
            }

            break;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    return 0;
}

template<int I>
short check(int(&num)[I], short(*keys)[2])
{
    while (1) {

        for (int i = 0; i < I; i++) {

            if ((GetKeyState((int)(*(*(keys + num[i])))) < 0) && (*(*(keys + num[i]) + 1) == 0)) {

                *(*(keys + num[i]) + 1) = 1;

                return *(*(keys + num[i]));

            }
            else if ((GetKeyState((int)(*(*(keys + num[i])))) >= 0) && (*(*(keys + num[i]) + 1) == 1)) {

                *(*(keys + num[i]) + 1) = 0;

            }

        }

    }

    return 0;
}

void print(int I, vector<vector<string> > V1, vector<vector<int> > V2, point P)
{
    int I1 = V1.size();
    int I2 = V1[0].size();

    switch (I)
    {
    case 0://游戏时界面
        for (int i = 0; i < 2 * I2 + 2; i++)
        {
            cout << '@';
        }

        cout << '\n';

        for (int i = 0; i < I1; i++) {

            cout << '@';

            for (int j = 0; j < I2; j++) {

                if (i == P.y && j == P.x) {

                    cout << "⊙";

                }
                else {

                    cout.width(2);
                    cout << V1[i][j];
                }

            }

            cout << '@';

            cout << '\n';

        }

        for (int i = 0; i < 2 * I2 + 2; i++) {

            cout << '@';

        }

        break;
    case 1://显示雷区
        for (int i = 0; i < I1; i++)
        {
            for (int j = 0; j < I2; j++)
            {
                cout.width(2);
                cout << V2[i][j];
            }
            cout << endl;
        }

        break;
    case 2://胜利
        for (int i = 0; i < I1; i++)
        {
            for (int j = 0; j < I2; j++)
            {
                if (V2[i][j])
                {
                    cout << "※";
                }
                else
                {
                    cout << "□";
                }
            }
            cout << endl;
        }

        break;
    case 3://失败
        for (int i = 0; i < I1; i++)
        {
            for (int j = 0; j < I2; j++)
            {
                if (V2[i][j])
                {
                    cout.width(2);
                    cout << "X";
                }
                else
                {
                    cout << V1[i][j];
                }
            }
            cout << endl;
        }

        break;
    }
}

int numInput(int& num, int(&arr)[5])
{
    switch (check(arr))
    {
    case 0x44:
        if (num == 0)
        {
            num--;
        }
        else if (num == -2)
        {
            num += 2;
        }
        else if ((!((num == 3) || (num == 6) || (num == 9))) && (num != -1))
        {
            num++;
        }
        return -3;

    case 0x41:
        if ((!((num == 1) || (num == 4) || (num == 7))) && (num > 0))
        {
            num--;
        }
        else if (num == -1)
        {
            num++;
        }
        else if (num == 0)
        {
            num -= 2;
        }
        return -3;

    case 0x53:
        if ((num > 0) && (num < 7))
        {
            num += 3;
        }
        else if (!((num == 7) || (num == -2)))
        {
            num = ((num == 8) || (num == 0)) ? 0 : -1;
        }
        else
        {
            num = -2;
        }

        return -3;

    case 0x57:
        if (num > 3)
        {
            num -= 3;
        }
        else if (num < 1)
        {
            num = (num == 0) ? 8 : ((num == -1) ? 9 : 7);
        }
        return -3;

    case 0x0D:

        if (num == -1)
        {
            num = 0;//复位
            return -1;
        }
        else
        {
            return num;
        }
    }
}

void numGround(int& num)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            if (num == (i * 3) + j)
            {
                cout << "  -> ";
            }
            else
            {
                cout << "     ";
            }

            cout << (i * 3) + j;
        }
        cout << endl;
    }

    switch (num)
    {
    case 0:
        cout << "     del-> 0     done";
        break;

    case -1:
        cout << "     del   0  -> done";
        break;

    case -2:
        cout << "  -> del   0     done";
        break;

    default:
        cout << "     del   0     done";
    }
}

void screenClear(HANDLE screen)
{
    char A[9999] = { '\0' };
    COORD C = { 0, 0 };
    DWORD D = 0;

    WriteConsoleOutputCharacterA(screen, A, 9999, C, &D);

    SetConsoleCursorPosition(screen, C);
}