#include <iostream>
#include <thread>
#include <windows.h>
#include "dictionary.h"
#include "game.h"
#include "board.h"
using namespace std;

int main (void)
{
    // 设置控制台输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 同时设置输入编码为 UTF-8
    SetConsoleCP(CP_UTF8);

    bool Running = true;   // 主循环标志
    int Command = 100;     // 存储用户输入的菜单选项

    while (Running)
    {
        Game game;                // 创建游戏对象（每轮主菜单重新创建，保证状态干净）
        game.clear();             // 清屏
        game.menu();              // 显示主菜单
        cin >> Command;           // 读取用户选择
        game.clear();             // 清屏

        switch (Command)
        {
            case (int)Command::Start_Game1 :   // 双人对战
            {
                game.select_boardsize();     // 让用户选择棋盘大小
                game.clear();
                game.start_newgame();        // 初始化游戏

                int row = 0, col = 0;
                bool Game_Running = true;
                game.display_board();        // 显示空棋盘

                while (Game_Running)
                {
                    game.print_prompt();     // 提示当前玩家
                    cin >> row >> col;       // 读取落子坐标

                    if (row == -1 && col == -1)   // 悔棋指令
                    {
                        if (game.undo_move())
                        {
                            game.clear();
                            cout << "悔棋成功！" << endl;
                            this_thread::sleep_for(chrono::seconds(1));     // 显示1秒
                            game.clear();
                            game.display_board();   // 显示悔棋后的棋盘
                        }
                        else
                        {
                            cout << "悔棋失败，没有可撤销的步骤。\n";
                        }
                        continue;   // 跳过后续落子处理
                    }
                    else if (row == -2 && col == -2)   // 退出对局指令
                    {
                        game.clear();
                        cout << "即将退回到游戏主菜单..." << endl;
                        this_thread::sleep_for(chrono::seconds(3));
                        Game_Running = false;
                        break;
                    }
                    else if (game.player_move(row, col))   // 尝试落子
                    {
                        game.clear();
                        cout << "落子成功！" << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        game.clear();
                        game.display_board();   // 显示落子后的棋盘

                        // 检查游戏是否结束
                        if (game.get_status() != Game_Status::On_Going)
                        {
                            game.print_prompt();   // 显示胜利/平局信息
                            Game_Running = false;
                        }
                    }
                    else
                    {
                        cout << "无效输入，请重新输入。" << endl;
                    }
                }
                break;
            }

            case (int)Command::Start_Game2 :   // 人机对战
            {
                game.select_aimode();      // 选择模式（会设置 isAIMode = true, AI执白
                game.clear();
                game.select_boardsize();
                game.clear();
                game.start_newgame();
                // 游戏循环需要修改：判断当前玩家是否是 AI，若是则自动落子
                int row, col;
                bool Game_Running = true;
                game.display_board();
                while (Game_Running)
                {
                    // 如果轮到 AI 下
                    if (game.getAIMode() && game.get_currentplayer() == game.getAIColor())
                    {
                        // 稍微延时，让玩家看到思考过程
                        this_thread::sleep_for(chrono::seconds(1));
                        if (game.ai_move())
                        {
                            game.clear();
                            cout << "AI 落子中..." << endl;
                            this_thread::sleep_for(chrono::seconds(1));
                            game.clear();
                            game.display_board();
                            if (game.get_status() != On_Going)
                            {
                                game.print_prompt();
                                Game_Running = false;
                            }
                        }
                        else
                        {
                            break;
                        }
                        continue; // 跳过玩家输入
                    }
                    // 玩家落子部分（与双人对战相同）
                    game.print_prompt();
                    cin >> row >> col;
                    if (row == -2 && col == -2)
                    {
                        game.clear();
                        cout << "即将退回到游戏主菜单..." << endl;
                        this_thread::sleep_for(chrono::seconds(3));
                        Game_Running = false;
                        break;
                    }
                    else if (game.player_move(row, col))
                    {
                        game.clear();
                        cout << "落子成功！" << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        game.clear();
                        game.display_board();
                        if (game.get_status() != On_Going)
                        {
                            game.print_prompt();
                            Game_Running = false;
                        }
                    }
                    else
                    {
                        cout << "无效落子，请重新输入。" << endl;
                    }
                }
                break;
            }

            case (int)Command::Help :          // 游戏帮助
            {
                game.help();                  // 显示帮助信息
                // 等待用户输入0返回主菜单
                while (true)
                {
                    cin >> Command;
                    if (Command == (int)Command::Return_Menu)
                    {
                        game.clear();
                        break;
                    }
                    else
                    {
                        cout << endl << "输入有误，请重新输入： ";
                    }
                }
                break;
            }

            case (int)Command::Quit_Game :     // 退出游戏
            {
                Running = false;
                cout << "谢谢游玩！" << endl;
                this_thread::sleep_for(chrono::seconds(3));
                break;
            }

            default:                          // 无效菜单选项
            {
                cout << endl << "输入有误，请重新输入： ";
                break;
            }
        }
    }

    return 0;
}