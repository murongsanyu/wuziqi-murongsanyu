#include <iostream>
#include "dictionary.h"
#include "game.h"
#include "board.h"
using namespace std;

int quit_flag = 0;      //暂时保留退出循环代码，防止调试过程中卡在死循环里

int main (void)
{
    bool running = 1;
    int command = 100;
    //加快输入输出速度
    //ios::sync_with_stdio(0);
    //cin.tie(0);
    //cout.tie(0);

    while (running)         //程序循环开始
    {
        Game game;
        game.clear();
        game.menu();    
        cin >> command;
        game.clear();
        switch (command)
        {
            case (int)Command :: Start_Game:         //开始游戏
            {
                //代码
                break;
            }
            case (int)Command :: Help:               //游戏帮助
            {
                game.help();
                cin >> command;
                if (command == (int)Command :: Return_Menu)
                {
                    game.clear();
                    break;
                }
                else
                {
                    cout << endl << "输入有误，请重新输入： ";
                }
            }
            case (int)Command :: Quit_Game:         //退出游戏
            {
                running = 0;
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