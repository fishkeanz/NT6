inherit ITEM;
#include <ansi.h>

void create()
{
        set_name(HIW "��ħ������" NOR, ({"tianmo yaojing", "yaojing" }));
        set_weight(900);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIW "һ�龧Ө�ı�ʯ����������ȴ��һ�ֲ���ĸо���\n" NOR);
                set("unit", "��");
                set("value", 50000);
                set("enchase/name", "��ħ������");
                set("enchase/point", 90);
                set("enchase/type", "all");
                set("enchase/cur_firm", 90);
                set("enchase/spe_data", ([]) );  
        }
        setup();
}

int query_autoload()
{
        return 1;
}
