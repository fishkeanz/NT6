inherit ROOM;

void create()
{
        set("short", "�����");
        set("long", @LONG
�������ռ�������Ķ��Ͻ��ʮ�ĸ�������Ժ����ɮǧ�ࡣ�Ǿ�
��������Ժ��ȫ����̻���ġ���ǰ��������¡����Ҷ����ǵ�
���������顣���ų�������������������
LONG );
        set("objects", ([
                  __DIR__"npc/zhike" : 1,
        ]));
        set("no_clean_up", 0);
        set("exits", ([
                  "north" : __DIR__"qianyuan",
                  "south" : __DIR__"jiedao",
        ]));
        set("outdoors", "kaifeng");

	set("coor/x", -5040);
	set("coor/y", 2170);
	set("coor/z", 10);
	setup();
        replace_program(ROOM);
}