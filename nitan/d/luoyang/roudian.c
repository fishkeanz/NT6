inherit ROOM;

void create()
{
        set("short", "������");
        set("long", @LONG
��ϡϡ��ľ���Ϸ��ż�����ɫ�Ұ���������Ŀ�����⡣һ����ͷ��Ӭ
ææµµ�ķ�����䡣����������������Ϲ��Ű����˵���ͷ����ͷ��
��ţͷ����̨����Ƕ����ż���ľϻ��ϻ�з���Щ�����˵�������Źǡ�
LONG);
        set("outdoors", "luoyang");
        set("no_clean_up", 0);
        set("exits", ([
                  "west" : __DIR__"zashua",
                  "south" : __DIR__"wanju",
                  "east" : __DIR__"hutong3",
        ]));
        set("objects", ([
                "/d/beijing/npc/old1" : 1,
        ]));
	set("coor/x", -6950);
	set("coor/y", 2180);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}