#include <iostream>
#include <thread>
#include "dictionary.h"
#include "game.h"
#include "board.h"
using namespace std;

int quit_flag = 0;      //暂时保留退出循环代码，防止调试过程中卡在死循环里

int main (void)
{
    bool Running = 1;
    int Command = 100;
    //加快输入输出速度
    //ios::sync_with_stdio(0);
    //cin.tie(0);
    //cout.tie(0);

    while (Running)         //程序循环开始
    {
        Game game;
        game.clear();
        game.menu();    
        cin >> Command;
        game.clear();
        switch (Command)
        {
            case (int)Command :: Start_Game:         //开始游戏
            {
                game.select_boardsize();
                game.clear();
                game.start_newgame();

                int row = 0;
                int col = 0;
                bool Game_Running = 1;
                game.start_newgame();
                game.display_board();

                while (Game_Running)
                {
                    game.print_prompt();
                    cin >> row >> col;
                    if (row == -1 && col == -1)
                    {

                    }
                    else if (row == -2 && col == -2)
                    {
                        game.clear();
                        cout << "即将退回到游戏主菜单..." << endl;
                        this_thread :: sleep_for(chrono :: seconds(2));
                        Game_Running = 0;
                        break;
                    }

                }
                break;
            }
            case (int)Command :: Help:               //游戏帮助
            {
                game.help();
                while (1)
                {
                    cin >> Command;
                    if (Command == (int)Command :: Return_Menu)
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
            case (int)Command :: Quit_Game:         //退出游戏
            {
                Running = 0;
                cout << "谢谢游玩！" << endl;
                break;
            }
            default:                                //用户指令输入有误
            {
                cout << endl << "输入有误，请重新输入： ";
                break;
            }
        }

        //暂时保留退出循环代码，防止调试过程中卡在死循环里
        quit_flag ++;
        if (quit_flag >= 20)
        {
            cout << "调试循坏次数过多！" << endl;
            break;
        }
    }

    return 0;
}