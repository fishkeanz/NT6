#include <room.h>
inherit ROOM;

void create()
{
        set("short", "���ڶ���");
        set("long", @LONG
���ڶ�����һ�������Ĵ�֣�Ҳ�Ǳ��������ҵĵضΡ�һ������
����ʯ������д�����������ͷ���졣�����Ǿ����������ķ����š�
�ӱ���Ʈ��һ�������˵Ļ��㣬��ʱ��ľ���Ϊ֮һ���ϱ���һ��
�����̣� ��ʱ�����������˵��ô�����
LONG );
        set("exits", ([
                "south" : "/d/beijing/tiepu",
                "north" : "/d/beijing/huadian",
                "east" : "/d/beijing/yong_3",
                "west" : "/d/beijing/yong_1",
        ]));

        set("outdoors", "beijing");
	set("coor/x", -2780);
	set("coor/y", 7650);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}