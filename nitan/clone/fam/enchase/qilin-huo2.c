inherit ITEM;
#include <ansi.h>

void create()
{
        set_name(HIR "�����" NOR, ({"qilin huo", "qilin", "huo" }));
        set_weight(1000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("value", 500000);
                set("long", HIR "��˵������������޺��������������������ֳ��ȵĸо���\n" NOR);
                set("enchase/name", "�����");
                set("enchase/point", 5);
                set("enchase/type", "medal2");
                set("enchase/cur_firm", 100);
                set("enchase/spe_data", ([]) );  
        }
        setup();
}

int query_autoload()
{
        return 1;
}

