
#include <iostream>
#include <thread>
#include <chrono>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <limits>
#include <random>
#include "dictionary.h"
#include "game.h"
#include "board.h"
#include "ai.h"
using namespace std;

// 游戏主循环函数声明（定义在文件末尾）
void game_run (Game & game);

// 主函数：显示主菜单，处理用户选择，启动游戏或退出
int main (void)
{
#ifdef _WIN32
    // 设置控制台输出编码为 UTF-8，以支持中文和特殊字符
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    bool Running = true;   // 主循环标志
    int Command = 100;     // 存储用户输入的菜单选项

    while (Running)
    {
        Game game;                // 创建游戏对象（每轮主菜单重新创建，保证状态干净）
        game.clear();             // 清屏
        game.menu();              // 显示主菜单
        while (!(cin >> Command))           // 读取用户选择，处理输入错误
        {
            cin.clear();
            cin.ignore(numeric_limits <streamsize> ::max(), '\n');
            cout << "输入无效，请重新输入数字: ";
        }
        game.clear();             // 清屏

        switch (Command)
        {
            case static_cast <int> (Command::Start_Game1) :    // 双人对战
            {
                game.select_boardsize();     // 让用户选择棋盘大小
                game.clear();
                game.start_newgame();        // 初始化游戏
                game_run(game);              // 进入游戏主循环
                break;
            }

            case static_cast <int> (Command::Start_Game2) :    // 人机对战
            {
                game.select_aimode();        // 选择难度（设置 AI 模式）
                game.clear();
                game.select_boardsize();     // 选择棋盘大小
                game.clear();
                game.start_newgame();        // 初始化游戏
                game_run(game);              // 进入游戏主循环
                break;
            }

            case static_cast <int> (Command::Load_Game) :       // 加载存档
            {
                if (game.load_game())
                {
                    cout << "加载成功！" << endl;
                    this_thread::sleep_for(chrono::seconds(2));
                    game.clear();
                    game_run(game);           // 进入游戏主循环
                }
                else
                {
                    cout << "加载失败，没有找到存档文件或文件损坏。" << endl;
                    this_thread::sleep_for(chrono::seconds(3));
                }
                break;
            }

            case static_cast <int> (Command::Help) :          // 游戏帮助
            {
                game.help();          // 显示帮助信息
                // 等待用户输入0返回主菜单
                while (true)
                {
                    if (!(cin >> Command))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "输入无效，请输入数字 0 返回主菜单: ";
                        continue;
                    }
                    if (Command == static_cast <int> (Command::Return_Menu))
                    {
                        game.clear();
                        break;
                    }
                    else
                    {
                        cout << "输入有误，请重新输入： ";
                    }
                }
                break;
            }

            case static_cast <int> (Command::Quit_Game) :     // 退出游戏
            {
                Running = false;
                cout << "谢谢游玩！" << endl;
                this_thread::sleep_for(chrono::seconds(3));
                break;
            }

            default:                          // 无效菜单选项
            {
                cout << "输入无效，请重新输入! ";
                this_thread::sleep_for(chrono::seconds(2));
                break;
            }
        }
    }

    return 0;
}

// 游戏主循环，适用于双人对战、人机对战、加载存档
// 参数：game - 已初始化好的游戏对象（包含棋盘、玩家、AI设置等）
// 思路：循环处理玩家输入和AI回合，直到游戏结束或玩家主动退出
void game_run (Game & game)
{
    bool GameRunning = true;
    game.display_board();

    while (GameRunning)
    {
        // 如果是人机模式且轮到 AI 下棋
        if (game.getAIMode() && game.get_currentplayer() == game.getAIColor())
        {
            this_thread::sleep_for(chrono::seconds(1));  // 模拟思考延迟
            if (game.ai_move())
            {
                game.clear();
                cout << "AI 落子中..." << endl;
                this_thread::sleep_for(chrono::seconds(1));
                game.clear();
                game.display_board();
                if (game.get_status() != On_Going)
                {
                    game.print_prompt();   // 显示获胜/平局信息，等待5秒后返回主菜单
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

        // 判断输入是否符合规范（两个整数）
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
            while (!(cin >> ch))
            {
                cin.clear();
                cin.ignore(numeric_limits <streamsize> ::max(), '\n');
                cout << "输入有误, 请重新输入: ";
            }
            while (ch != "是" && ch != "shi" && ch != "否" && ch != "fou")
            {
                cout << "输入有误, 请重新输入: ";
                cin >> ch;
            }
            if (ch == "是" || ch == "shi")
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
                cout << "\n悔棋失败, 没有可撤销的步骤。\n" << endl;
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
                game.print_prompt();   // 显示获胜/平局信息，等待5秒后返回主菜单
                GameRunning = false;
            }
        }
    }
}
