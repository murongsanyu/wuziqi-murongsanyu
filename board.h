#ifndef __BOARD_H
#define __BOARD_H

#include "dictionary.h"
#include <vector>
#include <stack>

// 用于记录每一步的落子信息
struct Step          // 使用 struct 表示纯数据类，所有成员公开
{
    int row;          // 行坐标
    int col;          // 列坐标
    Chess_Type player; // 落子方（黑或白）
};

class Board
{
private:
    // 动态棋盘：使用 vector 存储二维整数，0表示空，1黑，2白
    std::vector <std::vector <int>> grid;
    // 历史栈：用于悔棋，存储每一步的落子信息
    std::stack <Step> history;
    int Size;          // 当前棋盘大小（边长）

public:
    Board ();                      // 默认构造函数，Size=0
    Board (int Board_Size);        // 指定大小构造，会调用 resize
    ~Board ();

    // 改变棋盘大小，重新分配空间，清空历史栈
    void resize (int NewSize);

    // 落子：在(row,col)处放置当前玩家的棋子
    // 成功返回 true，失败（越界或已有棋子）返回 false
    bool place_piece (int row, int col, Chess_Type player);

    // 获取上一步信息（不弹出栈），用于悔棋前判断
    // 成功返回 true 并将 step 填充为栈顶信息
    bool get_laststep (Step & step) const;

    // 悔棋：弹出栈顶，清除对应位置的棋子
    // 成功返回 true，失败（栈空）返回 false
    bool undo (void);

    // 判断棋盘是否已满（平局）
    bool is_full (void) const;

    // 判断指定玩家是否获胜（全局检查）
    // 从每个棋子向四个方向延伸，统计连续同色棋子数
    bool check_win (Chess_Type player) const;

    // 获取指定位置棋子类型
    Chess_Type get_chess (int row, int col) const;

    // 在控制台打印棋盘，带行列号
    void display (void) const;

    // 获取当前棋盘大小
    int get_size (void) const;
};

#endif