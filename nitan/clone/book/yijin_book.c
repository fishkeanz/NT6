// yijinjing.c

inherit BOOK;
#include <ansi.h>

void create()
{
        set_name(YEL"�׽"NOR, ({ "yijinjing" }));
        set_weight(300);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long",
                "�׽\n"
                "����һ������˿֯���ᣬ�������������ˣ��������ּ����֪��ʶ
Ҳ��ʶ���׽����������֮�� ��һֱ�ײش˴���\n");
                set("value", 10);
                set("material", "silk");
                set("skill", ([
                        "name":         "yijinjing",    // name of the skill
                        "exp_required": 100000, // minimum combat experience required
                        "jing_cost":    50,     // jing cost every time study this
                        "difficulty":   30,     // the base int to learn this skill
                        "max_skill":    380     // the maximum level you can learn
                ]) );
        }
}
