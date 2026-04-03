#include <iostream>
#include <thread>
#include <fstream>
#include "game.h"
using namespace std;

// 构造函数：初始化默认大棋盘（15x15），悔棋次数清零
Game::Game ()
{
    Current_Player = Black;
    Status = On_Going;
    board.resize((int)Board_Size :: Large_Board);
    Undo_Cnt[0] = 0;
    Undo_Cnt[1] = 0;
    IsAIMode = false;      // 默认双人
    AIColor = White;       // 默认AI执白
}

Game::~Game () {}

// 使用 ANSI 转义序列清屏：\033[2J 清屏，\033[1;1H 将光标移到左上角
// 使用system("cls")实现控制台清屏
void Game::clear (void) const
{
    //cout << "\033[2J\033[1;1H";
    system("cls");
}

// 显示主菜单
void Game::menu (void)
{
    cout << "========== 五子棋游戏 ==========" << endl;
    cout << "1. 双人对战" << endl;
    cout << "2. 人机对战" << endl;
    cout << "3. 加载存档" << endl;
    cout << "4. 游戏帮助" << endl;
    cout << "5. 退出游戏" << endl;
    cout << "=================================" << endl;
    cout << "请选择(输入对应的数字): ";
}

// 让用户选择棋盘大小，并存入 BoardSize 成员
void Game::select_boardsize (void)
{
    int choice;
    cout << "请选择棋盘大小: " << endl;
    cout << " 1.小号棋盘(11*11)\n 2.中号棋盘(13*13)\n 3.大号棋盘(15*15)\n 4.超大号棋盘(19*19)\n";
    cout << "请输入选项: ";
    cin >> choice;
    while (choice != 1 && choice != 2 && choice != 3 && choice != 4)
    {
        cout << endl << "无效选择，请重新输入: ";
        cin >> choice;
    }
    // 将用户选项转换为对应的 Board_Size 枚举值，并转为 int 存入 BoardSize
    if (choice == 1)
    {
        BoardSize = (int)Board_Size::Small_Board;
    }
    else if (choice == 2)
    {
        BoardSize = (int)Board_Size::Meduim_Board;
    }
    else if (choice == 3)
    {
        BoardSize = (int)Board_Size::Large_Board;
    }
    else if (choice == 4)
    {
        BoardSize = (int)Board_Size::SuperLarge_Board;
    }
}

// 开始新游戏：重置棋盘、玩家、状态、悔棋次数
void Game::start_newgame (void)
{
    board.resize(BoardSize);      // 重新分配棋盘大小并清空
    Current_Player = Black;      // 黑棋先手
    Status = On_Going;          // 游戏进行中
    Undo_Cnt[0] = 0;        //悔棋次数归零
    Undo_Cnt[1] = 0;
}

// 选择人机对战模式
void Game::select_aimode (void)
{
    IsAIMode = true;
    AIColor = White;     // AI 执白，玩家执黑
    Current_Player = Black;     // 玩家先手
}

// 显示棋盘（委托给 board 对象）
void Game::display_board (void) const
{
    board.display();
}

// 打印提示信息：根据游戏状态输出相应内容
void Game::print_prompt (void) const
{
    if (Status == On_Going)
    {
        if (IsAIMode)
        {
            cout << "当前玩家: " << (Current_Player == Black ? "黑棋(X)" : "白棋(O)") << endl;
            cout << "输入落子坐标 (行 列)，或输入 -2 -2 退出游戏" << endl;
            cout << "请输入: ";
        }
        else
        {
            cout << "当前玩家: " << (Current_Player == Black ? "黑棋(X)" : "白棋(O)") << endl;
            cout << "输入落子坐标 (行 列)，或输入 -1 -1 悔棋，或输入 -2 -2 退出游戏" << endl;
            cout << "请输入: ";
        }
        
    }
    else if (Status == Black_Win)
    {
        cout << "黑棋获胜！" << endl;
        cout << "即将跳转至主菜单..." << endl;
        this_thread :: sleep_for(chrono :: seconds(5));
        clear();
    }
    else if (Status == White_Win)
    {
        cout << "白棋获胜！" << endl;
        cout << "即将跳转至主菜单..." << endl;
        this_thread :: sleep_for(chrono :: seconds(5));
        clear();
    }
    else if (Status == Draw)
    {
        cout << "双方平局！" << endl;
        cout << "即将跳转至主菜单..." << endl;
        this_thread :: sleep_for(chrono :: seconds(5));
        clear();
    }
}

// 玩家落子：先检查游戏状态，再尝试在 board 上放置棋子
// 若成功，检查胜负并切换玩家；否则提示无效
bool Game::player_move (int row, int col)
{
    if (Status != On_Going)
    {
        cout << "落子失败，游戏已结束，请重新开始。" << endl;
        return false;
    }
    if (!board.place_piece(row, col, Current_Player))
    {
        cout << "落子无效，请重新输入。\n";
        return false;
    }
    // 落子成功后，检查胜负
    check_gameover();
    if (Status == On_Going)
    {
        // 若游戏未结束，切换当前玩家
        Current_Player = (Current_Player == Black) ? White : Black;
    }
    return true;
}

bool Game::ai_move (void)
{
    if (Status != On_Going) 
    {
        return false;
    }
    if (Current_Player != AIColor)
    {
        return false;    // 不该 AI 下
    }

    AI ai;
    Step best = ai.get_bestmove(board, AIColor);
    if (best.row == -1)
    {
        return false;   // 无空位
    }

    // 落子
    if (!board.place_piece(best.row, best.col, AIColor))
    {
        return false;
    }

    // 检查胜负
    check_gameover();
    if (Status == On_Going)
    {
        // 切换玩家
        Current_Player = (Current_Player == Black) ? White : Black;
    }
    return true;
}

// 悔棋功能：需满足游戏进行中、有上一步、且该方悔棋次数未超限
bool Game::undo_move (void)
{
    if (Status != On_Going)
    {
        cout << "游戏已结束，不能悔棋。" << endl;
        return false;
    }
    Step last;
    if (board.get_laststep(last))   // 先获取上一步信息（不弹出）
    {
        int id = (last.player == Black) ? 0 : 1;   // 确定是黑棋还是白棋
        if (Undo_Cnt[id] >= 2)              // 每方最多悔棋2次
        {
            cout << (last.player == Black ? "黑棋" : "白棋") << "悔棋次数已达上限，不能再次悔棋。" << endl;
            return false;
        }
        if (board.undo())           // 再弹出该步
        {
            Current_Player = last.player;       // 被撤销的那一方重新下棋
            Undo_Cnt[id] ++;            // 该方悔棋次数+1
            return true;
        }
    }
    return false;
}

// 保存游戏到文件
bool Game::save_game (const string& filename) const
{
    ofstream ofs(filename, ios::binary);
    if (!ofs.is_open())
    {
        return false;
    }

    // 保存棋盘大小
    int size = board.get_size();
    ofs.write(reinterpret_cast <const char*> (&size), sizeof(size));

    // 保存棋盘数据
    for (int i = 0 ; i < size ; i ++)
    {
        for (int j = 0 ; j < size ; j ++)
        {
            int value = board.get_chess(i, j);
            ofs.write(reinterpret_cast <const char*> (&value), sizeof(value));
        }
    }

    // 保存当前玩家、游戏状态、棋盘大小（再次确认）、悔棋次数、AI模式、AI颜色
    int cur = Current_Player;
    int stat = Status;
    int bs = BoardSize;
    int undo0 = Undo_Cnt[0], undo1 = Undo_Cnt[1];
    int AiMode = IsAIMode ? 1 : 0;
    int AiCol = AIColor;

    ofs.write(reinterpret_cast <const char*> (&cur), sizeof(cur));
    ofs.write(reinterpret_cast <const char*> (&stat), sizeof(stat));
    ofs.write(reinterpret_cast <const char*> (&bs), sizeof(bs));
    ofs.write(reinterpret_cast <const char*> (&undo0), sizeof(undo0));
    ofs.write(reinterpret_cast <const char*> (&undo1), sizeof(undo1));
    ofs.write(reinterpret_cast <const char*> (&AiMode), sizeof(AiMode));
    ofs.write(reinterpret_cast <const char*> (&AiCol), sizeof(AiCol));

    // 注意：历史栈不保存，加载后无法悔棋之前的步数
    ofs.close();
    return true;
}

// 从文件中加载游戏
bool Game::load_game (const string& filename)
{
    ifstream ifs(filename, ios::binary);
    if (!ifs.is_open())
    {
        return false;
    }

    int size;
    ifs.read(reinterpret_cast <char*> (&size), sizeof(size));
    if (size <= 0)
    {
        return false;
    }

    // 重新调整棋盘大小
    BoardSize = size;
    board.resize(size);

    // 读取棋盘数据
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int value;
            ifs.read(reinterpret_cast <char*> (&value), sizeof(value));
            if (value != Empty && value != Black && value != White)
            {
                return false;
            }
            board.set_chess(i, j, static_cast <Chess_Type> (value));
        }
    }

    // 读取其他状态
    int cur, stat, bs, undo0, undo1, AiMode, AiCol;
    ifs.read(reinterpret_cast <char*> (&cur), sizeof(cur));
    ifs.read(reinterpret_cast <char*> (&stat), sizeof(stat));
    ifs.read(reinterpret_cast <char*> (&bs), sizeof(bs));
    ifs.read(reinterpret_cast <char*> (&undo0), sizeof(undo0));
    ifs.read(reinterpret_cast <char*> (&undo1), sizeof(undo1));
    ifs.read(reinterpret_cast <char*> (&AiMode), sizeof(AiMode));
    ifs.read(reinterpret_cast <char*> (&AiCol), sizeof(AiCol));
 
    Current_Player = static_cast <Chess_Type> (cur);
    Status = static_cast <Game_Status> (stat);
    BoardSize = bs;
    Undo_Cnt[0] = undo0;
    Undo_Cnt[1] = undo1;
    IsAIMode = (AiMode != 0);
    AIColor = static_cast <Chess_Type> (AiCol);

    // 注意：历史栈被清空，悔棋将无法进行（或从零开始）
    ifs.close();
    return true;
}

// 更新游戏状态：检查黑胜、白胜、平局
void Game::check_gameover (void)
{
    if (board.check_win(Black))
    {
        Status = Black_Win;
    }
    else if (board.check_win(White))
    {
        Status = White_Win;
    }
    else if (board.is_full())
    {
        Status = Draw;
    }
    else
    {
        Status = On_Going;
    }
}

Game_Status Game::get_status (void) const
{
    return Status;
}

Chess_Type Game::get_currentplayer (void) const
{
    return Current_Player;
}

// 帮助信息：使用多个 cout 输出，清晰易读
void Game::help (void)
{
    cout << "============== 游戏帮助 ==============" << endl;
    cout << "一、【游戏简介】\n";
    cout << "本程序是一款双人五子棋游戏，两名玩家轮流在棋盘上落子，\n";
    cout << "先将五颗棋子连成一线的一方获胜。\n";
    cout << "二、【游戏模式】\n";
    cout << "- 双人对战：两名玩家轮流操作，黑棋先下，白棋后下。\n";
    cout << "- 人机对战: 玩家与AI轮流操作, 玩家作为黑棋先下, AI作为白棋后下。\n";
    cout << "三、【操作说明】\n";
    cout << "- 落子：输入棋盘坐标(行 列),例如“7 7”表示在第7行第7列落子。\n";
    cout << "- 悔棋：输入“-1 -1”可撤销上一步落子(每次只能悔棋一步)(人机对战模式无法悔棋)。\n";
    cout << "- 退出对局：输入“-2 -2”可返回主菜单。\n";
    cout << "- 所有坐标均从0开始,范围0~14(如15x15棋盘)。\n";
    cout << "四、【棋盘显示说明】\n";
    cout << "- 空位显示为“.” \n- 黑棋显示为“X”\n- 白棋显示为“O”\n";
    cout << "- 行号和列号标注在棋盘四周，方便定位。\n";
    cout << "五、【五子棋基本规则】\n";
    cout << "1. 黑方先行，白方后行，交替落子。\n";
    cout << "2. 棋子落在交叉点上（即网格交点）。\n";
    cout << "3. 先在横、竖、斜任一方向形成连续五颗同色棋子的一方获胜。\n";
    cout << "4. 如果棋盘下满仍未分出胜负，则判为平局。\n";
    cout << "5. 悔棋功能可撤销自己或对方的上一步落子（双方均可使用）。\n";
    cout << "六、【注意事项】\n";
    cout << "- 输入坐标时请用空格分隔,例如: 5 8\n";
    cout << "- 若输入无效坐标（越界或已有棋子），程序会提示重新输入。\n";
    cout << "- 游戏进行中随时可以退出。\n";
    cout << "七、【开发信息】\n";
    cout << "版本: 1.0\n";
    cout << "作者: 南京理工大学2025级物理学院C++课程设计小组:翁翊翾， \n";
    cout << "日期: 2026年3月\n";
    cout << "========================================" << endl;
    cout << "输入0返回主菜单..." << endl;
    cout << "请输入：";
}
