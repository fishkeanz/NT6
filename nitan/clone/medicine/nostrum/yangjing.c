// yangjing.c

#include <ansi.h>
inherit ITEM;

void create()
{
        set_name(HIG"������"NOR, ({"yangjing dan", "dan"}));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set_weight(4000);
                set("unit", "��");
                set("vegetable", 7);
                set("nostrum", 128);
                set("level", 10);
                set("value", 5000);
        }
        set("pour_type", "1");
        setup();
}

int init()
{
        add_action("do_eat", "eat");
        add_action("do_eat", "fu");
}

int do_eat(string arg)
{
        object me = this_player();

        if(!id(arg)) return notify_fail("��Ҫ��ʲô��\n");
        if(!present(this_object(), me))
                return notify_fail("��Ҫ��ʲô��\n");
        if( me->is_busy() )
                return notify_fail("�𼱣������ԣ�С�ı�ҭ���ˡ�\n");

        message_vision("$N����һ��" + name() + "��\n", me);
/*        if (((int)me->query_condition("xx_poison") > 0) ||
                (query("eff_jing", me)<query("max_jing", me)) )
        {
                me->apply_condition("jing_curing", me->query_condition("jing_curing") + 10);
*/
        if (((int)me->query_condition("xx_poison") > 0) ||
        ((query("eff_jing", me)/query("max_jing", me))<0.9) )
        {
                me->receive_curing("jing",query("max_jing", me)/50);
        }
        me->start_busy(2);
        destruct(this_object());
        return 1;
}
 