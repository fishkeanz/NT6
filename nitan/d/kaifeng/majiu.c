#include <room.h>
inherit TRANS_ROOM;

void create()
{
      set("short", "����");
        set("long", @LONG
���ǵ���Ӻ�����ǣ����깩Ӧ���ʲ��ϡ����⸮�Թ��˾�����
�أ���ʷ�ƾã���紾�ӡ������ǻ����ǣ�����Ǻ����տ���������
ι�����㣬��ϴˢ�ø�Ǭ������һֱ�ź򵽿�����·�����������µ�
��ľ���϶���һ����ľ�� (paizi)�������жѷ������Ѳ��ϣ�������
һ����ۡ�
LONG );
        set("outdoors", "kaifeng");
        set("no_fight", 1);
        set("objects", ([
                "/clone/horse/zaohongma": 1,
                "/clone/horse/huangbiaoma": 1,
                "/clone/horse/ziliuma": 1,
                "/clone/npc/mafu": 1,
        ]));
        set("exits",([
               "west" : __DIR__"daoxiang",
               "north" : __DIR__"business",
               "up"   : "/clone/shop/kaifeng_shop",
        ]));
	set("coor/x", -5030);
	set("coor/y", 2250);
	set("coor/z", 0);
	setup();
        replace_program(TRANS_ROOM);
}