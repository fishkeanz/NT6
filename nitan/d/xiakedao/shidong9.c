// shidong9.c ʯ��

inherit ROOM;

void create()
{
        set("short", "ʯ��");
        set("long", @LONG
����һ����С��ʯ�������ߵ��ź��򣬹����䰵��ȴҲ�ܱ��
�������������ţ�����Լ������ǰ����һ����Ӱ��ֻ����������
ɫ����������ǰ����һ��Ψ�Ҷ�������ӡ�
LONG );
        set("exits", ([
                "west" : __DIR__"shidong10",
                "east" : __DIR__"yadi",
        ]));
        set("objects",([
                __DIR__"npc/baishou" : 1,
        ]));
        set("coor/x", -3100);
        set("coor/y", -22070);
        set("coor/z", 0);
        setup();
}

int valid_leave(object me, string dir)
{

        if (  (dir == "west")
            && (query("shen", me)<10000 )
            || (query("PKS", me)>10 )
           && objectp(present("bai shou",environment(me))) )
        return notify_fail
                ("���װ���һ����������а��������ҵ����������! \n");
        return ::valid_leave(me, dir);
}