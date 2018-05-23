inherit DEMONROOM;

#include <ansi.h>

void create()
{
        set("short", "蟠桃园");
        set("long",@LONG
这里是蓬莱仙岛的蟠桃园，蟠桃树一百年开花，再过一百年才
结果，每逢蟠桃出现的时候群仙便会会聚在此举行蟠桃大会。虽然
同居一岛，但是每过两百年群仙才会借着蟠桃大会聚会一次，共享
蟠桃美酒。
LONG);

        set("exits", ([
                "northeast"   : __DIR__"fanchendao",
                "southeast"   : __DIR__"pantaoyuan1",
        ]));
        set("no_rideto", 1);         // 设置不能骑马到其他地方
        set("no_flyto", 1);          // 设置不能从起来地方骑马来这里
        set("no_die", 1);            // 死亡后移动到扁鹊居
        set("penglai", 1);           // 表示在蓬莱岛

        set("n_time", 30); 
        set("n_npc", 1); 
        set("n_max_npc", 6);
        set("s_npc", __DIR__"npc/mihou");
        
        setup();
}

void init ()
{
        if (! this_player()->query_temp("apply/xianshu-lingwei"))
        {
                if (! this_player()->query("penglai/go_quest/ok"))
                {
                        this_player()->start_busy(3);
                        tell_object(this_player(), NOR + WHT "你到了此处，四周仙气缭绕令你一时间难以辨别方向。\n" NOR);
                }
                else
                {
                        if (random(2))
                        {
                                this_player()->start_busy(1);
                                tell_object(this_player(), NOR + WHT "你到了此处，四周仙气缭绕令你一时间难以辨别方向。\n" NOR);                
                        }
                }
        }
}