
inherit ITEM;
#include <ansi.h>
int do_fu(string arg);
void create()
{
   set_name(HIR"死人头"NOR, ({ "head"}));
   set_weight(100);
   set("unit", "颗");
   set("long","这是一颗死人头,吃了他(eat head)会变得很邪恶.\n");
   set("value", 50000);
   
}

void init()
{
	add_action("do_fu","eat");
}
int do_fu(string arg)
{
	object me,ob;int i;
	ob=this_object();me=this_player();
	if(!arg||(arg!="head"&&arg!="头"))
		return notify_fail("你要吃什么?\n");
	me->add("bellicosity",100);
	message_vision("$N抓起一颗[死人头]狂啃起来，满脸都透露着邪气。\n",me);
	destruct(ob);
	return 1;
}