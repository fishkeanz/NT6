// Room: /huanghe/huanghe_3.c
// Java. Sep 21 1998

#include <room.h>

inherit ROOM;
void create()
{
        set("short", "�ƺ�");
        set("long", @LONG
��ˮ���˹��ˣ���Դ�ھ�Ȫ�����ĺ�ˮ�����³Ǻ������������
��ˮ��ȥ���𽥼�ǿ��Ю����ǧ��ֵĸ�ԭ��ɳ���Ʋ��ɵ��������ϱ�
ȥ�����ڴ˴�ˮ���ļ���������������֬�ᡣ�������µĻƺӴ���
�㼴���ڴˡ�
LONG );
        set("exits", ([
                "east" : __DIR__"huanghe_2",
                "northwest" : "/d/yongdeng/yongdeng",
        ]));
        set("no_clean_up", 0);
        set("outdoors", "huanghe");
        set("coor/x", -5000);
        set("coor/y", 450);
        set("coor/z", 0);
        setup();
        replace_program(ROOM);
}