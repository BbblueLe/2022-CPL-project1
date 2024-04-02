#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define FILENAME "../grade.txt"
#define row 29
#define col 20
int grade = 0, max = 0;
int x = 29, y = 60;

struct Sa
{
    int data[row][col + 10];
    int color[row][col + 10];

} sa;

struct Block
{
    int new[4][4];

} block[10][4];

void Start();
void Hide();
void InitFace();
void color();
void DrawBlock(int a, int b, int x, int y);
void DrawSpace(int a, int b, int x, int y);
int Judge(int a, int b, int x, int y);
int JudgmentScore();
void Readfromfile();
void Writetofile();

void Readfromfile()
{
    FILE *fp = fopen(FILENAME, "r");
    if (!fp)
    {
        fp = fopen(FILENAME, "w");
        fwrite(&grade, sizeof(int), 1, fp);
    }
    fseek(fp, 0, SEEK_SET);
    fread(&max, sizeof(int), 1, fp);
    fclose(fp);
}

void Writetofile()
{
    FILE *fp = fopen(FILENAME, "w");
    if (!fp)
    {
        printf("保存失败\n");
        exit(0);
    }
    fwrite(&grade, sizeof(int), 1, fp);
    fclose(fp);
}

int main()
{
    SetConsoleOutputCP(65001); // 避免出现中文乱码
    char chCmd[32];
    srand((unsigned int)time(NULL));
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    system("title 俄罗斯方块");
    sprintf(chCmd, "mode con cols=%d lines=%d", y, x);
    system(chCmd);
    GetConsoleScreenBufferInfo(handle, &info);
    if (info.srWindow.Right + 1 >= 60 && info.srWindow.Bottom + 1 >= 29)
    {
        Hide();
        Readfromfile();
        InitFace();

        Start();
    }
    else
    {
        printf("终端窗口过小\n");
    }

    return 0;
}

void Hide()
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(handle, &info);
}

void CursorJump(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, pos);
}

void color(int Color)
{
    switch (Color)
    {
    case 0:
        Color = 2;
        break;
    case 1:
    case 2:
        Color = 12;
        break;
    case 3:
    case 4:
        Color = 10;
        break;
    case 5:
        Color = 5;
        break;
    case 6:
        Color = 6;
        break;
    default:
        Color = 7;
        break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color); // 颜色设置
}

void DrawBlock(int a, int b, int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (block[a][b].new[i][j] == 1)
            {
                CursorJump(2 * (x + j), y + i);
                printf("■");
            }
        }
    }
}

void DrawSpace(int a, int b, int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (block[a][b].new[i][j] == 1)
            {
                CursorJump(2 * (x + j), y + i);
                printf("  ");
            }
        }
    }
}

int Judge(int a, int b, int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((block[a][b].new[i][j] == 1) && sa.data[y + i][x + j] == 1)
                return 0;
        }
    }
    return 1;
}

int JudgmentScore()
{

    for (int i = row - 2; i > 4; i--)
    {
        int sum = 0;
        for (int j = 1; j < col - 1; j++)
        {
            sum += sa.data[i][j];
        }
        if (sum == 0)
            break;
        if (sum == col - 2)
        {
            grade += 100;
            color(7);
            CursorJump(2 * col + 4, row - 3);
            printf("当前分数：%d", grade);
            for (int j = 1; j < col - 1; j++)
            {
                sa.data[i][j] = 0;
                CursorJump(2 * j, i);
                printf("  ");
            }

            for (int m = i; m > 1; m--)
            {
                sum = 0;
                for (int n = 1; n < col - 1; n++)
                {
                    sum += sa.data[m - 1][n];
                    sa.data[m][n] = sa.data[m - 1][n];
                    sa.color[m][n] = sa.color[m - 1][n];
                    if (sa.data[m][n] == 1)
                    {
                        CursorJump(2 * n, m);
                        color(sa.color[m][n]);
                        printf("■");
                    }
                    else
                    {
                        CursorJump(2 * n, m);
                        printf("  ");
                    }
                }
                if (sum == 0)
                    return 1;
            }
        }
    }

    for (int j = 1; j < col - 1; j++)
    {
        if (sa.data[1][j] == 1)
        {
            Sleep(1000);
            system("cls");
            color(5);
            CursorJump(2 * (col / 3), row / 2 - 3);
            if (grade > max)
            {
                printf("恭喜你打破最高记录，最高记录更新为%d", grade);
                Writetofile();
            }
            else if (grade == max)
            {
                printf("与最高记录持平，加油再创佳绩", grade);
            }
            else
            {
                printf("继续加油，当前与最高记录相差%d", max - grade);
            }
            CursorJump(2 * (col / 3), row / 2);
            printf("游戏结束");
            while (1)
            {
                CursorJump(2 * (20 / 3), 29 / 2 + 3);
                printf("再来一局?(y/n):");
                char key;
                scanf("%c", &key);
                switch (key)
                {
                case 89:
                case 121:
                    system("cls");
                    main();
                    break;
                case 78:
                case 110:
                    exit(0);
                default:
                    printf("Error input!\n");
                    break;
                }
            }
        }
    }
    return 0;
}

void InitFace()
{
    int temp[4][4];
    color(7);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col + 10; j++)
        {
            if (j == 0 || j == col - 1 || j == col + 9)
            {
                sa.data[i][j] = 1; // 标记该位置有方块
                CursorJump(2 * j, i);
                printf("* ");
            }
            else if (i == row - 1)
            {
                sa.data[i][j] = 1; // 标记该位置有方块
                printf("* ");
            }
            else
                sa.data[i][j] = 0; // 标记该位置无方块
        }
    }

    CursorJump(2 * col, 1);
    printf("下一个方块：");

    CursorJump(2 * col + 4, row - 21);
    printf("当前操作为：");

    CursorJump(2 * col + 4, row - 19);
    printf("加速：↓");

    CursorJump(2 * col + 4, row - 17);
    printf("左移：←");

    CursorJump(2 * col + 4, row - 15);
    printf("右移：→");

    CursorJump(2 * col + 4, row - 13);
    printf("旋转：空格");

    CursorJump(2 * col + 4, row - 11);
    printf("暂停: R");

    CursorJump(2 * col + 4, row - 9);
    printf("退出: Esc");

    CursorJump(2 * col + 4, row - 7);
    printf("重新开始:Q");

    CursorJump(2 * col + 4, row - 5);
    printf("当前分数：%d", grade);

    CursorJump(2 * col + 4, row - 3);
    printf("最高纪录:%d", max);

    for (int i = 0; i <= 2; i++)
    {
        block[0][0].new[1][i] = 1;
    }
    block[0][0].new[2][1] = 1;

    for (int i = 1; i <= 3; i++)
    {

        block[1][0].new[i][1] = 1;
    }
    block[1][0].new[3][2] = 1;

    for (int i = 1; i <= 3; i++)
    {
        block[2][0].new[i][2] = 1;
    }
    block[2][0].new[3][1] = 1;

    for (int i = 0; i <= 1; i++)
    {
        block[3][0].new[1][i] = 1;
        block[3][0].new[2][i + 1] = 1;
    }

    for (int i = 0; i <= 1; i++)
    {

        block[4][0].new[1][i + 1] = 1;
        block[4][0].new[2][i] = 1;
    }

    for (int i = 0; i <= 1; i++)
    {

        block[5][0].new[1][i + 1] = 1;
        block[5][0].new[2][i + 1] = 1;
    }

    for (int i = 0; i <= 3; i++)
    {
        block[6][0].new[i][1] = 1;
    }

    for (int i = 0; i <= 2; i++)
    {
        block[7][0].new[i][1] = 1;
    }

    for (int i = 1; i <= 2; i++)
    {
        block[8][0].new[i][1] = 1;
    }
    block[8][0].new[2][2] = 1;

    // “五方块L ”形(瘦长)
    for (int i = 0; i <= 3; i++)
        block[9][0].new[i][1] = 1;
    block[9][0].new[3][2] = 1;

    // “五方块L型”(扁平)
    for (int i = 1; i <= 2; i++)
        block[10][0].new[i][0] = 1;
    for (int i = 0; i <= 3; i++)
        block[10][0].new[1][i] = 1;

    // “五方块L型”(均匀)
    for (int i = 1; i <= 2; i++)
        block[11][0].new[i][1] = 1;
    for (int i = 2; i <= 3; i++)
        block[11][0].new[2][i] = 1;

    for (int shape = 0; shape < 12; shape++) // 12种形状
    {
        for (int form = 0; form < 3; form++) // 4种形态(还需增加3种)
        {

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    temp[i][j] = block[shape][form].new[i][j];
                }
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    block[shape][form + 1].new[i][j] = temp[3 - j][i];
                }
            }
        }
    }
}

void Start()
{
    int a = rand() % 12, b = rand() % 4;
    while (1)
    {
        int t = 0;
        int nexts = rand() % 12, nextf = rand() % 4;
        int x = col / 2 - 5, y = 0;
        color(nexts);
        DrawBlock(nexts, nextf, col + 3, 3);
        while (1)
        {
            color(a);
            DrawBlock(a, b, x, y);
            if (t == 0)
            {
                t = 10000;
            }
            while (--t)
            {
                if (kbhit() != 0)
                    break;
            }
            if (t == 0)
            {
                if (Judge(a, b, x, y + 1) == 0)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (block[a][b].new[i][j] == 1)
                            {
                                sa.data[y + i][x + j] = 1;
                                sa.color[y + i][x + j] = a;
                            }
                        }
                    }
                    while (JudgmentScore())
                        ;
                    break;
                }
                else
                {
                    DrawSpace(a, b, x, y);
                    y++;
                }
            }
            else
            {
                char get = getch();
                switch (get)
                {
                case 80:
                    if (Judge(a, b, x, y + 1) == 1)
                    {
                        CursorJump(2 * col + 15, row - 21);
                        color(7);
                        printf("下");
                        DrawSpace(a, b, x, y);
                        y++;
                    }
                    break;
                case 75:
                    if (Judge(a, b, x - 1, y) == 1)
                    {
                        CursorJump(2 * col + 15, row - 21);
                        color(7);
                        printf("左");
                        DrawSpace(a, b, x, y);
                        x--;
                    }
                    break;
                case 77:
                    if (Judge(a, b, x + 1, y) == 1)
                    {
                        CursorJump(2 * col + 15, row - 21);
                        color(7);
                        printf("右");
                        DrawSpace(a, b, x, y);
                        x++;
                    }
                    break;
                case 32:
                    if (Judge(a, (b + 1) % 4, x, y + 1) == 1)
                    {
                        DrawSpace(a, b, x, y);
                        y++;
                        b = (b + 1) % 4;
                    }
                    break;
                case 27:
                    system("cls");
                    color(7);
                    CursorJump(col, row / 2);
                    printf("游戏结束");
                    CursorJump(col, row / 2 + 2);
                    exit(0);
                case 'r':
                case 'R':
                    system("pause>nul");
                    break;
                case 'q':
                case 'Q':
                    system("cls");
                    main();
                }
            }
        }
        a = nexts, b = nextf;
        DrawSpace(nexts, nextf, col + 3, 3);
    }
}
