#include "ai.h"
#include <vector>
#include <climits>

// 评估某个方向上的连续棋子数量（用于评分）
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

// 评估某个空位的分数（越高越有利）
static int evaluate_position (const Board & board, int row, int col, Chess_Type AiColor, Chess_Type PlayerColor)
{
    // 分别计算 AI 颜色和玩家颜色在该位置的分数
    int AiScore = 0;
    int PlayerScore = 0;
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int d = 0 ; d < 4 ; d ++)
    {
        int CntAI = count_consecutive(board, row, col, dirs[d][0], dirs[d][1], AiColor);
        int CntPlayer = count_consecutive(board, row, col, dirs[d][0], dirs[d][1], PlayerColor);
        // 长度越长，分数越高（指数增长，但这里简化）
        AiScore += CntAI * CntAI;
        PlayerScore += CntPlayer * CntPlayer;
    }
    // 进攻分（AI 自己） + 防守分（阻止玩家）* 1.2（防守权重稍高）
    return AiScore + (int)(PlayerScore * 1.2);
}

Step AI::get_bestmove (const Board & board, Chess_Type AiColor) {
    Chess_Type PlayerColor = (AiColor == Black) ? White : Black;
    int BestScore = -1;
    Step BestMove = {-1, -1, AiColor};
    int size = board.get_size();
    for (int i = 0 ; i < size ; i ++)
    {
        for (int j = 0 ; j < size ; j ++)
        {
            if (board.get_chess(i, j) != Empty)
            {
                continue;
            }
            int score = evaluate_position(board, i, j, AiColor, PlayerColor);
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