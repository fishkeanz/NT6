// $file_name.c                                                 /* COMMENT_ENTRY */
// Create by TEMPLATE_D. $time.                                 /* COMMENT_ENTRY */
// skull.c
// Written by Lonely                                            /* COMMENT_EXIT  */

#include <ansi.h>

inherit TESSERA;

void create()
{
//**    set_name("$name", ({ "$id", }));                        /* NAME_ENTRY */
        set_name(HIW "��֮ˮ����" NOR, ({ "magic diamond" }) );  /* NAME_EXIT  */
        set_weight(20);
        if( clonep() )
                set_default_object(__FILE__);
        else {
//**            set("$dbase_key", "$dbase_value\n");            /* DBASE_ENTRY */
                set("long", HIW "һ�Ż���������Ĺ�ʵ���ʯ�������Բ�ͬѰ���ĸо���\n" NOR);  /* DBASE_EXIT  */
                set("value", 200000);
                set("unit", "��");
                set("level", 4);
                set("material", "mdiamond");
                set("can_be_enchased", 1);
                set("magic/type", "metal");
                set("magic/power", 500);
                set("auto_load", 1);
        }
        set("no_identify", 1);
	setup();
}
