#ifndef __SNAIL_COMMON_H__
#define __SNAIL_COMMON_H__

#define SNAIL_CONTEXT_NUM_MAX   (128)

#define SNAIL_REQUEST_LEN_MAX   (2048)

#define SNAIL_RESPONSE_LEN_MAX  (2048)

#define SNAIL_LOCAL_HOST     "127.0.0.1"
#define SNAIL_PORT  (9191)

#define SNAIL_PATH_LEN_MAX  (2048)

typedef enum
{
    SNAIL_TASK_UNKNOWN = 0,
    SNAIL_TASK_MAX,
}SNAIL_TASK_TYPE_E;

#endif
