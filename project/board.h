#ifndef __BOARD_H
#define __BOARD_H

#include "dictionary.h"
#include <vector>
#include <stack>

// 用于记录每一步的落子信息
class Step
{
public:
    int row;
    int col;
    Chess_Type player;   // 落子方
};

class Board
{
private:
    std::vector <std::vector <int>> grid;         // 动态棋盘
    std::stack <Step> history;            // 悔棋用的历史栈
    int Size;

public:
    Board();
    Board (int Board_Size);
    ~Board ();

    // 初始化棋盘（全部置空）
    // 可能用于用户在游戏途中想要重开游戏，暂时保留这个函数
    void init (void);

    // 改变棋盘大小并初始化
    void resize(int NewSize);

    // 在指定位置落子，成功返回true，失败（已有子或越界）返回false
    bool place_piece(int row, int col, Chess_Type player);

    // 获取上一步信息（不弹出栈）
    bool get_laststep(Step & step) const;

    // 悔棋一步：弹出上一步并清除棋盘上那个位置
    bool undo (void);

    // 检查当前棋盘是否满（平局）
    bool is_full (void) const;

    // 判断指定玩家是否获胜（从上一步落子位置检查效率更高，但这里简单实现全局检查）
    bool check_win(Chess_Type player) const;

    // 获取指定位置的棋子类型
    Chess_Type get_chess(int row, int col) const;

    // 显示棋盘
    void display (void) const;

    // 获取棋盘大小
    int get_size (void) const;
};

#endif
