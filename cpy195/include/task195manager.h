#ifndef __TASK195MANAGER_H__
#define __TASK195MANAGER_H__
#include "datatype.h"
#include "xcommon.h"
#include "config.h"
#include "xlist.h"

#define Tz195

#ifndef Tz195
    #define MAX_GOURP           (128)
    #define MAX_GAME            (1)
    #define MAX_SYNC_MEMBER     (1)
    #define MAX_SYNC            (1)
#else
    #define MAX_GOURP           (32)
    #define MAX_GAME            (2)
    #define MAX_SYNC_MEMBER     (4)
    #define MAX_SYNC            (20)
#endif

//#define CONTINUE_DIE_TIME_INTERVAL  (90.0)
//#define CONTINUE_DIE_TIME_INTERVAL  (120.0)
#define CONTINUE_DIE_TIME_INTERVAL  (100.0)
#define CONTINUE_DIE_TIMES          (1)

struct sync_node
{
    struct list_head node;
    INT32 position;
    INT32 need_sync;
    INT32 max_count;
    INT32 member_count;
    INT32 hwnd[4];
};

struct game_node
{
    struct list_head node;
    INT32 node_id;
    INT32 hwnd;
    INT32 task_id;
    ROLE_E role;
    INT32 die_position;
    INT32 current_step;

    INT32 continue_die_count;
    CHAR last_die_time[32];//2020-02-22 20:00:00
};

struct group_node
{
    CRITICAL_SECTION lock;
    struct list_head node;
    INT32 group_id;
    INT32 max_game_count;
    INT32 current_game_count;
    INT32 group_step;
    INT32 group_step_auxi;
    INT32 border_stone_left;
    INT32 border_stone_right;
    struct list_head work_game_list;
    struct list_head free_game_list;
    struct list_head sync_game_list;
    struct sync_node wait_next;
    INT32 wait_next_flag;
};

struct manager
{
    CRITICAL_SECTION lock;
    INT32 max_group_count;
    INT32 current_group_count;
    struct list_head work_group_list;
    struct list_head free_group_list;
};

/* 内部调用 */
INT32 InitSync(INT32 max_sync_count, struct list_head* list);
void DestorySync(struct list_head* list);
INT32 InitGroup(struct group_node* group_node, INT32 group_id,
    INT32 max_game_count, INT32 max_sync_count, struct list_head* list);
void DestoryGroup(struct group_node* group_node);

/* 内部调用 */
INT32 GetGroup(struct group_node* group_node, INT32 hwnd, ROLE_E role);
INT32 PutGroup(struct group_node* group_node, INT32 hwnd);
void ShowSync(struct sync_node* sync_node);
void ShowGame(struct game_node* game_node);
void ShowGroup(struct group_node* group_node);
void ShowManager(void);;

/* 外部调用: 脚本初始化时调用   */
INT32 InitManager(INT32 max_group_count, INT32 max_game_count, INT32 max_sync_count);
void DestoryManager(void);


INT32 GetContextAuxi(INT32 hwnd);
INT32 GetContextAuxiTaskId(INT32 hwnd, INT32* ptr_task_id);
INT32 UpdateContextAuxi(INT32 hwnd, INT32 task_id);
INT32 PutContextAuxi(INT32 hwnd);

/* 外部调用: 脚本开启线程时调用 */
INT32 GetContext(INT32 group_id, INT32 hwnd, ROLE_E role);
INT32 PutContext(INT32 group_id, INT32 hwnd);

/* 外部调用: 线程更新进度时调用 */
INT32 UpdateGameTaskId(INT32 group_id, INT32 hwnd, INT32 task_id);
INT32 UpdateGamePosition(INT32 group_id, INT32 hwnd, INT32 position, INT32 force);
INT32 UpdateGamePositionDie(INT32 group_id, INT32 hwnd, INT32 die_position, INT32 force);
INT32 ResetGame(INT32 group_id, INT32 hwnd);

/* 外部调用: 复活重新进入副本时调用 */
INT32 GetGameTaskId(INT32 group_id, INT32 hwnd, INT32* ptr_task_id);
INT32 GetGamePosition(INT32 group_id, INT32 hwnd, INT32* ptr_position);
INT32 GetGamePositionDie(INT32 group_id, INT32 hwnd, INT32* ptr_die_position);

INT32 MarkLastDietime(INT32 group_id, INT32 hwnd, CHAR* die_time);
INT32 MarkWaitFlag(INT32 group_id, INT32 hwnd);
INT32 CheckWaitFlag(INT32 group_id);

/* 外部调用: 线程检查是否需要同步及到达同步地点时报告当前线程已到达 */
INT32 MarkReachFlag(INT32 group_id, INT32 hwnd, INT32 sync_position);
INT32 CheckSyncFlag(INT32 group_id, INT32 sync_position);
INT32 ClearMarkReachFlag(INT32 group_id, INT32 hwnd, INT32 sync_position);

/* 外部调用: 组更新进度时调用 */
INT32 UpdateGroupPosition(INT32 group_id, INT32 position, INT32 force);
INT32 UpdateGroupPositionAuxi(INT32 group_id, INT32 position, INT32 force);

INT32 GetGroupPosition(INT32 group_id, INT32* ptr_position);
INT32 GetGroupPositionAuxi(INT32 group_id, INT32* ptr_position);

/* 外部调用: 更新结界石状态时调用,force置为1 */
INT32 UpdateGroupBorderStoneLeft(INT32 group_id, INT32 border_stone, INT32 force);
INT32 UpdateGroupBorderStoneRight(INT32 group_id, INT32 border_stone, INT32 force);

/* 外部调用: 线程进行时结合自己的进度,组进度及大组进度时决定前进目标时调用 */
INT32 GetGroupBorderStoneLeft(INT32 group_id, INT32* ptr_border_stone);
INT32 GetGroupBorderStoneRight(INT32 group_id, INT32* ptr_border_stone);

/* 外部调用: 组重新进入副本时调用 */
INT32 ResetGroup(INT32 group_id);

int left_strtime(char* start_time, char* end_time, double* pescape);

#endif
