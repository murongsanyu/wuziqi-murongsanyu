
#ifndef GAME_H
#define GAME_H

#include <fstream>
#include "dictionary.h"
#include "board.h"
#include "ai.h"

// 游戏管理类：整合棋盘、玩家、AI、存档等功能
class Game
{
private:
    Board board;                  // 棋盘对象
    Chess_Type Current_Player;    // 当前轮到谁下棋（黑或白）
    Game_Status Status;           // 当前游戏状态（进行中/胜利/平局）
    int BoardSize;                // 当前棋盘大小（边长）
    int Undo_Cnt[2];              // 悔棋次数记录：[0]黑棋，[1]白棋（每方最多2次）
    bool IsAIMode;                // 是否为人机模式
    Chess_Type AIColor;           // AI 执棋颜色（如果人机模式）
    AI_Difficulty AIDifficulty;   // AI 难度（简单/中等/困难）

public:
    Game();                       // 构造函数：初始化默认值（大棋盘，双人模式）
    ~Game();

    // 清空控制台（跨平台：Windows用cls，Linux/Mac用clear）
    void clear (void) const;

    // 主菜单界面：打印选项并等待用户输入
    void menu (void);

    // 让用户选择棋盘大小，结果存入 BoardSize
    void select_boardsize (void);

    // 开始新游戏（根据当前 BoardSize 重置棋盘、玩家、状态、悔棋次数）
    void start_newgame (void);

    // 重置当前对局（清空棋盘，悔棋次数归零，黑棋先手，保留AI设置）
    void reset_game(void);

    // 选择人机对战模式，并设置 AI 颜色和难度（玩家执黑，AI执白）
    void select_aimode (void);

    // 显示当前棋盘（委托给 board.display()）
    void display_board (void) const;

    // 打印游戏提示（当前玩家或游戏结果，以及操作提示）
    void print_prompt (void) const;

    // 处理玩家落子
    // 参数：row, col - 坐标
    // 返回值：落子成功返回 true，否则 false（无效坐标或游戏已结束）
    bool player_move (int row, int col);

    // AI 移动：调用 AI 类获取最佳落子并执行
    // 返回值：成功落子返回 true，否则 false（无空位或不该AI下）
    bool ai_move (void);

    // 悔棋操作：撤销上一步，更新悔棋次数，切换当前玩家
    // 返回值：成功悔棋返回 true，否则 false（游戏已结束、无历史或次数用尽）
    bool undo_move (void);

    // 保存游戏到文件（二进制格式）
    // 参数：filename - 存档文件名（默认为 "save.dat"）
    // 返回值：成功返回 true，否则 false
    bool save_game (const std::string & filename = "save.dat") const;

    // 从文件加载游戏
    // 参数：filename - 存档文件名（默认为 "save.dat"）
    // 返回值：成功返回 true，否则 false
    bool load_game (const std::string & filename = "save.dat");

    // 检查游戏是否结束（黑胜/白胜/平局），更新 Status
    void check_gameover (void);

    // 显示帮助信息（游戏规则、操作说明等）
    void help (void);

    // 获取当前游戏状态
    Game_Status get_status (void) const;

    // 获取当前玩家
    Chess_Type get_currentplayer (void) const;

    // 获取是否为人机模式
    bool getAIMode (void) const;

    // 获取 AI 棋子颜色
    Chess_Type getAIColor (void) const;
};

#endif
