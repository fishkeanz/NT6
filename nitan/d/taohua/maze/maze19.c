// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

inherit ROOM;

#include "maze.h"

void create()
{
        set("short", "ʯ��");
        set("long", @LONG
����һ����ԼĪ���˶�ߵĴ�ʯ�鹹�ɵ���ʯ�󣬹�ģ�ƺ����󣬵�
̧����ȥ�����ܾ������ص�������Ᾱ�ʯ����ȫ��������·ͨ��ʯ��֮�⣬
��֮���г�������������������������ͷ����ʯ������һ��б����ʯ��(s
hibei)�������ڸ����С�
LONG );

        set("exits", ([
                "east"  : __DIR__"maze24",
                "west"  : __DIR__"maze15",
                "north" : __DIR__"maze41",
        ]));

        set("item_desc", ([
                "shibei" : "
�����٣�Ԫ���࣬�����ꡣ ���ڰ������ס�
�������ţ����٣��꼪��
�����Ŷ������٣����޲�����
�������������٣��������� ����֮���޾̡�
�������ģ����٣��޾̡�
�������壺֪�٣����֮�ˣ�����
�������������٣����޾̡�
\n",
        ]));

        setup();
}

void init()
{
        object me;
        me = this_player();
        
        addn_temp("taohua/count", 19, me);
        check_count(me, 19);
        return;
}