// Room: /city/yamen.c
// YZC 1995/12/04 

inherit ROOM;

void create()
{
	set("short", "���Ŵ���");
        set("long", @LONG
���������Ŵ��ţ�������ľ���Ž����ر��š����ྲ�����رܡ�
�����ӷַ���ͷʯʨ�ӵ��Աߡ�ǰ����һ����ģ���Ȼ�ǹ�С����ԩ
�õġ�������������ǰѲ�ߡ�
LONG );
	set("exits", ([
		"south" : "/d/city/xidajie1",
		"north" : "/d/city/ymzhengting",
	]));
	set("objects", ([
		"/d/city/npc/yayi" : 4,
	]));
	set("coor/x", -10);
	set("coor/y", 10);
	set("coor/z", 0);
	setup();
}

int valid_leave(object me, string dir)
{
	if (!wizardp(me) && objectp(present("ya yi", environment(me))) && dir == "north")
		return notify_fail("���ۺȵ������������䡭������\n");
	return ::valid_leave(me, dir);
}