
#include <iostream>
#include <iomanip>
#ifdef _WIN32
    #include <windows.h>
#endif
#include "board.h"
using namespace std;

// 默认构造函数，Size初始化为0，棋盘为空，历史栈空，最后一步无效
Board::Board ()
{
    Size = 0;
    LastMove = {-1, -1, Empty};
}

// 带大小的构造函数，调用 resize 分配空间并初始化
Board::Board (int Board_Size)
{
    Size = Board_Size;
    LastMove = Step{-1, -1, Empty};
    resize(Board_Size);
}

Board::~Board () {}

// 改变棋盘大小，重新分配二维 vector，全部置为 Empty，并清空历史栈和最后一步
void Board::resize (int New_Size)
{
    Size = New_Size;
    // assign 方法：将 grid 设为 New_Size 行，每行是包含 New_Size 个 Empty 的 vector
    grid.assign(Size, vector<Chess_Type>(Size, Empty));
    // 清空历史栈，悔棋功能将无法撤销之前的步数
    while (!history.empty())
    {
        history.pop();        
    }
    LastMove = {-1, -1, Empty};   // 清空高亮
}

// 落子函数：检查边界和空位，放置棋子，记录历史，更新最后一步
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
    // 记录最后一步（用于高亮显示）
    LastMove = {row, col, player};
    // 记录历史（用于悔棋）
    history.push(LastMove);
    
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

// 悔棋：弹出上一步，清除对应位置棋子，更新最后一步为新的栈顶（或无效）
bool Board::undo (void)
{
    if (history.empty())
    {
        return false;
    }
    Step last = history.top();
    history.pop();
    grid[last.row][last.col] = Empty;
    // 更新 lastMove 为新的栈顶，如果没有则清空
    if (!history.empty())
    {
        LastMove = history.top();
    }
    else
    {
        LastMove = {-1, -1, Empty};
    }
    return true;
}

// 判断棋盘是否已满（遍历所有格子）
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
// 思路：遍历每个棋子，从该点向四个方向（水平、垂直、主对角线、副对角线）延伸，
// 统计连续同色棋子数，若达到5个及以上则获胜。
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

// 显示棋盘（带行列号、边框、颜色、最后一步高亮）
// 思路：使用 Unicode 画线字符绘制表格，根据平台（Windows）设置控制台颜色，
// 最后一步的棋子使用不同背景色高亮。
void Board::display(void) const
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 保存默认颜色
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultColor = csbi.wAttributes;
#endif

    // 打印顶部列号（每个列号占3格，右对齐）
    cout << "  ";
    for (int c = 0 ; c < Size ; c ++)
    {
        cout << setw(4) << c;
        
    }
    cout << "\n";

    // 打印顶部边框
    cout << "   ┌";
    for (int c = 0 ; c < Size ; c ++)
    {
        cout << "───";
        if (c < Size - 1)
        {
            cout << "┬";
        }
    }
    cout << "┐\n";

    // 打印每一行
    for (int r = 0 ; r < Size ; r ++)
    {
        // 打印行号 + 竖线 + 棋子
        cout << setw(2) << r << " │";
        for (int c = 0 ; c < Size ; c ++)
        {
            bool IsLastMove = (r == LastMove.row && c == LastMove.col);
            if (grid[r][c] == Empty)
            {
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, 8);   // 灰色
                cout << " · ";
                SetConsoleTextAttribute(hConsole, defaultColor);
#else
                cout << " · ";
#endif
            }
            else if (grid[r][c] == Black)
            {
#ifdef _WIN32
                if (IsLastMove)
                {
                    SetConsoleTextAttribute(hConsole, 14 | 16);     // 黄字+蓝底
                }
                else
                {
                    SetConsoleTextAttribute(hConsole, 14);  // 亮黄
                }
                cout << " X ";
                SetConsoleTextAttribute(hConsole, defaultColor);
#else
                cout << " X ";
#endif
            }
            else if (grid[r][c] == White)
            {
#ifdef _WIN32
                if (IsLastMove)
                {
                    SetConsoleTextAttribute(hConsole, 11 | 16);     // 亮青+蓝底
                }
                else
                {
                    SetConsoleTextAttribute(hConsole, 11);  // 亮青
                }
                cout << " O ";
                SetConsoleTextAttribute(hConsole, defaultColor);
#else
                cout << " O ";
#endif
            }
            if (c < Size - 1)
            {
                cout << "│";
            }
        }
        cout << "│\n";

        // 如果不是最后一行，打印行间分隔线
        if (r < Size - 1)
        {
            cout << "   ├";
            for (int c = 0 ; c < Size ; c ++)
            {
                cout << "───";
                if (c < Size - 1)
                {
                    cout << "┼";
                }
            }
            cout << "┤\n";
        }
    }

    // 打印底部边框
    cout << "   └";
    for (int c = 0 ; c < Size ; c ++)
    {
        cout << "───";
        if (c < Size - 1)
        {
            cout << "┴";
        }
    }
    cout << "┘\n";
}

// 设置指定位置的棋子（仅供加载存档使用，不记录历史，不更新最后一步）
void Board::set_chess (int row, int col, Chess_Type player)
{
    if (row >= 0 && row < Size && col >= 0 && col < Size)
    {
       grid[row][col] = player;
    }
}

// 获取当前棋盘大小
int Board::get_size (void) const
{
    return Size;
}

// 清空最后一步记录（重置时调用）
void Board::clear_lastmove (void)
{
    LastMove = {-1, -1, Empty};
}

// 获取最后一步（可不用）
Step Board::get_lastmove (void) const
{
    return LastMove;
}
