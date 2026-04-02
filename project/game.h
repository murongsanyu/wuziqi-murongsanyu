#ifndef __GAME_H
#define __GAME_H

#include "dictionary.h"
#include "board.h"
#include "ai.h"

class Game
{
private:
    Board board;                  // 棋盘对象
    Chess_Type Current_Player;    // 当前轮到谁下棋（黑或白）
    Game_Status Status;           // 当前游戏状态（进行中/胜利/平局）
    int BoardSize;                // 当前棋盘大小（边长）
    int Undo_Cnt[2];              // 悔棋次数记录：[0]黑棋，[1]白棋
    bool IsAIMode;                // 是否为人机模式
    Chess_Type AIColor;           // AI 执棋颜色（如果人机模式）

public:
    Game();
    ~Game();

    // 清空控制台（使用 ANSI 转义序列）
    void clear (void) const;

    // 主菜单界面
    void menu (void);

    // 让用户选择棋盘大小，结果存入 BoardSize
    void select_boardsize (void);

    // 开始新游戏（根据当前 BoardSize 重置棋盘、玩家、状态、悔棋次数）
    void start_newgame (void);

    // 让用户选择双人/人机，并设置 AI 颜色
    void select_aimode (void);

    // 显示当前棋盘（委托给 board.display()）
    void display_board (void) const;

    // 打印游戏提示（当前玩家或游戏结果）
    void print_prompt (void) const;

    // 处理玩家落子
    // 参数：行、列；返回是否成功落子
    bool player_move (int row, int col);

    // AI 移动
    // AI 自动落子，返回是否成功
    bool ai_move (void);

    // 悔棋操作
    // 如果符合条件（游戏进行中、有历史记录、悔棋次数未超限），撤销上一步并切换当前玩家
    // 返回是否成功悔棋
    bool undo_move (void);

    // 检查游戏是否结束，更新 Status
    void check_gameover (void);

    // 显示帮助信息
    void help (void);

    // 获取当前游戏状态
    Game_Status get_status (void) const;

    // 获取当前玩家
    Chess_Type get_currentplayer (void) const;

    bool getAIMode (void) const { return IsAIMode; }

    Chess_Type getAIColor (void) const { return AIColor; }
};

#endif