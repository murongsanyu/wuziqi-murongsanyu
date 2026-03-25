#ifndef __DICTIONARY_H
#define __DICTIONARY_H

//棋盘大小的强类型枚举，使用时记得加作用域，记得强转一下数据类型，如：  int size = (int)Board_Size :: Large_Board;
enum class Board_Size
{
    Small_Board = 11,
    Meduim_Board = 13,
    Large_Board = 15,
    SuperLarge_Board = 19
};

//指令的强枚举类型
//采用枚举类型是为了加强程序可读性，在程序中写Start_Game大家都能看懂，但是写0的话大家都看不懂
enum class Command
{
    Return_Menu = 0,    //返回主菜单
    Start_Game = 1,     //开始游戏
    Help = 2,           //帮助
    Quit_Game = 3,      //退出游戏
    Move = 4,           //落子
    Give_Up = 5,        //认输
    
};

// 棋子状态
enum Chess_Type
{
    Empty = 0,
    Black = 1,   // 黑棋先手
    White = 2
};

//游戏状态
enum Game_Status
{
    On_Going,      // 进行中
    Black_Win,
    White_Win,
    Draw          // 平局（棋盘下满）
};

#endif
