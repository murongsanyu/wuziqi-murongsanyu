
#include <vector>
#include <climits>
#include <random>
#include "ai.h"
using namespace std;

// ==================== 内部辅助函数 ====================

// 评估某个方向上的连续棋子数量（用于评分）
// 参数：board - 棋盘；row,col - 起始坐标；dx,dy - 方向向量；player - 要统计的棋子颜色
// 返回值：从该点沿方向（包括正反）连续同色棋子的总数（包括自身）
static int count_consecutive (const Board & board, int row, int col, int dx, int dy, Chess_Type player)
{
    int cnt = 1;
    // 正方向
    for (int step = 1 ; step < 5 ; step ++)
    {
        int nr = row + dx * step;
        int nc = col + dy * step;
        if (nr < 0 || nr >= board.get_size() || nc < 0 || nc >= board.get_size())
        {
            break;
        }
        if (board.get_chess(nr, nc) == player)
        {
            cnt ++;
        }
        else
        {
            break;
        }
    }
    // 反方向
    for (int step = 1 ; step < 5 ; step ++)
    {
        int nr = row - dx * step;
        int nc = col - dy * step;
        if (nr < 0 || nr >= board.get_size() || nc < 0 || nc >= board.get_size())
        {
            break;
        }
        if (board.get_chess(nr, nc) == player)
        {
            cnt++;
        }
        else
        {
            break;
        }
    }
    return cnt;
}

// 评估某个空位的分数（用于中等难度）
// 参数：board - 棋盘；row,col - 待评估位置；AiColor - AI颜色；PlayerColor - 玩家颜色
// 返回值：该位置的评分（整数），进攻分（AI自己形成的连珠） + 防守分（阻止玩家形成的连珠）*1.2
// 思路：分别计算在该位置放置AI棋子或玩家棋子时，四个方向上的连续长度，长度平方作为分数（鼓励长连），防守权重略高。
static int evaluate_position (const Board & board, int row, int col, Chess_Type AiColor, Chess_Type PlayerColor)
{
    int AiScore = 0;
    int PlayerScore = 0;
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int d = 0 ; d < 4 ; d ++)
    {
        int CntAI = count_consecutive(board, row, col, dirs[d][0], dirs[d][1], AiColor);
        int CntPlayer = count_consecutive(board, row, col, dirs[d][0], dirs[d][1], PlayerColor);
        // 长度越长，分数越高（指数增长，这里简化为平方）
        AiScore += CntAI * CntAI;
        PlayerScore += CntPlayer * CntPlayer;
    }
    // 进攻分（AI 自己） + 防守分（阻止玩家）* 1.2（防守权重稍高）
    return AiScore + static_cast <int> (PlayerScore * 1.2);
}

// 根据连续棋子数量返回棋型权重（用于困难难度）
// 参数：cnt - 连续棋子数量（1~5）
// 返回值：对应的评分权重（指数增长，突出长连的重要性）
static int get_weight (int cnt)
{
    switch (cnt)
    {
        case 5: return 100000;   // 连五，直接获胜
        case 4: return 10000;    // 活四或冲四，威胁极大
        case 3: return 500;      // 活三
        case 2: return 20;       // 活二
        default: return 1;       // 单子
    }
}

// 增强评分（困难难度使用）
// 与中等评分类似，但使用非线性的权重表，更符合五子棋棋型价值
static int evaluate_position_advanced (const Board & board, int row, int col, Chess_Type AiColor, Chess_Type PlayerColor)
{
    int AiScore = 0;
    int PlayerScore = 0;
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int d = 0 ; d < 4 ; d ++)
    {
        int CntAI = count_consecutive(board, row, col, dirs[d][0], dirs[d][1], AiColor);
        int CntPlayer = count_consecutive(board, row, col, dirs[d][0], dirs[d][1], PlayerColor);
        AiScore += get_weight(CntAI);
        PlayerScore += get_weight(CntPlayer);
    }
    // 进攻 + 防守（防守权重更高）
    return AiScore + static_cast <int> (PlayerScore * 1.2);
}

// ==================== AI 公有接口 ====================

// 根据难度获取最佳落子
// 思路：
//   - 简单：随机选择一个空位
//   - 中等/困难：遍历所有空位，调用评分函数，取最高分的位置
Step AI::get_bestmove (const Board & board, Chess_Type AiColor, AI_Difficulty difficulty)
{
    Chess_Type PlayerColor = (AiColor == Black) ? White : Black;        // 对手颜色
    int size = board.get_size();
    Step BestMove = {-1, -1, AiColor};

    // 简单难度：随机落子
    if (difficulty == AI_Difficulty::Easy)
    {
        vector <pair <int,int>> EmptyCells;
        for (int i = 0 ; i < size ; i++)
        {
            for (int j = 0 ; j < size ; j++)
            {
                if (board.get_chess(i, j) == Empty)
                {
                    EmptyCells.push_back({i, j});
                }
            }
        }

        if (EmptyCells.empty())
        {
            return BestMove;
        }

        // 使用 C++11 随机数库生成随机索引
        static random_device rd;                // 真随机数种子
        static mt19937 gen(rd());               // 梅森旋转引擎
        uniform_int_distribution <> dis(0, (int)EmptyCells.size() - 1);      // 均匀分布

        int idx = dis(gen);
        BestMove.row = EmptyCells[idx].first;
        BestMove.col = EmptyCells[idx].second;
        return BestMove;
    }

    // 中等/困难：遍历所有空位，使用不同评分函数，选择最高分
    int BestScore = INT_MIN;
    for (int i = 0 ; i < size ; i ++)
    {
        for (int j = 0 ; j < size ; j ++)
        {
            if (board.get_chess(i, j) != Empty)
            {
                continue;
            }

            int score = 0;
            if (difficulty == AI_Difficulty::Medium)
            {
                score = evaluate_position(board, i, j, AiColor, PlayerColor);
            }
            else if (difficulty == AI_Difficulty::Hard)
            {
                score = evaluate_position_advanced(board, i, j, AiColor, PlayerColor);
            }

            if (score > BestScore)
            {
                BestScore = score;
                BestMove.row = i;
                BestMove.col = j;
            }
        }
    }
    return BestMove;
}
