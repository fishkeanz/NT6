// chai dan

#include <weapon.h>

inherit STAFF;



void create()
{
        set_name("柴担", ({ "chai dan" }) );
        set_weight(2000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "根");
                set("long", "这是一把用来挑柴的柴担。\n");
                set("value", 300);
                set("material", "wood");
        }
        init_staff(10);

// These properties are optional, if you don't set them, it will use the
// default values.

        set("wield_msg", "$N抽出一根挑柴用的$n握在手中当武器。\n");
        set("unwield_msg", "$N放下了手中的$n。\n");

// The setup() is required.

        setup();
}
