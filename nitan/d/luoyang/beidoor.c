inherit ROOM;

void create()
{
        set("short", "�ڳǱ���");
        set("long", @LONG
�����ڳǱ��Ÿ����ɣ���Լ�������࣬������ĳ�����������ȭͷ��
��С���������Ŷ���Լ���ɣ���Լÿ������ʱ���ͻ��ٱ����ء���������
������׽��֮��Ĺٸ�ͨ����һ����ֱ����ʯ�������ϱ��������죬��
ȥ���������ڳ��ˣ���һ�Ÿ߸߿�������ǻ��Ǻӡ�
LONG);
        set("outdoors", "luoyang");
        set("exits", ([
                  "north" : __DIR__"nbridge",
                  "south" : __DIR__"nroad4",
        ]));
        set("objects", ([
                "/d/kaifeng/npc/guanbing" : 4,
                "/d/beijing/npc/ducha" : 1,
                "/clone/npc/walker" : 1,
        ]));

	set("coor/x", -6990);
	set("coor/y", 2230);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}