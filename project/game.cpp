
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include "game.h"
using namespace std;

// 构造函数：初始化默认大棋盘（15x15），悔棋次数清零，双人模式
Game::Game ()
{
    Current_Player = Black;
    Status = On_Going;
    BoardSize =  static_cast <int> (Board_Size::Large_Board);
    Undo_Cnt[0] = 0;
    Undo_Cnt[1] = 0;
    IsAIMode = false;      // 默认双人
    AIColor = White;       // 默认AI执白（仅在AI模式下有效）
    AIDifficulty = AI_Difficulty::Medium;      // 默认中等难度
}

Game::~Game () {}

// 清屏函数：根据操作系统调用不同命令
void Game::clear (void) const
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
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
    int choice = 0;
    cout << "请选择棋盘大小: " << endl;
    cout << " 1.小号棋盘(11*11)\n 2.中号棋盘(13*13)\n 3.大号棋盘(15*15)\n 4.超大号棋盘(19*19)\n";
    cout << "请输入选项: ";
    while (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        cout << "输入有误, 请重新输入正确的数字: ";
    }
    while (choice != 1 && choice != 2 && choice != 3 && choice != 4)
    {
        cout << "无效选择，请重新输入: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;     // 重置，避免未定义行为
        }
    }
    // 将用户选项转换为对应的 Board_Size 枚举值，并转为 int 存入 BoardSize
    if (choice == 1)
    {
        BoardSize = static_cast <int> (Board_Size::Small_Board);
    }
    else if (choice == 2)
    {
        BoardSize = static_cast <int> (Board_Size::Medium_Board);
    }
    else if (choice == 3)
    {
        BoardSize = static_cast <int> (Board_Size::Large_Board);
    }
    else if (choice == 4)
    {
        BoardSize = static_cast <int> (Board_Size::SuperLarge_Board);
    }
}

// 开始双人对战新游戏：重置棋盘、玩家、状态、悔棋次数
void Game::start_newgame (void)
{
    board.resize(BoardSize);      // 重新分配棋盘大小并清空
    Current_Player = Black;      // 黑棋先手
    Status = On_Going;          // 游戏进行中
    Undo_Cnt[0] = 0;        //悔棋次数归零
    Undo_Cnt[1] = 0;
}

// 重置当前对局（清空棋盘，悔棋次数归零，黑棋先手）
// 用于 -3 -3 命令，保留AI模式设置
void Game::reset_game(void)
{
    board.resize(BoardSize);
    Current_Player = Black;
    Status = On_Going;
    Undo_Cnt[0] = 0;
    Undo_Cnt[1] = 0;
    // 注意：IsAIMode、AIColor和AIDifficulty 保持不变
}

// 选择人机对战模式：设置 AI 执白，玩家执黑，并选择难度
void Game::select_aimode (void)
{
    IsAIMode = true;
    AIColor = White;     // AI 执白，玩家执黑
    Current_Player = Black;     // 玩家先手
    int choice;
    cout << "请选择 AI 难度：" << endl;
    cout << " 1. 简单" << endl;
    cout << " 2. 中等" << endl;
    cout << " 3. 困难" << endl;
    cout << "请输入选项数字: ";
    while (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        cout << "输入有误, 请重新输入正确的数字: ";
    }
    while (choice != 1 && choice != 2 && choice != 3)
    {
        cout << "无效选择, 请重新输入: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;     // 重置，避免未定义行为
        }
    }
    AIDifficulty = static_cast <AI_Difficulty> (choice);
}

// 显示棋盘（委托给 board 对象）
void Game::display_board (void) const
{
    board.display();
}

// 打印提示信息：根据游戏状态输出相应内容
// 如果游戏进行中，显示当前玩家和操作提示；如果游戏结束，显示结果并等待5秒后返回主菜单
void Game::print_prompt (void) const
{
    if (Status == On_Going)
    {
        if (IsAIMode)
        {
            cout << "当前玩家: " << (Current_Player == Black ? "黑棋(X)" : "白棋(O)") << endl;
            cout << "输入落子坐标 (行 列)，或输入 -2 -2 退出游戏，或 -3 -3 重新开始" << endl;
            cout << "请输入: ";
        }
        else
        {
            cout << "当前玩家: " << (Current_Player == Black ? "黑棋(X)" : "白棋(O)") << endl;
            cout << "黑棋剩余悔棋次数：" << 2 - Undo_Cnt[0] << "，白棋剩余悔棋次数：" << 2 - Undo_Cnt[1] << endl;
            cout << "输入落子坐标 (行 列)，或输入 -1 -1 悔棋，或输入 -2 -2 退出游戏，或 -3 -3 重新开始" << endl;
            cout << "请输入: ";
        }
    }
    else if (Status == Black_Win)
    {
        cout << "\n黑棋获胜! \n" << endl;
        cout << "\n即将跳转至主菜单..." << endl;
        this_thread :: sleep_for(chrono :: seconds(5));
        clear();
    }
    else if (Status == White_Win)
    {
        cout << "\n白棋获胜! \n" << endl;
        cout << "\n即将跳转至主菜单..." << endl;
        this_thread :: sleep_for(chrono :: seconds(5));
        clear();
    }
    else if (Status == Draw)
    {
        cout << "\n双方平局! \n" << endl;
        cout << "\n即将跳转至主菜单..." << endl;
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
        cout << "\n落子无效，请重新输入。\n\n";
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

// AI 移动：当游戏进行中且轮到AI时，调用AI类获取最佳落子并执行
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
    Step best = ai.get_bestmove(board, AIColor, AIDifficulty);
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

// 悔棋功能：需满足游戏进行中、有上一步、且该方悔棋次数未超限（双人模式每方最多2次）
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

// 保存游戏到文件（二进制格式）
// 保存内容：棋盘大小、所有棋子、当前玩家、游戏状态、悔棋次数、AI模式、AI颜色、AI难度
// 注意：历史栈不保存，加载后无法悔棋之前的步数
bool Game::save_game (const string & filename) const
{
    ofstream ofs(filename, ios::binary);
    if (!ofs.is_open())
    {
        return false;
    }

    // 保存棋盘大小
    int size = BoardSize;
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

    // 保存当前玩家、游戏状态、悔棋次数、AI模式、AI颜色、AI难度
    Chess_Type cur = Current_Player;
    Game_Status stat = Status;
    int undo0 = Undo_Cnt[0], undo1 = Undo_Cnt[1];
    bool AiMode = IsAIMode ? 1 : 0;
    Chess_Type AiCol = AIColor;
    AI_Difficulty AiDiff = AIDifficulty;

    ofs.write(reinterpret_cast <const char*> (&cur), sizeof(cur));
    ofs.write(reinterpret_cast <const char*> (&stat), sizeof(stat));
    ofs.write(reinterpret_cast <const char*> (&undo0), sizeof(undo0));
    ofs.write(reinterpret_cast <const char*> (&undo1), sizeof(undo1));
    ofs.write(reinterpret_cast <const char*> (&AiMode), sizeof(AiMode));
    ofs.write(reinterpret_cast <const char*> (&AiCol), sizeof(AiCol));
    ofs.write(reinterpret_cast <const char*> (&AiDiff), sizeof(AiDiff));

    ofs.close();
    return true;
}

// 从文件中加载游戏（二进制格式）
// 读取顺序与保存时一致，恢复棋盘状态和游戏参数
// 注意：历史栈被清空，悔棋将无法进行
bool Game::load_game (const string & filename)
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
    int cur, stat, undo0, undo1, AiMode, AiCol, AiDiff;
    ifs.read(reinterpret_cast <char*> (&cur), sizeof(cur));
    ifs.read(reinterpret_cast <char*> (&stat), sizeof(stat));
    ifs.read(reinterpret_cast <char*> (&undo0), sizeof(undo0));
    ifs.read(reinterpret_cast <char*> (&undo1), sizeof(undo1));
    ifs.read(reinterpret_cast <char*> (&AiMode), sizeof(AiMode));
    ifs.read(reinterpret_cast <char*> (&AiCol), sizeof(AiCol));
    ifs.read(reinterpret_cast<char*>(&AiDiff), sizeof(AiDiff));
 
    Current_Player = static_cast <Chess_Type> (cur);
    Status = static_cast <Game_Status> (stat);
    Undo_Cnt[0] = undo0;
    Undo_Cnt[1] = undo1;
    IsAIMode = (AiMode != 0);
    AIColor = static_cast <Chess_Type> (AiCol);
    AIDifficulty = static_cast <AI_Difficulty> (AiDiff);

    board.clear_lastmove();   // 清空最后一步高亮，避免显示错误

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

bool Game::getAIMode (void) const
{
    return IsAIMode;
}

Chess_Type Game::getAIColor (void) const
{
    return AIColor;
}

// 帮助信息：输出详细的游戏说明、操作方式、规则等
void Game::help (void)
{
    cout << "============== 游戏帮助 ==============" << endl;
    cout << "一、【游戏简介】\n";
    cout << "本程序是一款五子棋游戏，支持双人对战与人机对战。\n";
    cout << "先将五颗棋子连成一线的一方获胜。\n";
    cout << "二、【游戏模式】\n";
    cout << "- 双人对战：两名玩家轮流操作，黑棋先下，白棋后下。\n";
    cout << "- 人机对战：玩家执黑先下，AI 执白后下（AI 难度可自选）。\n";
    cout << "三、【操作说明】\n";
    cout << "- 落子：输入坐标（行 列），例如 “7 7” 表示在第7行第7列落子。\n";
    cout << "- 悔棋（仅双人对战）：输入 “-1 -1” 可撤销上一步落子，每方最多悔棋2次。\n";
    cout << "- 重置对局：输入 “-3 -3” 可清空棋盘并重新开始当前对局（保留模式与难度）。\n";
    cout << "- 退出对局：输入 “-2 -2” 会询问是否保存游戏，之后返回主菜单。\n";
    cout << "- 坐标范围：0 ～ 当前棋盘边长-1（如15x15棋盘为0~14）。\n";
    cout << "四、【棋盘显示说明】\n";
    cout << "- 空位显示为 “·”\n- 黑棋显示为 “X”\n- 白棋显示为 “O”\n";
    cout << "- 行号和列号标注在棋盘四周，最后一步棋子会高亮显示。\n";
    cout << "五、【五子棋基本规则】\n";
    cout << "1. 黑方先行，白方后行，交替落子。\n";
    cout << "2. 棋子落在网格交叉点上。\n";
    cout << "3. 在横、竖、斜任一方向形成连续五颗同色棋子的一方获胜。\n";
    cout << "4. 棋盘下满仍未分出胜负则为平局。\n";
    cout << "六、【存档说明】\n";
    cout << "- 游戏进行中可通过输入 “-2 -2” 并选择“是”来保存当前对局。\n";
    cout << "- 主菜单选择“加载存档”可恢复上次保存的游戏（注意：加载的存档无法立即悔棋）。\n";
    cout << "七、【开发信息】\n";
    cout << "版本: 1.14\n";
    cout << "课程设计小组: 南京理工大学2025级物理学院C++课程设计小组\n";
    cout << "参与成员(按姓氏拼音排序): 翁翊翾、叶云瑞、张诗翰、张煜佳\n";
    cout << "贡献说明及权利分配(经全体成员一致确认):\n";
    cout << "  - 核心代码编写及主要设计: 翁翊翾\n";
    cout << "  - 辅助测试及文档: 叶云瑞、张诗翰、张煜佳\n";
    cout << "  - 第一著作权人及作品负责人: 翁翊翾\n";
    cout << "  - 其他成员享有署名权及非商业性使用的知情权\n";
    cout << "最终修改日期: 2026年4月8日\n";
    cout << "========================================" << endl;
    cout << "输入0返回主菜单..." << endl;
    cout << "请输入：";
}
