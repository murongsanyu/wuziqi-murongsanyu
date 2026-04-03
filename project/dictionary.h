#ifndef __DICTIONARY_H
#define __DICTIONARY_H

// 棋盘大小的强类型枚举
// 使用 enum class 避免名称冲突，使用时需加作用域，例如 Board_Size::Large_Board
enum class Board_Size
{
    Small_Board = 11,      // 小棋盘 11x11
    Meduim_Board = 13,     // 中棋盘 13x13
    Large_Board = 15,      // 大棋盘 15x15
    SuperLarge_Board = 19  // 超大棋盘 19x19
};

// 指令的强类型枚举，用于主菜单选项，增强可读性
enum class Command
{
    Return_Menu = 0,    // 返回主菜单
    Start_Game1 = 1,    // 双人对战
    Start_Game2 = 2,    // 人机对战
    Load_Game = 3,      // 加载存档
    Help = 4,           // 游戏帮助
    Quit_Game = 5,      // 退出游戏
};

// 棋子状态（普通枚举，因为与棋盘交互频繁，使用简短的枚举值）
enum Chess_Type
{
    Empty = 0,   // 空位
    Black = 1,   // 黑棋（先手）
    White = 2    // 白棋
};

// 游戏状态
enum Game_Status
{
    On_Going,      // 游戏进行中
    Black_Win,     // 黑棋获胜
    White_Win,     // 白棋获胜
    Draw           // 平局（棋盘下满）
};

#endif