#ifndef __AI_H
#define __AI_H

#include "board.h"

class AI
{
public:
    // 为指定玩家（AI 自己的颜色）计算最佳落子位置
    // board: 当前棋盘引用
    // aiColor: AI 使用的棋子颜色 (Black 或 White)
    // 返回 Step 结构体，包含最佳位置的行、列和玩家（固定为 aiColor）
    Step get_bestmove (const Board & board, Chess_Type AiColor);
};

#endif
