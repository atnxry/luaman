#ifndef __TASK_EXSIGNLE_PUB_H__
#define __TASK_EXSIGNLE_PUB_H__

#define FONT_EXSIGNLE_DIR "font\\exsignle\\"
#define BMP_EXSIGNLE_DIR "bmp\\exsignle\\"

#define EXSIGNLE_ROLES() {"A", "B", "C", "D", "E", \
    "F", "G", "H", "I", "J",\
    "K", "L", "M", "N", "O",\
    "P", "Q", "R", "S", "T",\
    "U", "V", "W", "X", "Y",\
    "Z",\
}

#define EXSIGNLE_ROLES_NR   (26)

struct gabage_info
{
    const char* name;
    const char* color_offset;
};

//点击整理背包 893:148 -> 763:543
#define do_click_tidy_package(_tsk_, _dmobj_, _pkg_x_, _pkg_y_) do{\
    Check_Terminate_Thread_With_Alram(_tsk_, _dmobj_);\
    (_dmobj_)->MoveTo((_pkg_x_) - 130, (_pkg_y_) + 395);\
    (_dmobj_)->LeftClick();\
    Delay(200);\
    (_dmobj_)->MoveTo(5, 5);\
    Delay(200);\
}while(0)

int start_work(struct cluamanaccountinfo* accountinfo, int roleno, Idmsoft* dmobj, void* ctxt);

#endif
