//顽石诀。


inherit SKILL;
#include <ansi.h>

string query_type()
{
  return "knowledge";
}

int valid_learn(object me)
{
  
  if(!me->query("execute_quest/wanshijue",1)&&
     me->query("vocation")!="杀手")
   return notify_fail("你现在无法领会顽石诀的奥秘！\n");
  
  return 1;
  
}

string *query_execute()
{
   return ({"huanmo","wuxing"});
}
