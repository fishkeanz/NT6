// Room: /d/shaolin/zoulang4.c
// Date: YZC 96/01/19

inherit ROOM;

string look_poem();

void create()
{
        set("short", "����");
        set("long", @LONG
�������������ȡ�����һ��֧������ĸ�ǽ�ϣ���һ�����뱱��
�޺��õ��ݶ�����������������������Ҳ�ò�������˸��ַ����ͼ
�Σ�ÿ�����ƺ�������˵һ���渶��˵ķ𾭹��¡��ϱ�ǽ������һ
��ʫ(poem)������ͨ���޺��á�
LONG );
        set("exits", ([
                "west" : __DIR__"zoulang3",
                "north" : __DIR__"luohan1",
        ]));
        set("item_desc",([
                "poem"                :        (: look_poem :),
        ]));
        set("no_clean_up", 0);
        setup();
}

string look_poem()
{
        return
        "\n"
        "    ����������������������������������������������������\n"
        "    ����������������������������������������������������\n"
        "    ���������������������������ӣ�����������������������\n"
        "    ����������������������������������������������������\n"
        "    ��������������  ɫ����գ��ղ���ɫ������������������\n"
        "    ����������������������������������������������������\n"
        "    ��������������  ɫ���ǿգ��ռ���ɫ������������������\n"
        "    ����������������������������������������������������\n"
        "    ��������������  ������ʶ���ิ���ǡ�����������������\n"
        "    ����������������������������������������������������\n"
        "    ���������������������������ӣ�����������������������\n"
        "    ����������������������������������������������������\n"
        "    ��������������  ����࣬�������𣬡���������������\n"
        "    ����������������������������������������������������\n"
        "    ��������������  ������������������������������������\n"
        "    ����������������������������������������������������\n"
        "    ����������������������������������������������������\n";
}