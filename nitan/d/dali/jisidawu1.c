//Room: /d/dali/jisidawu1.c
//Date: June. 29 1998 by Java

inherit ROOM;
void create()
{
        set("short","�������");
        set("long",@LONG
�����������ļ�����ݣ���Ϊ��������������һ����̨��̨�ϰ�
����һ��������λ����������������֮������λ���ǽ�Ϲ��д����
Ƥ���ϻ����λ��ƣ����Ǹ����ͼ�ڡ�һλ�ϼ�˾�������ڼ�̨ǰ��
����ȫ��ļ�˾���
LONG);
        set("objects", ([
           __DIR__"npc/laojisi": 1,
        ]));
        set("exits",([ /* sizeof() == 1 */
            "out"      : __DIR__"wuyiminju3",
        ]));
	set("coor/x", -19221);
	set("coor/y", -6861);
	set("coor/z", 19);
	setup();
}

void init()
{
        object me = this_player();
        if( query_temp("xuncheng", me) )
        {
                set_temp("dali_xc/jsdw", 1, me);
        }
}