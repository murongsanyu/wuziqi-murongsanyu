
#ifndef BOARD_H
#define BOARD_H

#include "dictionary.h"
#include <vector>
#include <stack>

// 用于记录每一步的落子信息
struct Step          // 使用 struct 表示纯数据类，所有成员公开
{
    int row;          // 行坐标
    int col;          // 列坐标
    Chess_Type player; // 落子方（黑或白）
};

// 棋盘类：管理棋盘状态、落子、悔棋、胜负判断等
class Board
{
private:
    // 动态棋盘：使用 vector 存储二维整数，0表示空，1黑，2白
    std::vector <std::vector <Chess_Type>> grid;
    // 历史栈：用于悔棋，存储每一步的落子信息
    std::stack <Step> history;
    int Size;          // 当前棋盘大小（边长）
    Step LastMove;      // 记录最后一步落子位置（用于高亮显示）

public:
    // 构造函数：默认构造，Size=0，棋盘为空
    Board ();
    
    // 带大小的构造函数：指定棋盘边长，内部调用 resize 分配空间
    // 参数：Board_Size - 棋盘边长（如 15）
    Board (int Board_Size);
    
    // 析构函数：释放资源（vector自动管理，无需额外操作）
    ~Board ();

    // 改变棋盘大小，重新分配空间，清空历史栈和最后一步记录
    // 参数：NewSize - 新的棋盘边长
    void resize (int NewSize);

    // 落子：在(row,col)处放置当前玩家的棋子
    // 参数：row, col - 坐标；player - 棋子颜色（黑或白）
    // 返回值：成功返回 true，失败（越界或已有棋子）返回 false
    bool place_piece (int row, int col, Chess_Type player);

    // 获取上一步信息（不弹出栈），用于悔棋前判断
    // 参数：step - 引用，用于返回栈顶信息
    // 返回值：成功返回 true，失败（栈空）返回 false
    bool get_laststep (Step & step) const;

    // 悔棋：弹出栈顶，清除对应位置的棋子
    // 返回值：成功返回 true，失败（栈空）返回 false
    bool undo (void);

    // 判断棋盘是否已满（平局）
    // 返回值：已满返回 true，否则 false
    bool is_full (void) const;

    // 判断指定玩家是否获胜（全局检查）
    // 从每个棋子向四个方向延伸，统计连续同色棋子数
    // 参数：player - 要检查的玩家棋子颜色
    // 返回值：该玩家获胜返回 true，否则 false
    bool check_win (Chess_Type player) const;

    // 获取指定位置棋子类型
    // 参数：row, col - 坐标
    // 返回值：Chess_Type 枚举值（Empty/Black/White），越界返回 Empty
    Chess_Type get_chess (int row, int col) const;

    // 在控制台打印棋盘（带行列号、边框、颜色高亮最后一步）
    void display (void) const;

    // 设置指定位置的棋子（仅供加载存档时使用，不记录历史）
    // 参数：row, col - 坐标；player - 棋子颜色
    void set_chess(int row, int col, Chess_Type player);

    // 获取当前棋盘大小
    // 返回值：边长（int）
    int get_size (void) const;

    // 清空最后一步记录（重置时调用）
    void clear_lastmove (void);
    
    // 获取最后一步（可不用）
    // 返回值：Step 结构体，包含最后一步坐标和玩家
    Step get_lastmove (void) const;
};

#endif
