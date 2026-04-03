#include "Board.h"
#include <iostream>
#include <iomanip>
using namespace std;

// 默认构造函数，Size初始化为0
Board::Board () : Size(0) {}

// 带大小的构造函数，调用 resize 分配空间并初始化
Board::Board (int Board_Size)
{
    Size = Board_Size;
    resize(Board_Size);
}

Board::~Board () {}

// 改变棋盘大小，重新分配二维 vector，全部置为 Empty，并清空历史栈
void Board::resize (int New_Size)
{
    Size = New_Size;
    // assign 方法：将 grid 设为 New_Size 行，每行是包含 New_Size 个 Empty 的 vector
    grid.assign(Size, vector<int>(Size, Empty));
    while (!history.empty())
    {
        history.pop();        
    }
}

// 落子函数
bool Board::place_piece (int row, int col, Chess_Type player)
{
    // 边界检查
    if (row < 0 || row >= Size || col < 0 || col >= Size)
    {
        return false;
    }
    // 检查该位置是否为空
    if (grid[row][col] != Empty)
    {
        return false;
    }
    // 放置棋子
    grid[row][col] = player;
    // 记录历史（使用花括号初始化 Step 结构体）
    history.push(Step{row, col, player});
    return true;
}

// 获取上一步信息（不弹出）
bool Board::get_laststep (Step & step) const
{
    if (history.empty())
    {
        return false;
    }
    step = history.top();
    return true;
}

// 悔棋：弹出上一步，清除对应位置棋子
bool Board::undo (void)
{
    if (history.empty())
    {
        return false;
    }
    Step last = history.top();
    history.pop();
    grid[last.row][last.col] = Empty;
    return true;
}

// 判断棋盘是否已满
bool Board::is_full (void) const
{
    for (int i = 0 ; i < Size ; i ++)
    {
        for (int j = 0 ; j < Size ; j ++)
        {
            if (grid[i][j] == Empty)
            {
                return false;
            }
        }
    }
    return true;
}

// 全局检查某个玩家是否获胜
bool Board::check_win (Chess_Type player) const
{
    // 四个方向向量：水平(1,0)、垂直(0,1)、主对角线(1,1)、副对角线(1,-1)
    int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int i = 0 ; i < Size ; i ++)
    {
        for (int j = 0 ; j < Size ; j ++)
        {
            if (grid[i][j] != player)
            {
                continue; // 只检查当前玩家棋子
            }
            for (int d = 0 ; d < 4 ; d ++)
            {
                int cnt = 1;          // 当前棋子计数为1
                int dx = dirs[d][0];
                int dy = dirs[d][1];
                // 正方向延伸
                for (int step = 1 ; step < 5 ; step ++)
                {
                    int ni = i + dx * step;
                    int nj = j + dy * step;
                    if (ni < 0 || ni >= Size || nj < 0 || nj >= Size)
                    {
                        break;
                    }
                    if (grid[ni][nj] == player)
                    {
                        cnt ++;
                    }
                    else
                    {
                        break;
                    }
                }
                // 反方向延伸
                for (int step = 1 ; step < 5 ; step ++)
                {
                    int ni = i - dx * step;
                    int nj = j - dy * step;
                    if (ni < 0 || ni >= Size || nj < 0 || nj >= Size)
                    {
                        break;
                    }
                    if (grid[ni][nj] == player)
                    {
                        cnt ++;
                    }
                    else
                    {
                        break;
                    }
                }
                // 如果某个方向连续同色棋子数达到5或以上，获胜
                if (cnt >= 5)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// 获取指定位置的棋子类型，越界返回 Empty
Chess_Type Board::get_chess (int row, int col) const
{
    if (row < 0 || row >= Size || col < 0 || col >= Size)
    {
        return Empty;
    }
    return static_cast <Chess_Type> (grid[row][col]);
}

// 打印棋盘，带行列号，使用 setw 对齐
void Board::display (void) const
{
    // 打印列号（顶行）
    cout << "   ";
    for (int c = 0 ; c < Size ; c ++)
    {
        cout << setw(2) << c << " ";
    }
    cout << endl;

    // 打印每一行
    for (int r = 0 ; r < Size ; r ++)
    {
        cout << setw(2) << r << " ";   // 行号
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

void Board::set_chess (int row, int col, Chess_Type player)
{
    if (row >= 0 && row < Size && col >= 0 && col < Size)
    {
       grid[row][col] = player;
    }
}

int Board::get_size (void) const
{
    return Size;
}