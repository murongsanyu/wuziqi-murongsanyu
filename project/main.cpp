#include <iostream>
#include <thread>
#include <windows.h>
#include "dictionary.h"
#include "game.h"
#include "board.h"
using namespace std;

void game_run (Game & game);

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
            case (int)Command::Start_Game1 :    // 双人对战
            {
                game.select_boardsize();     // 让用户选择棋盘大小
                game.clear();
                game.start_newgame();        // 初始化游戏

                game_run(game);        //统一调用循环

                break;
            }

            case (int)Command::Start_Game2 :    // 人机对战
            {
                game.select_aimode();      // 选择模式（会设置 isAIMode = true, AI执白
                game.clear();
                game.select_boardsize();
                game.clear();
                game.start_newgame();
                
                game_run(game);

                break;
            }

            case (int)Command::Load_Game:       // 加载存档
            {
                if (game.load_game())
                {
                    cout << "加载成功！" << endl;
                    this_thread::sleep_for(chrono::seconds(2));
                    game.clear();
                    game_run(game);       // 调用统一循环
                }
                else
                {
                    cout << "加载失败，没有找到存档文件或文件损坏。" << endl;
                    this_thread::sleep_for(chrono::seconds(3));
                }

                break;
            }

            case (int)Command::Help :          // 游戏帮助
            {
                game.help();          // 显示帮助信息
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

// 游戏主循环，适用于双人对战、人机对战、加载存档
void game_run (Game & game)
{
    bool GameRunning = true;
    game.display_board();

    while (GameRunning)
    {
        // 如果是人机模式且轮到 AI 下棋
        if (game.getAIMode() && game.get_currentplayer() == game.getAIColor())
        {
            this_thread::sleep_for(chrono::seconds(1));  // 模拟思考
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
                    GameRunning = false;
                }
            }
            else
            {
                break;  // 无空位，游戏结束
            }
            continue;
        }

        // 玩家回合
        game.print_prompt();
        int row, col;

        // 判断输入是否符合规范
        if (!(cin >> row >> col))
        {
            cin.clear();
            cin.ignore(numeric_limits <streamsize> ::max(), '\n');
            cout << "输入无效，请重新输入两个由空格隔开的整数！" << endl;
            continue;
        }

        // 退出对局（-2 -2）
        if (row == -2 && col == -2)
        {
            string ch;
            game.clear();
            cout << "是否保存游戏？" << endl;
            cout << "请输入 “是” 或 “否” :  ";
            cin >> ch;
            if (ch == "是")
            {
                if (game.save_game())
                {
                    game.clear();
                    cout << "保存成功！" << endl;
                }
                else
                {
                    cout << "保存失败！" << endl;
                }
            }
            cout << "即将退回到游戏主菜单..." << endl;
            this_thread::sleep_for(chrono::seconds(3));
            GameRunning = false;
            break;
        }

        // 重置对局（-3 -3）
        if (row == -3 && col == -3)
        {
            game.reset_game();
            game.clear();
            cout << "对局已重置！" << endl;
            this_thread::sleep_for(chrono::seconds(2));
            game.clear();
            game.display_board();
            continue;   // 跳过后续落子逻辑，重新进入循环
        }

        // 悔棋（仅双人模式支持，输入 -1 -1）
        if (!game.getAIMode() && row == -1 && col == -1)
        {
            if (game.undo_move())
            {
                game.clear();
                cout << "悔棋成功！" << endl;
                this_thread::sleep_for(chrono::seconds(1));
                game.clear();
                game.display_board();
            }
            else
            {
                cout << "悔棋失败，没有可撤销的步骤。" << endl;
            }
            continue;
        }

        // 正常落子
        if (game.player_move(row, col))
        {
            game.clear();
            cout << "落子成功！" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            game.clear();
            game.display_board();
            if (game.get_status() != On_Going)
            {
                game.print_prompt();
                GameRunning = false;
            }
        }
    }
}
