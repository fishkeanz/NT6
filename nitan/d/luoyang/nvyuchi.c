inherit ROOM;

void create()
{
        set("short", "Ůԡ��");
        set("long", @LONG
�����������ڣ��������ƣ�һ�ж�������֮�У��޷��������У�΢��
���ԣ�����������������۾���ϸһ���������������������������Ů��
���Ƕ�Ү���ٿ����Լ����������μ�穡��������������ǸϿ��Ҹ��ط�ϴ
��ɣ�
LONG);
        set("no_clean_up", 0);
        set("exits", ([
                  "west" : __DIR__"rest2",
        ]));
        set("objects", ([
                "/d/kaifeng/npc/obj/zaopeng" : 2,
        ]));
	set("coor/x", -6970);
	set("coor/y", 2150);
	set("coor/z", 10);
	setup();
        replace_program(ROOM);
}