//Room: /d/dali/road4.c
//Date: June. 29 1998 by Java

inherit ROOM;
void create()
{
        set("short","��·");
        set("long",@LONG
һ����ͨ�ϱ��Ĵ�·�����ɴ��������ϱ߽����Ĺٵ�����·����
��������ï�ܣ���Ƭ���ָ���ɽ�룬�˼���ƽ�ƽ̹����·��ֱ����
���ű��ߵĴ����Ǻ��ϱߵ�ϲ�ݵȳ���
LONG);
        set("outdoors", "dalis");
        set("exits",([ /* sizeof() == 1 */
            "north"      : __DIR__"shuangheqiao",
            "south"      : __DIR__"tianweijing",
        ]));
        set("no_clean_up", 0);
	set("coor/x", -19130);
	set("coor/y", -6930);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}