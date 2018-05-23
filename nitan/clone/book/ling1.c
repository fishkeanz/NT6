#include <weapon.h>
#include <ansi.h>

inherit SWORD;

void create()
{
        set_name(CYN "ʥ����" NOR, ({ "shenghuo ling", "shenghuo", "ling" }) );
        set_weight(1000);
        if (clonep())
                destruct(this_object());
        else {
                set("unit", "��");
                set("value", 10000);
                set("material", "steel");
                set("no_sell", "�����̵ģ�����������ҲֵǮ��");
                set("long", NOR + CYN "
����һ�����������ĺ��ƣ��ǽ�������ʵؼ�Ӳ�ޱȣ���͸��
�����������л�����ڣ�ʵ��������ӳ�⣬��ɫ��á����Ͽ�
���в��ٲ�˹�ģ��ƺ�����ѧ�йأ�������£�����Ҫ��ϸ��
��һ�����С�\n" NOR);
                set("wield_msg", HIR "$N" HIR "��ৡ���һ������������һƬ����������������С�\n" NOR);
                set("unwield_msg", HIR "$N" HIR "��ʥ���������䡣\n" NOR);
                set("skill", ([
                        "name":         "shenghuo-ling",
		                "family_name" : "����",
                        "exp_required": 1000000,
                        "jing_cost":    80,
                        "difficulty":   46,
                        "max_skill":    49,
                        "min_skill":    0,
                        "need" : ([ "sanscrit" : 150 ]),
                ]) );
        }
        init_sword(80);
        setup();
}