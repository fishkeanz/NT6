#include <ansi.h>
inherit ITEM;
void create()
{
    set_name( HIR "���" NOR, ({ "hongbao" }) );
    set_weight(1);
    if( clonep() )
            set_default_object(__FILE__);
    else {
            set("unit", "��");
            set("material", "cloth");
            set("long", "һ��СС�ĺ�����������ţ���wiz���Ρ�СС���\n");
          }
    setup();
}

void init()
{
    if(environment()==this_player())
        add_action("do_open","open");
}
int do_open(string arg)
{
        object gold;
        if(arg != "hongbao" && arg != "���") 
                return 0;
        if(query("opened"))
                return notify_fail("���С������������ǿյģ�\n");
        gold = new("/clone/money/gold");
        gold->set_amount(10+random(10));
        set("name", "����Ҷ��", gold);
        set("base_unit", "��", gold);
        gold->move(this_player());
        set("opened",1);
        return notify_fail("���С������������ó����Ŵ���Ҷ�ӣ�\n");
}