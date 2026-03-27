#include "Board.h"
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

Board::Board() : Size(0) {}

Board::Board(int Board_Size)            //构造函数传入棋盘大小
{
    Size = Board_Size;
    resize(Board_Size);
}

Board::~Board() {}

void Board::init (void)
{
    for (auto &row : grid)
    {
        fill(row.begin(), row.end(), Empty);            //清空棋盘
    }
    while (!history.empty())            //初始化history栈
    {
        history.pop();
    }
}

void Board::resize(int New_Size)
{
    Size = New_Size;
    grid.assign(Size, vector<int>(Size, Empty));        //用新的棋盘大小替换原棋盘大小
    while (!history.empty())            //初始化history栈
    {
        history.pop();
    }
}

Chess_Type Board::get_chess (int row, int col) const
{
    if (row < 0 || row >= Size || col < 0 || col >= Size)           //排除错误的坐标索引
    {
        return Empty;
    }
    return static_cast <Chess_Type> (grid[row][col]);           //返回棋子坐标
}

void Board::display (void) const
{
    cout << "   ";
    for (int c = 0 ; c < Size ; c ++)
    {
        cout << setw(2) << c << " ";
    }
    cout << endl;

    for (int r = 0 ; r < Size ; r ++)
    {
        cout << setw(2) << r << " ";
        for (int c = 0 ; c < Size ; c ++)
        {
            if (grid[r][c] == Empty)
            {
                cout << " · ";
            }
                
            else if (grid[r][c] == Black)
            {
                cout << " X ";
            }
                
            else
            {
                cout << " O ";
            }
        }
        cout << endl;
    }
}
