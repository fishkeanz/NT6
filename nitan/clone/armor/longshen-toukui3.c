// toukui.c ͷ��
 
#include <armor.h>
#include <ansi.h>

inherit HEAD;
 
void create()
{
        set_name(HIM "����ͷ��" NOR, ({ "longshen toukui", "longshenshen3", "toukui3" }) );
        set_weight(1000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("material", "steel");
                set("unit", "��");
                set("long", HIM "����һ�����������ͷ�������Ա���ͷ����\n" NOR);
                set("value", 3500000);
                set("armor_prop/armor", 400);
                set("limit", ([
                        "dex"    :   65,
                ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}