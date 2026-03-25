#ifndef __GAME_H
#define __GAME_H

#include "dictionary.h"
#include "board.h"

class Game
{
private:
    //Board board;
    Chess_Type current_player;  // 当前轮到谁下
    Game_Status status;        // 当前游戏状态

public:
    Game();
    ~Game();

    //清空控制台
    void clear (void);

    //游戏主菜单
    void menu (void);

    //游戏帮助
    void help (void);

    // 开始新游戏
    void startNewGame();

    // 显示当前棋盘
    void displayBoard() const;

    // 处理玩家输入（坐标），返回是否成功落子
    bool playerMove(int row, int col);

    // 悔棋操作（只能悔棋一步，且必须当前玩家同意？简化：允许双方悔棋一步，撤销上一步）
    bool undoMove();

    // 检查游戏是否结束，更新状态
    void checkGameOver();

    // 获取当前游戏状态
    Game_Status getStatus() const;

    // 获取当前玩家
    Chess_Type getCurrentPlayer() const;

    // 打印游戏提示
    void printPrompt() const;
};

#endif
