#include <ansi.h>

inherit EQUIP_SOUL;

void create()
{
        set_name(HIM "戒指之魂" NOR, ({"rings soul4", "rings", "soul4"}));
        set_weight(100);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIM "这是一个戒指之魂。\n" NOR);
                set("unit", "个");
                set("value", 200000);
                set("no_sell", 1);
        }
        set("type", "rings");
        set("status", 4);
        set("auto_load", 1);
        
        setup();
}

