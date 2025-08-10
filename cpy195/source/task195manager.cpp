#include <stdlib.h>
#include <stdio.h>
//#include <Windows.h>
#include <time.h>
#include "stdafx.h"
#include "datatype.h"
#include "xrrno.h"
#include "xlog.h"
#include "xlist.h"
#include "xcommon.h"
#include "config.h"
#include "task195manager.h"

struct auxi_detail
{
    struct list_head node;
    INT32 hwnd;
    INT32 task_id;
};

static struct manager g_Manager;
static struct list_head g_auxi_list;

static int trans_time(char* strtime, time_t* pt)
{
    struct tm tm;
    int count = 0;
    time_t t = 0;

    if (!strtime || !pt)
    {
        return ERRNO_INVALID;
    }

    memset(&tm, 0, sizeof(struct tm));
    count = sscanf(strtime, "%04d-%02d-%02d %02d:%02d:%02d",
        &tm.tm_year,
        &tm.tm_mon,
        &tm.tm_mday,
        &tm.tm_hour,
        &tm.tm_min,
        &tm.tm_sec);
    if (count != 6)
    {
        api_log_MsgDebug("sscanf time failed, strtime:%s", strtime);
        goto next;
    }

    api_log_MsgDebug("strtime:%s", strtime);
    api_log_MsgDebug("sscanf time success: %04d-%02d-%02d %02d:%02d:%02d", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    goto parse_ok;

next:
    count = sscanf(strtime, "%04d/%02d/%02d %02d:%02d:%02d",
        &tm.tm_year,
        &tm.tm_mon,
        &tm.tm_mday,
        &tm.tm_hour,
        &tm.tm_min,
        &tm.tm_sec);
    if (count != 6)
    {
        api_log_MsgDebug("sscanf time failed, strtime:%s", strtime);
        return ERRNO_INVALID;
    }

parse_ok:
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;

    t = mktime(&tm);
    if ((time_t)-1 == t)
    {
        return ERRNO_INVALID;
    }

    *pt = t;
    return ERRNO_SUCCESS;
}

static double left_time(time_t future, time_t past)
{
    return difftime(future, past);
}

int left_strtime(char* start_time, char* end_time, double* pescape)
{
    double escape = 0.0;
    time_t start = 0;
    time_t end = 0;
    int code = ERRNO_FAILED;
    
    if (!start_time || !end_time)
    {
        api_log_MsgDebug("left_strtime failed, invalid argument");
        return ERRNO_INVALID;
    }
    
    code = trans_time(start_time, &start);
    if (ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("left_strtime failed, invalid start time, start_time:%s", start_time);
        return ERRNO_INVALID;
    }
    
    code = trans_time(end_time, &end);
    if (ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("left_strtime failed, invalid end time, end_time:%s", end_time);
        return ERRNO_INVALID;
    }
    
    escape = left_time(end, start);
    if (pescape)
    {
        *pescape = escape;
    }

    return ERRNO_SUCCESS;
}


INT32 InitSync(INT32 max_sync_count, struct list_head* list)
{
    struct sync_node* sync_node = NULL;
    INT32 j = 0;
    struct sync_node* pos = NULL, *n = NULL;
    INT32 code = ERRNO_FAILED;
    INT32 sync_positions[] = {1, 4, 7, 17, 19};
    INT32 k = 0;
    
    for (j = 0; j < max_sync_count; ++j)
    {
        sync_node = (struct sync_node*)LUAMAN_ALLOC(sizeof(struct sync_node));
        if (NULL == sync_node)
        {
            api_log_MsgError("alloc sync nodes failed, memory short, max_sync_count:%d", max_sync_count);

            code = ERRNO_NOMEM;
            goto failed;
        }
        
        memset(sync_node, 0, sizeof(struct sync_node));

        INIT_LIST_HEAD(&sync_node->node);
        sync_node->position = j;
        
        /* 修改: 需要同步的成员个数 */
        sync_node->max_count = MAX_SYNC_MEMBER;
        
        /* 修改: 需要同步的地方 */
        for (k = 0; k < sizeof(sync_positions)/sizeof(sync_positions[0]); k++)
        {
            if (j == sync_positions[k])
            {
                sync_node->need_sync = 1;
            }
        }
        
        list_add_tail(&sync_node->node, list);
    }
    

    //LogDebug("alloc sync nodes success, max_sync_count:%d", max_sync_count);
    return ERRNO_SUCCESS;

failed:
    DestorySync(list);
    return code;
}

void DestorySync(struct list_head* list)
{
    struct sync_node* pos = NULL, *n = NULL;

    api_log_MsgDebug("destory sync");
    
    list_for_each_entry_safe(pos, n, list, node, struct sync_node)
    {
        ShowSync(pos);
        list_del(&pos->node);
        LUAMAN_FREE(pos);
        pos = NULL;
    }
    
    return;
}

INT32 InitGroup(struct group_node* group_node, INT32 group_id,
    INT32 max_game_count, INT32 max_sync_count, struct list_head* list)
{
    struct game_node* game_node = NULL;
    INT32 node_id = 0;
    INT32 sync_position = 0;
    struct sync_node* pos = NULL, *n = NULL;
    INT32 code = ERRNO_FAILED;
    
    memset(group_node, 0, sizeof(struct group_node));
    
    InitializeCriticalSection(&group_node->lock);
    INIT_LIST_HEAD(&group_node->node);

    group_node->group_id = group_id;
    group_node->max_game_count = max_game_count;
    group_node->current_game_count = 0;
    group_node->border_stone_left = 0;
    group_node->border_stone_right = 0;
    
    INIT_LIST_HEAD(&group_node->work_game_list);
    INIT_LIST_HEAD(&group_node->free_game_list);
    INIT_LIST_HEAD(&group_node->sync_game_list);

    for (node_id = 0; node_id < max_game_count; node_id++)
    {
        game_node = (struct game_node*)LUAMAN_ALLOC(sizeof(struct game_node));
        if (NULL == game_node)
        {
            api_log_MsgError("init group failed, memory short");
            code = ERRNO_NOMEM;
            goto failed;
        }
        
        memset(game_node, 0, sizeof(struct game_node));
        INIT_LIST_HEAD(&game_node->node);
        game_node->node_id = node_id;
        list_add_tail(&game_node->node, &group_node->free_game_list);
    }

    list_for_each_entry_safe(pos, n, list, node, struct sync_node)
    {
        list_del(&pos->node);
        list_add_tail(&pos->node, &group_node->sync_game_list);
    }

    //api_log_MsgDebug("init group success, group_id:%d, max_game_count:%d, max_sync_count:%d", group_id, max_game_count, max_sync_count);

    return ERRNO_SUCCESS;

failed:
    DestoryGroup(group_node);
    return code;
}

void DestoryGroup(struct group_node* group_node)
{
    struct game_node* pos = NULL, *n = NULL;
    INT32 group_id = group_node->group_id;

    api_log_MsgDebug("destory group, group_id:%d", group_id);

    EnterCriticalSection(&group_node->lock);

    api_log_MsgDebug("destory work game list");
    
    list_for_each_entry_safe(pos, n, &group_node->work_game_list, node, struct game_node)
    {
        ShowGame(pos);
        list_del(&pos->node);
        LUAMAN_FREE(pos);
        pos = NULL;
    }

    api_log_MsgDebug("destory free game list");
    
    list_for_each_entry_safe(pos, n, &group_node->free_game_list, node, struct game_node)
    {
        ShowGame(pos);
        list_del(&pos->node);
        LUAMAN_FREE(pos);
        pos = NULL;
    }

    DestorySync(&group_node->sync_game_list);

    group_node->current_game_count = 0;
    group_node->max_game_count = 0;
    group_node->group_step = 0;
    group_node->group_step_auxi = 0;
    group_node->border_stone_left = 0;
    group_node->border_stone_right = 0;
    
    LeaveCriticalSection(&group_node->lock);
    
    api_log_MsgDebug("destory group success, group_id:%d", group_id);
    return;
}

void ShowSync(struct sync_node* sync_node)
{
    api_log_MsgDebug("\t\tposition         : %d", sync_node->position);
    api_log_MsgDebug("\t\tneed_sync        : %d", sync_node->need_sync);
    api_log_MsgDebug("\t\tmax_count        : %d", sync_node->max_count);
    api_log_MsgDebug("\t\tmember_count     : %d", sync_node->member_count);
    api_log_MsgDebug("\t\thwnd[0]          : %d", sync_node->hwnd[0]);
    api_log_MsgDebug("\t\thwnd[1]          : %d", sync_node->hwnd[1]);
    api_log_MsgDebug("\t\thwnd[2]          : %d\n", sync_node->hwnd[2]);
    
    return;
}

void ShowGame(struct game_node* game_node)
{
    api_log_MsgDebug("\t\tnode_id          : %d", game_node->node_id);
    api_log_MsgDebug("\t\thwnd             : %d", game_node->hwnd);
    api_log_MsgDebug("\t\trole             : %d", game_node->role);
    api_log_MsgDebug("\t\tdie_position     : %d", game_node->die_position);
    api_log_MsgDebug("\t\tcurrent_step     : %d\n", game_node->current_step);
    return;
}

void ShowGroup(struct group_node* group_node)
{
    struct game_node* pos = NULL, *n = NULL;
    struct sync_node* spos = NULL, *sn = NULL;

    EnterCriticalSection(&group_node->lock);

    api_log_MsgDebug("\tgroup_id             : %d", group_node->group_id);
    api_log_MsgDebug("\tmax_game_count       : %d", group_node->max_game_count);
    api_log_MsgDebug("\tcurrent_game_count   : %d\n", group_node->current_game_count);
    
    list_for_each_entry_safe(spos, sn, &group_node->sync_game_list, node, struct sync_node)
    {
        ShowSync(spos);
    }

    list_for_each_entry_safe(pos, n, &group_node->work_game_list, node, struct game_node)
    {
        ShowGame(pos);
    }
    
    list_for_each_entry_safe(pos, n, &group_node->free_game_list, node, struct game_node)
    {
        ShowGame(pos);
    }
    
    LeaveCriticalSection(&group_node->lock);
    return;
}

void ShowManager(void)
{
    struct manager* manager = &g_Manager;
    struct group_node* pos = NULL, *n = NULL;

    //EnterCriticalSection(&manager->lock);

    api_log_MsgDebug("max_group_count        : %d", manager->max_group_count);
    api_log_MsgDebug("current_group_count    : %d", manager->current_group_count);
    
    list_for_each_entry_safe(pos, n, &manager->work_group_list, node, struct group_node)
    {
        ShowGroup(pos);
    }

    list_for_each_entry_safe(pos, n, &manager->free_group_list, node, struct group_node)
    {
        ShowGroup(pos);
    }

    //LeaveCriticalSection(&manager->lock);
    
    return;
}

INT32 InitManager(INT32 max_group_count, INT32 max_game_count, INT32 max_sync_count)
{
    struct manager* manager = &g_Manager;
    INT32 group_id = 0;
    struct group_node* group_node = NULL, *pos = NULL, *n = NULL;
    INT32 code = ERRNO_FAILED;
    struct sync_node* sync_node = NULL;
    struct list_head list;

    INIT_LIST_HEAD(&g_auxi_list);
    
    if (max_group_count <= 0 || max_group_count > MAX_GOURP)
    {
        api_log_MsgError("init manager failed, max_group_count:%d", max_group_count);
        return ERRNO_INVALID;
    }

    if (max_game_count <= 0 || max_game_count > MAX_GAME)
    {
        api_log_MsgError("init manager failed, max_game_count:%d", max_game_count);
        return ERRNO_INVALID;
    }

    if (max_sync_count <= 0 || max_sync_count > MAX_SYNC)
    {
        api_log_MsgError("init manager failed, max_sync_count:%d", max_sync_count);
        return ERRNO_INVALID;
    }
    
    memset(manager, 0, sizeof(struct manager));
    
    InitializeCriticalSection(&manager->lock);
    manager->max_group_count = max_group_count;
    manager->current_group_count = 0;
    INIT_LIST_HEAD(&manager->free_group_list);
    INIT_LIST_HEAD(&manager->work_group_list);

    for (group_id = 0; group_id < max_group_count; ++group_id)
    {
        group_node = (struct group_node*)LUAMAN_ALLOC(sizeof(struct group_node));
        if (NULL == group_node)
        {
            api_log_MsgError("init manager failed, memory short, max_group_count:%d", max_group_count);
            code = ERRNO_NOMEM;

            goto failed;
        }
        
        INIT_LIST_HEAD(&list);
        
        code = InitSync(max_sync_count, &list);
        if (ERRNO_SUCCESS != code)
        {
            LUAMAN_FREE(group_node);
            
            api_log_MsgError("init manager failed: alloc sync nodes failed, max_group_count:%d", max_group_count);
            goto failed;
        }
        
        code = InitGroup(group_node, group_id, max_game_count, max_sync_count, &list);
        if (ERRNO_SUCCESS != code)
        {
            LUAMAN_FREE(group_node);
            DestorySync(&list);
            
            api_log_MsgError("init group failed, group_id:%d, code:%d", group_id, code);
            goto failed;
        }

        list_add_tail(&group_node->node, &manager->free_group_list);
        
        //LogDebug("init group success, group_id:%d", group_id);

        group_node++;
    }

    api_log_MsgDebug("init manager success, max_group_count:%d, max_game_count:%d",
        max_group_count, max_game_count);
    
    return ERRNO_SUCCESS;

failed:
    DestoryManager();
    return code;
}

void DestoryManager(void)
{
    struct manager* manager = &g_Manager;
    struct group_node* pos = NULL, *n = NULL;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(pos, n, &manager->free_group_list, node, struct group_node)
    {
        list_del(&pos->node);
        DestoryGroup(pos);
        LUAMAN_FREE(pos);
        pos = NULL;
    }

    list_for_each_entry_safe(pos, n, &manager->work_group_list, node, struct group_node)
    {
        list_del(&pos->node);
        DestoryGroup(pos);
        LUAMAN_FREE(pos);
        pos = NULL;
    }

    manager->max_group_count = 0;
    manager->current_group_count = 0;
    //LeaveCriticalSection(&manager->lock);

    api_log_MsgDebug("destory manager success");
    return;
}

INT32 GetGroup(struct group_node* group_node, INT32 hwnd, ROLE_E role)
{
    struct game_node* node = NULL, *pos = NULL, *n = NULL;
    INT32 code = ERRNO_FAILED;
    
    EnterCriticalSection(&group_node->lock);
    list_for_each_entry_safe(pos, n, &group_node->work_game_list, node, struct game_node)
    {
        /* master只能有一个 */
        if (pos->hwnd == hwnd)
        {
            code = ERRNO_EXIST;

            api_log_MsgWarn("hwnd aready allocated, group_id:%d, node_id:%d, hwnd:%d, role:%d, pos->role:%d",
                group_node->group_id, pos->node_id, hwnd, role, pos->role);
            goto failed;
        }

        if (ROLE_M == role && pos->role == role)
        {
            code = ERRNO_CONFLICT;

            api_log_MsgWarn("master aready allocated, group_id:%d, node_id:%d, hwnd:%d, role:%d, pos->role:%d",
                group_node->group_id, pos->node_id, hwnd, role, pos->role);
            goto failed;
        }
    }

    if (list_empty(&group_node->free_game_list))
    {
        api_log_MsgDebug("no free game, group_id:%d, hwnd:%d, role:%d",
            group_node->group_id, hwnd, role);
        
        code = ERRNO_NOSPACE;
        goto failed;
    }
    
    node = list_first_entry(&group_node->free_game_list, struct game_node, node);
    if (NULL == node)
    {
        api_log_MsgWarn("alloc hwnd failed: no space, group_id:%d, hwnd:%d, role:%d",
            group_node->group_id, hwnd, role);
        
        code = ERRNO_NOSPACE;
        goto failed;
    }

    list_del(&node->node);
    node->current_step = 0;
    node->hwnd = hwnd;
    node->role = role;
    node->task_id = 0;
    node->die_position = 0;

    list_add_tail(&node->node, &group_node->work_game_list);
    group_node->current_game_count = group_node->current_game_count + 1;

    code = ERRNO_SUCCESS;

    api_log_MsgDebug("alloc hwnd success, group_id:%d, hwnd:%d, role:%d",
        group_node->group_id, hwnd, role);
    
failed:
    LeaveCriticalSection(&group_node->lock);
    return code;
}

INT32 PutGroup(struct group_node* group_node, INT32 hwnd)
{
    struct game_node* node = NULL, *pos = NULL, *n = NULL;
    INT32 code = ERRNO_FAILED;
    
    EnterCriticalSection(&group_node->lock);
    list_for_each_entry_safe(pos, n, &group_node->work_game_list, node, struct game_node)
    {
        if (pos->hwnd == hwnd)
        {
            list_del(&pos->node);

            api_log_MsgDebug("put group success, node_id:%d, hwnd:%d, role:%d",
                pos->node_id, pos->hwnd, pos->role);
            
            pos->current_step = 0;
            pos->die_position = 0;
            pos->hwnd = 0;
            pos->role = ROLE_U;
            pos->task_id = 0;
            group_node->current_game_count = group_node->current_game_count - 1;
            
            list_add_tail(&pos->node, &group_node->free_game_list);
            LeaveCriticalSection(&group_node->lock);
            
            return ERRNO_SUCCESS;
        }
    }
    LeaveCriticalSection(&group_node->lock);

    api_log_MsgDebug("put group failed: hwnd not exist, hwnd:%d", hwnd);
    return ERRNO_NOT_EXIST;
}

INT32 GetContext(INT32 group_id, INT32 hwnd, ROLE_E role)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    INT32 code = ERRNO_FAILED;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            gnode = gpos;
            
            api_log_MsgDebug("group allocated, group_id:%d, hwnd:%d, role:%d",
                group_id, hwnd, role);
            goto proc;
        }
    }

    list_for_each_entry_safe(gpos, gn, &manager->free_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            gnode = gpos;
            list_del(&gpos->node);
            list_add_tail(&gnode->node, &manager->work_group_list);
            manager->current_group_count = manager->current_group_count + 1;

            api_log_MsgDebug("alloc new group, group_id:%d, hwnd:%d, role:%d",
                group_id, hwnd, role);
            goto proc;
        }
    }

    //LeaveCriticalSection(&manager->lock);

    api_log_MsgDebug("alloc failed: group not exist, group_id:%d, hwnd:%d, role:%d",
        group_id, hwnd, role);
    
    return ERRNO_NOT_EXIST;
    
proc:
    code = GetGroup(gnode, hwnd, role);
    if (ERRNO_SUCCESS != code)
    {
        //LeaveCriticalSection(&manager->lock);
        
        api_log_MsgError("get context failed, hwnd:%d, role:%d, code:%d",
            hwnd, role, code);
        return code;
    }
    
    //LeaveCriticalSection(&manager->lock);

    api_log_MsgDebug("get context success, hwnd:%d, role:%d",
        hwnd, role);
    return ERRNO_SUCCESS;
}

INT32 PutContext(INT32 group_id, INT32 hwnd)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    INT32 code = ERRNO_FAILED;

    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        //if (gpos->group_id == group_id)
        {
            code = PutGroup(gpos, hwnd);
            //LeaveCriticalSection(&manager->lock);
            if (ERRNO_SUCCESS == code)
            {
                api_log_MsgDebug("put context completed: group_id:%d, hwnd:%d, code:%d",
                    group_id, hwnd, code);
                return code;
            }
        }
    }
    //LeaveCriticalSection(&manager->lock);

    api_log_MsgWarn("put context failed: not exist, group_id:%d, hwnd:%d",
        group_id, hwnd);
    return ERRNO_NOT_EXIST;
}

INT32 UpdateGameTaskId(INT32 group_id, INT32 hwnd, INT32 task_id)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    struct game_node* mpos = NULL, *mn = NULL;
    INT32 last_task_id = 0;
    INT32 code = ERRNO_NOT_EXIST;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(mpos, mn, &gpos->work_game_list, node, struct game_node)
            {
                if (mpos->hwnd == hwnd)
                {
                    last_task_id = mpos->task_id;
                    mpos->task_id = task_id;

                    code = ERRNO_SUCCESS;
                    LeaveCriticalSection(&gpos->lock);

                    api_log_MsgDebug("update game taskid success, group_id:%d, hwnd:%d, task_id:%d, last_task_id:%d",
                        group_id, hwnd, task_id, last_task_id);
                    goto back;
                }
            }
            LeaveCriticalSection(&gpos->lock);

            api_log_MsgError("update game taskid failed: hwnd not exist, group_id:%d, hwnd:%d",
                group_id, hwnd);
            goto back;
        }
    }

    api_log_MsgError("update game taskid failed: group not exist, group_id:%d, hwnd:%d",
        group_id, hwnd);
    
back:
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 UpdateGamePosition(INT32 group_id, INT32 hwnd, INT32 position, INT32 force)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    struct game_node* mpos = NULL, *mn = NULL;
    INT32 last_position = 0;
    INT32 code = ERRNO_NOT_EXIST;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(mpos, mn, &gpos->work_game_list, node, struct game_node)
            {
                if (mpos->hwnd == hwnd)
                {
                    last_position = mpos->current_step;
                    if (position < last_position)
                    {
                        if (force)
                        {
                            mpos->current_step = position;

                            code = ERRNO_SUCCESS;
                            LeaveCriticalSection(&gpos->lock);

                            api_log_MsgDebug("update game position force success, group_id:%d, hwnd:%d, position:%d, last_position:%d",
                                group_id, hwnd, position, last_position);
                            goto back;
                        }

                        code = ERRNO_INVALID;
                        LeaveCriticalSection(&gpos->lock);

                        api_log_MsgDebug("update game position failed, group_id:%d, hwnd:%d, position:%d, last_position:%d",
                            group_id, hwnd, position, last_position);
                        goto back;
                    }
                    
                    mpos->current_step = position;
                    code = ERRNO_SUCCESS;
                    LeaveCriticalSection(&gpos->lock);

                    api_log_MsgDebug("update game position success, group_id:%d, hwnd:%d, position:%d, last_position:%d",
                        group_id, hwnd, position, last_position);
                    goto back;
                }
            }
            LeaveCriticalSection(&gpos->lock);

            api_log_MsgError("update game position failed: hwnd not exist, group_id:%d, hwnd:%d",
                group_id, hwnd);
            goto back;
        }
    }

    api_log_MsgError("update game position failed: group not exist, group_id:%d, hwnd:%d",
        group_id, hwnd);
    
back:
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 UpdateGamePositionDie(INT32 group_id, INT32 hwnd, INT32 die_position, INT32 force)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    struct game_node* mpos = NULL, *mn = NULL;
    INT32 last_die_position = 0;
    INT32 code = ERRNO_NOT_EXIST;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(mpos, mn, &gpos->work_game_list, node, struct game_node)
            {
                if (mpos->hwnd == hwnd)
                {
                    last_die_position = mpos->die_position;
                    if (die_position < last_die_position)
                    {
                        if (force)
                        {
                            mpos->die_position = die_position;
                            code = ERRNO_SUCCESS;
                            LeaveCriticalSection(&gpos->lock);

                            api_log_MsgDebug("update game die_position force success, group_id:%d, hwnd:%d, die_position:%d, last_die_position:%d",
                                group_id, hwnd, die_position, last_die_position);
                            goto back;
                        }

                        code = ERRNO_INVALID;
                        LeaveCriticalSection(&gpos->lock);

                        api_log_MsgDebug("update game die_position failed, group_id:%d, hwnd:%d, die_position:%d, last_die_position:%d",
                            group_id, hwnd, die_position, last_die_position);
                        goto back;
                    }
                    
                    mpos->die_position = die_position;
                    code = ERRNO_SUCCESS;
                    LeaveCriticalSection(&gpos->lock);

                    api_log_MsgDebug("update game die_position success, group_id:%d, hwnd:%d, die_position:%d, last_die_position:%d",
                        group_id, hwnd, die_position, last_die_position);
                    goto back;
                }
            }
            LeaveCriticalSection(&gpos->lock);

            api_log_MsgError("update game die_position failed: hwnd not exist, group_id:%d, hwnd:%d",
                group_id, hwnd);
            goto back;
        }
    }

    api_log_MsgError("update game die_position failed: group not exist, group_id:%d, hwnd:%d",
        group_id, hwnd);
    
back:
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 ResetGame(INT32 group_id, INT32 hwnd)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    struct game_node* mpos = NULL, *mn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(mpos, mn, &gpos->work_game_list, node, struct game_node)
            {
                if (mpos->hwnd == hwnd)
                {
                    mpos->die_position = 0;
                    mpos->current_step = 0;

                    mpos->continue_die_count = 0;
                    memset(mpos->last_die_time, 0, 32);
                    code = ERRNO_SUCCESS;
                    break;
                }
            }
            LeaveCriticalSection(&gpos->lock);
            break;
        }
    }
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 GetGameTaskId(INT32 group_id, INT32 hwnd, INT32* ptr_task_id)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    struct game_node* mpos = NULL, *mn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        //if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(mpos, mn, &gpos->work_game_list, node, struct game_node)
            {
                if (mpos->hwnd == hwnd)
                {
                    if (NULL != ptr_task_id)
                    {
                        *ptr_task_id = mpos->task_id;
                        code = ERRNO_SUCCESS;
                    }

                    api_log_MsgDebug("current game taskid, group_id:%d, hwnd:%d, task_id:%d",
                        group_id, hwnd, mpos->task_id);

                    LeaveCriticalSection(&gpos->lock);
                    
                    return ERRNO_SUCCESS;
                }
            }
            LeaveCriticalSection(&gpos->lock);
            //break;
        }
    }

    //LeaveCriticalSection(&manager->lock);

    if (ERRNO_NOT_EXIST == code)
    {
        api_log_MsgError("get game taskid failed: not exist, group_id:%d, hwnd:%d",
            group_id, hwnd);
    }
    
    return code;
}

static INT32 _GetGamePosition(INT32 group_id, INT32 hwnd, INT32 type, INT32* ptr_position)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    struct game_node* mpos = NULL, *mn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(mpos, mn, &gpos->work_game_list, node, struct game_node)
            {
                if (mpos->hwnd == hwnd)
                {
                    code = ERRNO_SUCCESS;
                    
                    if (NULL != ptr_position)
                    {
                        switch (type)
                        {
                        case 0:
                            *ptr_position = mpos->current_step;
                            
                            api_log_MsgDebug("current game position, group_id:%d, hwnd:%d, current_step:%d",
                                group_id, hwnd, mpos->current_step);
                            break;
                        case 1:
                            *ptr_position = mpos->die_position;

                            api_log_MsgDebug("last game die position, group_id:%d, hwnd:%d, die_position:%d",
                                group_id, hwnd, mpos->die_position);
                            break;
                        default:
                            code = ERRNO_NOT_SUPPORT;

                            api_log_MsgDebug("get game position failed: not support, group_id:%d, hwnd:%d, type:%d",
                                group_id, hwnd, type);
                            break;
                        }
                    }

                    break;
                }
            }
            LeaveCriticalSection(&gpos->lock);
            break;
        }
    }
    //LeaveCriticalSection(&manager->lock);

    if (ERRNO_NOT_EXIST == code)
    {
        api_log_MsgError("get game position failed: not exist, group_id:%d, hwnd:%d, type:%d",
            group_id, hwnd, type);
    }
    return code;
}

INT32 GetGamePosition(INT32 group_id, INT32 hwnd, INT32* ptr_position)
{
    return _GetGamePosition(group_id, hwnd, 0, ptr_position);
}

INT32 GetGamePositionDie(INT32 group_id, INT32 hwnd, INT32* ptr_die_position)
{
    return _GetGamePosition(group_id, hwnd, 1, ptr_die_position);
}

INT32 MarkLastDietime(INT32 group_id, INT32 hwnd, CHAR* die_time)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL, *gnode = NULL;
    struct game_node* mpos = NULL, *mn = NULL;
    CHAR* last_die_time = NULL;
    double past_time = 0.0;
    INT32 code = -1;
    
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(mpos, mn, &gpos->work_game_list, node, struct game_node)
            {
                if (mpos->hwnd == hwnd)
                {
                    last_die_time = mpos->last_die_time;

                    code = left_strtime(last_die_time, die_time, &past_time);
                    if (ERRNO_SUCCESS == code)
                    {
                        api_log_MsgDebug("compute die interval success, last_die_time:%s, die_time:%s, past_time:%f, die_count:%d",
                            last_die_time, die_time, past_time, mpos->continue_die_count);
                        
                        if (past_time <= CONTINUE_DIE_TIME_INTERVAL)
                        {
                            mpos->continue_die_count += 1;
                        }
                        else
                        {
                            mpos->continue_die_count = 0;
                        }
                    }
                    else
                    {
                        api_log_MsgDebug("compute die interval failed, last_die_time:%s, die_time:%s, die_count:%d, code:%d",
                            last_die_time, die_time, mpos->continue_die_count, code);
                    }

                    if (mpos->continue_die_count >= CONTINUE_DIE_TIMES)
                    {
                        gpos->wait_next_flag = 1;
                    }
                    
                    api_log_MsgDebug("update the die time, last_die_time:%s, die_time:%s, group_id:%d, hwnd:%d, die_count:%d",
                        last_die_time, die_time, group_id, hwnd, mpos->continue_die_count);

                    memset(mpos->last_die_time, 0, 32);
                    memcpy(mpos->last_die_time, die_time, strlen(die_time));
                    
                    LeaveCriticalSection(&gpos->lock);
                    return ERRNO_SUCCESS;
                }
            }

            LeaveCriticalSection(&gpos->lock);
            return ERRNO_NOT_EXIST;
        }
    }

    return ERRNO_NOT_EXIST;
}

INT32 MarkWaitFlag(INT32 group_id, INT32 hwnd)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    struct sync_node* wait_next = NULL;
    INT32 j = 0;
    INT32 empty_hwnd = -1;
    
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);

            if (0 == gpos->wait_next_flag)
            {
                LeaveCriticalSection(&gpos->lock);

                api_log_MsgDebug("no need mark wait flag, group_id:%d, hwnd:%d",
                    group_id, hwnd);
                
                return ERRNO_NOT_SUPPORT;
            }
            
            wait_next = &gpos->wait_next;

            for (j = 0; j < 4; ++j)
            {
                if (wait_next->hwnd[j] == hwnd)
                {
                    LeaveCriticalSection(&gpos->lock);

                    api_log_MsgDebug("wait flag aready marked, group_id:%d, hwnd:%d",
                        group_id, hwnd);
                    
                    return ERRNO_SUCCESS;
                }

                if (-1 == empty_hwnd && 0 == wait_next->hwnd[j])
                {
                    empty_hwnd = j;
                }
            }

            if (-1 != empty_hwnd)
            {
                wait_next->hwnd[empty_hwnd] = hwnd;
                wait_next->member_count = wait_next->member_count + 1;
                LeaveCriticalSection(&gpos->lock);

                api_log_MsgDebug("wait flag marke success, group_id:%d, hwnd:%d",
                    group_id, hwnd);
                return ERRNO_SUCCESS;
            }
            
            LeaveCriticalSection(&gpos->lock);

            api_log_MsgDebug("wait flag marke not support, group_id:%d, hwnd:%d",
                group_id, hwnd);
            return ERRNO_NOT_SUPPORT;
        }
    }

    api_log_MsgDebug("wait flag marke failed: not exist, group_id:%d, hwnd:%d",
        group_id, hwnd);
    
    return ERRNO_NOT_EXIST;
}

INT32 CheckWaitFlag(INT32 group_id)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    struct sync_node* wait_next = NULL;

    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            if (0 == gpos->wait_next_flag)
            {
                LeaveCriticalSection(&gpos->lock);

                api_log_MsgDebug("no need check mark wait flag, group_id:%d",
                    group_id);
                
                return ERRNO_NOT_SUPPORT;
            }
            
            wait_next = &gpos->wait_next;

            if (MAX_GAME == wait_next->member_count)
            {
                LeaveCriticalSection(&gpos->lock);
                return ERRNO_SUCCESS;
            }

            LeaveCriticalSection(&gpos->lock);
            return ERRNO_AGAIN;
        }
    }

    return ERRNO_NOT_SUPPORT;
}


INT32 MarkReachFlag(INT32 group_id, INT32 hwnd, INT32 sync_position)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    struct sync_node* spos = NULL, *sn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    INT32 j = 0;
    INT32 empty_hwnd = -1;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(spos, sn, &gpos->sync_game_list, node, struct sync_node)
            {
                if (sync_position == spos->position)
                {
                    /* 不需要同步 */
                    if (!spos->need_sync)
                    {
                        LeaveCriticalSection(&gpos->lock);
                        code = ERRNO_SUCCESS;

                        api_log_MsgDebug("no need sync, group_id:%d, hwnd:%d, sync_position:%d",
                            group_id, hwnd, sync_position);
                        goto back;
                    }
                    
                    for (j = 0; j < 5; ++j)
                    {
                        if (spos->hwnd[j] == hwnd)
                        {
                            LeaveCriticalSection(&gpos->lock);
                            code = ERRNO_SUCCESS;

                            api_log_MsgDebug("hwnd has aready synced, j:%d, group_id:%d, hwnd:%d, sync_position:%d",
                                j, group_id, hwnd, sync_position);
                            
                            goto back;
                        }
                        
                        if (-1 == empty_hwnd && 0 == spos->hwnd[j])
                        {
                            empty_hwnd = j;
                        }
                    }

                    if (-1 != empty_hwnd)
                    {
                        spos->hwnd[empty_hwnd] = hwnd;
                        spos->member_count = spos->member_count + 1;
                        code = ERRNO_SUCCESS;
                        
                        api_log_MsgDebug("sync success, empty_hwnd:%d, group_id:%d, hwnd:%d, sync_position:%d",
                            empty_hwnd, group_id, hwnd, sync_position);
                        break;
                    }
                    
                    code = ERRNO_NOT_SUPPORT;

                    api_log_MsgDebug("sync failed:, not support, group_id:%d, hwnd:%d, sync_position:%d",
                        group_id, hwnd, sync_position);
                    break;
                }
            }
            LeaveCriticalSection(&gpos->lock);
            break;
        }
    }

    if (ERRNO_NOT_EXIST == code)
    {
        api_log_MsgError("mark reach flag failed: group not exist, group_id:%d, hwnd:%d, sync_position:%d",
            group_id, hwnd, sync_position);
    }
back:
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 CheckSyncFlag(INT32 group_id, INT32 sync_position)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    struct sync_node* spos = NULL, *sn = NULL;
    INT32 code = ERRNO_NOT_EXIST;

    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(spos, sn, &gpos->sync_game_list, node, struct sync_node)
            {
                if (sync_position == spos->position)
                {
                    if (!spos->need_sync)
                    {
                        LeaveCriticalSection(&gpos->lock);
                        code = ERRNO_SUCCESS;
                        goto back;
                    }

                    if (MAX_GAME == spos->member_count)
                    {
                        LeaveCriticalSection(&gpos->lock);
                        code = ERRNO_SUCCESS;
                        goto back;
                    }

                    LeaveCriticalSection(&gpos->lock);
                    code = ERRNO_AGAIN;
                    goto back;
                }
            }
            LeaveCriticalSection(&gpos->lock);
            break;
        }
    }

back:
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 ClearMarkReachFlag(INT32 group_id, INT32 hwnd, INT32 sync_position)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    struct sync_node* spos = NULL, *sn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    INT32 j = 0;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            list_for_each_entry_safe(spos, sn, &gpos->sync_game_list, node, struct sync_node)
            {
                if (sync_position == spos->position)
                {
                    /* 不需要同步 */
                    if (!spos->need_sync)
                    {
                        LeaveCriticalSection(&gpos->lock);
                        code = ERRNO_SUCCESS;

                        api_log_MsgDebug("no need clear reach flag, group_id:%d, hwnd:%d, sync_position:%d",
                            group_id, hwnd, sync_position);
                        goto back;
                    }
                    
                    for (j = 0; j < 5; ++j)
                    {
                        if (spos->hwnd[j] == hwnd)
                        {
                            spos->hwnd[j] = 0;
                            spos->member_count = spos->member_count - 1;
                            
                            LeaveCriticalSection(&gpos->lock);
                            code = ERRNO_SUCCESS;

                            api_log_MsgDebug("clear reach flag success, j:%d, group_id:%d, hwnd:%d, sync_position:%d",
                                j, group_id, hwnd, sync_position);
                            
                            goto back;
                        }
                    }
                    break;
                }
            }
            LeaveCriticalSection(&gpos->lock);
            break;
        }
    }

    if (ERRNO_NOT_EXIST == code)
    {
        api_log_MsgError("clear reach flag failed: group not exist, group_id:%d, hwnd:%d, sync_position:%d",
            group_id, hwnd, sync_position);
    }
back:
    //LeaveCriticalSection(&manager->lock);
    return code;
}

static INT32 _UpdateGroupPosition(INT32 group_id, INT32 position, INT32 type, INT32 force)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    INT32 last_position = 0;
    INT32* ptr_group_step = NULL;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            switch (type)
            {
            case 0:
                ptr_group_step = &gpos->group_step;

                api_log_MsgDebug("update group position, group_id:%d, position:%d",
                    group_id, position);
                break;
            
            case 1:
                ptr_group_step = &gpos->group_step_auxi;

                api_log_MsgDebug("update group position auxi, group_id:%d, position:%d",
                    group_id, position);
                break;
            
            default:
                {
                    code = ERRNO_NOT_SUPPORT;
                    LeaveCriticalSection(&gpos->lock);

                    api_log_MsgDebug("update group position success: not support, type:%d, group_id:%d, position:%d",
                        type, group_id, position);
                    goto back;
                }
                break;
            }

            last_position = *ptr_group_step;
            if (position == last_position)
            {
                code = ERRNO_SUCCESS;
                LeaveCriticalSection(&gpos->lock);

                api_log_MsgDebug("update group position success: same position, group_id:%d, position:%d",
                    group_id, position);
                goto back;
            }

            if (position < last_position)
            {
                if (force)
                {
                    *ptr_group_step = position;
                    code = ERRNO_SUCCESS;
                    LeaveCriticalSection(&gpos->lock);

                    api_log_MsgDebug("update group position force success: group_id:%d, position:%d, last_position:%d",
                        group_id, position, last_position);
                    goto back;
                }

                code = ERRNO_INVALID;
                LeaveCriticalSection(&gpos->lock);

                api_log_MsgError("update group position failed: group_id:%d, position:%d, last_position:%d",
                    group_id, position, last_position);
                goto back;
            }

            *ptr_group_step = position;
            code = ERRNO_SUCCESS;
            LeaveCriticalSection(&gpos->lock);

            api_log_MsgDebug("update group position success: group_id:%d, position:%d, last_position:%d",
                group_id, position, last_position);
            goto back;
        }
    }

    api_log_MsgError("update group position failed: group not exist, group_id:%d, position:%d",
        group_id, position);
back:
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 UpdateGroupPosition(INT32 group_id, INT32 position, INT32 force)
{
    return _UpdateGroupPosition(group_id, position, 0, force);
}

INT32 UpdateGroupPositionAuxi(INT32 group_id, INT32 position, INT32 force)
{
    return _UpdateGroupPosition(group_id, position, 1, force);
}

static INT32 _GetGroupPosition(INT32 group_id, INT32 type, INT32* ptr_position)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    INT32* ptr_group_step = NULL;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            switch (type)
            {
            case 0:
                ptr_group_step = &gpos->group_step;

                api_log_MsgDebug("get group position, group_id:%d", group_id);
                break;
            
            case 1:
                ptr_group_step = &gpos->group_step_auxi;

                api_log_MsgDebug("get group position auxi, group_id:%d", group_id);
                break;
            
            default:
                {
                    code = ERRNO_NOT_SUPPORT;
                    LeaveCriticalSection(&gpos->lock);

                    api_log_MsgDebug("get group position success: not support, type:%d, group_id:%d",
                        type, group_id);
                    goto back;
                }
                break;
            }

            if (NULL != ptr_position)
            {
                *ptr_position = *ptr_group_step;
            }
            
            LeaveCriticalSection(&gpos->lock);

            api_log_MsgDebug("get group position success, group_id:%d, type:%d",
                group_id, type);
            
            code = ERRNO_SUCCESS;
            break;
        }
    }

back:
    //LeaveCriticalSection(&manager->lock);

    if (ERRNO_NOT_EXIST == code)
    {
        api_log_MsgError("get group position failed: group not exist, group_id:%d, type:%d",
            group_id, type);
    }
    return code;
}

INT32 GetGroupPosition(INT32 group_id, INT32* ptr_position)
{
    return _GetGroupPosition(group_id, 0, ptr_position);
}

INT32 GetGroupPositionAuxi(INT32 group_id, INT32* ptr_position)
{
    return _GetGroupPosition(group_id, 1, ptr_position);
}

static INT32 _UpdateGroupBorderStone(INT32 group_id, INT32 border_stone,
    INT32 force, DIR_E direction)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    INT32 last_border_stone = 0;
    INT32* ptr_border_stone = NULL;

    api_log_MsgDebug("update border stone, group_id:%d, border_stone:%d, direction:%d",
        group_id, border_stone, direction);
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            switch (direction)
            {
            case DIR_LEFT:
                last_border_stone = gpos->border_stone_left;
                ptr_border_stone = &gpos->border_stone_left;
                break;
                 
            case DIR_RIGHT:
                last_border_stone = gpos->border_stone_right;
                ptr_border_stone = &gpos->border_stone_right;
                break;
                
            default:
                {
                    LeaveCriticalSection(&gpos->lock);
                    
                    code = ERRNO_NOT_SUPPORT;
                    api_log_MsgDebug("update border stone failed: invalid border stone, group_id:%d, border_stone:%d",
                        group_id, border_stone);
                    goto back;
                }
                break;
            }

            if (last_border_stone != border_stone)
            {
                if (force)
                {
                    *ptr_border_stone = border_stone;
                    LeaveCriticalSection(&gpos->lock);

                    code = ERRNO_SUCCESS;
                    api_log_MsgDebug("update border stone force success, group_id:%d, last_border_stone:%d, border_stone:%d",
                        group_id, last_border_stone, border_stone);
                    goto back;
                }

                LeaveCriticalSection(&gpos->lock);

                code = ERRNO_INVALID;
                api_log_MsgDebug("update border stone failed, group_id:%d, last_border_stone:%d, border_stone:%d",
                    group_id, last_border_stone, border_stone);
                goto back;
            }
            LeaveCriticalSection(&gpos->lock);

            code = ERRNO_SUCCESS;
            api_log_MsgDebug("update border stone success: same border stone, group_id:%d, last_border_stone:%d, border_stone:%d",
                group_id, last_border_stone, border_stone);
            goto back;
        }
    }

    api_log_MsgError("update border stone failed: group not exist, group_id:%d, border_stone:%d",
        group_id, border_stone);
    
back:
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 UpdateGroupBorderStoneLeft(INT32 group_id, INT32 border_stone, INT32 force)
{
    return _UpdateGroupBorderStone(group_id, border_stone, force, DIR_LEFT);
}

INT32 UpdateGroupBorderStoneRight(INT32 group_id, INT32 border_stone, INT32 force)
{
    return _UpdateGroupBorderStone(group_id, border_stone, force, DIR_RIGHT);
}

static INT32 _GetGroupBorderStone(INT32 group_id, DIR_E direction, INT32* ptr_border_stone)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    INT32 code = ERRNO_NOT_EXIST;
    INT32* ptr_border = NULL;

    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            switch (direction)
            {
            case DIR_LEFT:
                ptr_border = &gpos->border_stone_left;
                break;
                 
            case DIR_RIGHT:
                ptr_border = &gpos->border_stone_right;
                break;
                
            default:
                {
                    LeaveCriticalSection(&gpos->lock);
                    
                    code = ERRNO_NOT_SUPPORT;
                    api_log_MsgDebug("get border stone failed: not support, group_id:%d",
                        group_id);
                    goto back;
                }
                break;
            }

            if (NULL != ptr_border_stone)
            {
                *ptr_border_stone = *ptr_border;
            }
            LeaveCriticalSection(&gpos->lock);

            api_log_MsgDebug("get group border stone success, group_id:%d, border_stone:%d",
                group_id, *ptr_border_stone);
            
            code = ERRNO_SUCCESS;
            break;
        }
    }

back:
    //LeaveCriticalSection(&manager->lock);

    if (ERRNO_NOT_EXIST == code)
    {
        api_log_MsgError("get group border stone failed: group not exist, group_id:%d, direction:%d",
            group_id, direction);
    }
    return code;
}

INT32 GetGroupBorderStoneLeft(INT32 group_id, INT32* ptr_border_stone)
{
    return _GetGroupBorderStone(group_id, DIR_LEFT, ptr_border_stone);
}

INT32 GetGroupBorderStoneRight(INT32 group_id, INT32* ptr_border_stone)
{
    return _GetGroupBorderStone(group_id, DIR_RIGHT, ptr_border_stone);
}

INT32 ResetGroup(INT32 group_id)
{
    struct manager* manager = &g_Manager;
    struct group_node* gpos = NULL, *gn = NULL;
    struct sync_node* spos = NULL, *sn = NULL;
    INT32 j = 0;
    INT32 code = ERRNO_NOT_EXIST;
    
    //EnterCriticalSection(&manager->lock);
    list_for_each_entry_safe(gpos, gn, &manager->work_group_list, node, struct group_node)
    {
        if (gpos->group_id == group_id)
        {
            EnterCriticalSection(&gpos->lock);
            gpos->border_stone_left = 0;
            gpos->border_stone_right = 0;
            gpos->group_step = 0;
            gpos->group_step_auxi = 0;
            
            list_for_each_entry_safe(spos, sn, &gpos->sync_game_list, node, struct sync_node)
            {
                for (j = 0; j < 4; j++)
                {
                    spos->hwnd[j] = 0;
                    spos->member_count = 0;
                }
            }

            memset(&gpos->wait_next, 0, sizeof(struct sync_node));
            gpos->wait_next_flag = 0;
            
            LeaveCriticalSection(&gpos->lock);
            code = ERRNO_SUCCESS;
            break;
        }
    }
    //LeaveCriticalSection(&manager->lock);
    return code;
}

INT32 GetContextAuxi(INT32 hwnd)
{
    struct list_head* list = &g_auxi_list;
    struct auxi_detail* pos = NULL, *n = NULL, *node = NULL;
    
    list_for_each_entry_safe(pos, n, list, node, struct auxi_detail)
    {
        if (hwnd == pos->hwnd)
        {
            api_log_MsgDebug("context auxi aready exist, hwnd:%d", hwnd);
            return ERRNO_EXIST;
        }
    }

    node = (struct auxi_detail*)malloc(sizeof(struct auxi_detail));
    if (NULL == node)
    {
        api_log_MsgError("get context auxi failed: no memory, hwnd:%d", hwnd);
        return ERRNO_NOMEM;
    }

    memset(node, 0, sizeof(struct auxi_detail));
    INIT_LIST_HEAD(&node->node);
    node->hwnd = hwnd;
    list_add_tail(&node->node, list);

    api_log_MsgDebug("get context auxi success, hwnd:%d", hwnd);
    
    return ERRNO_SUCCESS;
}

INT32 GetContextAuxiTaskId(INT32 hwnd, INT32* ptr_task_id)
{
    struct list_head* list = &g_auxi_list;
    struct auxi_detail* pos = NULL, *n = NULL;
    
    list_for_each_entry_safe(pos, n, list, node, struct auxi_detail)
    {
        if (hwnd == pos->hwnd)
        {
            if (NULL != ptr_task_id && pos->task_id != 0)
            {
                *ptr_task_id = pos->task_id;
                
                api_log_MsgDebug("get context auxi taskid success, hwnd:%d, task_id:%d",
                    hwnd, pos->task_id);
                return ERRNO_SUCCESS;
            }

            return ERRNO_INVALID;
        }
    }

    api_log_MsgDebug("get context auxi taskid failed: not exist, hwnd:%d", hwnd);
    
    return ERRNO_NOT_EXIST;
}

INT32 UpdateContextAuxi(INT32 hwnd, INT32 task_id)
{
    struct list_head* list = &g_auxi_list;
    struct auxi_detail* pos = NULL, *n = NULL;

    if (task_id <= 0)
    {
        return ERRNO_INVALID;
    }
    
    list_for_each_entry_safe(pos, n, list, node, struct auxi_detail)
    {
        if (hwnd == pos->hwnd)
        {
            pos->task_id = task_id;

            api_log_MsgDebug("update context auxi taskid success, hwnd:%d, task_id:%d",
                hwnd, task_id);
            return ERRNO_SUCCESS;
        }
    }

    api_log_MsgDebug("update context auxi taskid failed: not exist, hwnd:%d, task_id:%d",
        hwnd, task_id);
    
    return ERRNO_NOT_EXIST;
}

INT32 PutContextAuxi(INT32 hwnd)
{
    struct list_head* list = &g_auxi_list;
    struct auxi_detail* pos = NULL, *n = NULL;
    
    list_for_each_entry_safe(pos, n, list, node, struct auxi_detail)
    {
        if (hwnd == pos->hwnd)
        {
            api_log_MsgDebug("put context auxi success, hwnd:%d, task_id:%d",
                hwnd, pos->task_id);
            
            list_del(&pos->node);
            free(pos);

            return ERRNO_SUCCESS;
        }
    }

    api_log_MsgDebug("put context auxi failed: not exist, hwnd:%d", hwnd);
    return ERRNO_NOT_EXIST;
}

