#ifndef __GAME_H
#define __GAME_H

#include "dictionary.h"
#include "board.h"

class Game
{
private:
    Board board;
    Chess_Type Current_Player;      // 当前轮到谁下
    Game_Status Status;        // 当前游戏状态
    int BoardSize;      // 当前棋盘大小

public:
    Game();
    ~Game();

    //清空控制台
    void clear (void);

    //游戏主菜单
    void menu (void);

    //供用户选择棋盘大小
    void select_boardsize (void);

    // 开始新游戏
    void start_newgame (void);

    // 显示当前棋盘
    void display_board() const;

    // 打印游戏提示
    void print_prompt() const;

    // 处理玩家输入（坐标），返回是否成功落子
    bool playerMove(int row, int col);

    // 悔棋操作（只能悔棋一步，且必须当前玩家同意？简化：允许双方悔棋一步，撤销上一步）
    bool undoMove();

    // 检查游戏是否结束，更新状态
    void checkGameOver();

    //游戏帮助
    void help (void);

    // 获取当前游戏状态
    Game_Status getStatus() const;

    // 获取当前玩家
    Chess_Type getCurrentPlayer() const;
};

#endif
