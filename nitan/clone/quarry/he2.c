#include <ansi.h>
inherit QUARRY;

void create()
{
        set_name(HIW "ѩ��" NOR, ({ "xue he", "xue", "he" }));
        set("long", HIW "ȫ����׵�ѩ�ף��Ǵ�ѩɽ���ز���\n" NOR);
        set("no_auto_kill", 1);
        set("aves", 1);

        set("age", 1);
        set("str", 5);
        set("dex", 28);
        set("max_qi", 150);
        set("max_jing", 150);
        set("combat_exp", 1000);

        set("power", 5);

        set_temp("apply/dodge", 60);
        set_temp("apply/defense", 60);

        setup();
}