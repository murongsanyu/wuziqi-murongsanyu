
#ifndef AI_H
#define AI_H

#include "board.h"

// AI 类：提供不同难度下的最佳落子计算
class AI
{
public:
    // 为指定玩家（AI 自己的颜色）计算最佳落子位置
    // 参数：
    //   board: 当前棋盘引用（只读）
    //   AiColor: AI 使用的棋子颜色 (Black 或 White)
    //   difficulty: 难度等级 (Easy/Medium/Hard)
    // 返回值：
    //   Step 结构体，包含最佳位置的行、列和玩家（固定为 AiColor）
    //   若无空位，则返回 row = -1, col = -1 的 Step
    Step get_bestmove (const Board & board, Chess_Type AiColor, AI_Difficulty difficulty);
};

#endif
