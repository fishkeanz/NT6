
inherit ITEM;
#include <ansi.h>
int do_fu(string arg);
void create()
{
   set_name(HIR"养颜丹"NOR, ({ "yangyan dan","dan" }));
   set_weight(100);
   set("unit", "颗");
   set("long","这是一颗由多种美容物质提炼而成的丹药,服用(fu dan)它能美容.\n");
   set("value", 150000);
   
}

void init()
{
	add_action("do_fu","fu");
}
int do_fu(string arg)
{
	object me,ob;int i;
	ob=this_object();me=this_player();
	if(!arg||(arg!="dan"&&arg!="丹"))
		return notify_fail("你要服用什么?\n");
                            if(me->query("per")>30)
		return notify_fail("你都这么美了，还养什么颜，不让别人活了啊！\n");
	me->add("per",1);
	message_vision("$N服下了一颗[养颜丹]，感觉到自己变美了。\n",me);
	destruct(ob);
	return 1;
}