// Room: /d/songshan/yunutai.c
// Last Modified by Winder on Jul. 15 2001

inherit ROOM;

void create()
{
        set("short", "��Ų̊");
        set("long", @LONG
��������ɽ���ϵ���Ų̊����Ų̊�µ�ʯ�ȶ������ǡ�ʯ�Ȼ�����
(jing)֮��������ʯ�ڸ��ʣ��վ��������ֹ���ᾶ��ˣ���С���¡���
���о�ʯ�������පѨ��ˮ��ʯ�죬����������������ʯ�ȡ���
LONG );
        set("item_desc", ([
                "jing" : "
    ʯ����̦��������ˮ�峺���ף��Ͽ�ݺ�����裬ˮ��Ⱥ��ս�Ρ���
ˮ̶���и���ʯͷ������ˮ�棬ƽ���簸������ʯ����������Χ��ˮ��
���滷ɽ������ӳȾ��ˮ��΢���������棬ɽ��ˮ�㣬���ﻨ�㣬����
��������֮�С�����ʫ��֣��ԭ����ʯ�ȡ�ʫ�ƣ�
                  ����δ�����������������׸�������
                  ʯ�Ȳβ���������̶��Ө������ƽ��
                  ������ӻػ�ת����ˮ����ί���С�
                  ���踶�������ã����̺Ӻ���ӯӯ��
    ��ʱ��ʯ����Ϊ��ˮӪɽ�󡱡�������ʤ������ÿ����¾�������
�ѽڣ�����Я���Ʋˣ���ˮ��ʯ�ϣ���ȭ�������ʤ�����ƴ�Ů����
���죬��ε�ʯ��Ѱ�����֡�����Ԫ�����������������ʱ�����ڴ�̨
�ϣ��ϵѸ��裬����Ⱥ������ʸ�ʫ��Ħ�±��̡��ݴˣ�ʷ�ơ�ʯ�Ȼ�
������
    �������졶ʯ�ȡ�ʫ�ƣ�
                  ��ɽʮ������׭����ώ�������΢��
                  ��¶��˪��ʤ�������罻���л�ս��
                  ������²���ɫ��ǧѰ����ԡ���¡�
                  ��פ���������ǣ����������ӳ��ɡ�
",
        ]));
        set("exits", ([
                "northwest" : __DIR__"shandao1",
        ]));
        set("no_clean_up", 0);
        set("outdoors", "songshan");
        set("coor/x", -20);
        set("coor/y", 810);
        set("coor/z", 30);
        setup();
}
 
int valid_leave(object me, string dir)
{
        if ((random((int)me->query_skill("dodge")) <= 20) && dir=="up")    
        {
                return notify_fail("��ʹ����һ�ģ����ٲ�����������Զ�͵���������ˤ�ı������ס�\n");
        }
        if ((random((int)me->query_skill("dodge")) <= 35) && dir=="up")    
        {
                return notify_fail("�����һԾ��ȴ���ٲ�������һ�ɶ�Զ�������ǰ׷�������\n");
        }
        if ((random((int)me->query_skill("dodge")) <= 60) && dir=="up")    
        {
                return notify_fail("�������������ٲ���ֻ��һ����ˣ��ټӰѾ���\n");
        }
        return ::valid_leave(me, dir);
}