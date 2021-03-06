//影子剑法.
//中级-->终极武功。

inherit SKILL;
#include <ansi.h>

mapping *action=({
([ "action":"$N身行突然暴起，一招「无影飞剑」以快速无比的速度刺向$n的$l处！",
    "apply_factor": 2,
    "name" : "无影飞剑", 
    "damage_type":"刺伤",
  ]),
 ([ "action":"$N左晃右闪，令人琢磨不定，一招「无影无踪」突然一剑刺向$n的$l！",
    "apply_factor": 5,
    "name" : "无影无踪", 
    "damage_type":"刺伤",
  ]),
  ([ "action":"$N聚气凝神，$w忽然猛地刺向$n的$l，正是一招「天外飞影」！",
    "apply_factor": 10,
    "name" : "天外飞影",
    "damage_type":"刺伤",
  ]),
  ([ "action":"$N的$w忽然化做万千剑影，从四面八方刺向$n的$l，正是一招「千剑寻命」！",
    "apply_factor": 7,
    "name" : "千剑寻命",
    "damage_type":"刺伤",
  ])});   

string *parry_weapon=({

"$N的$w上充满了杀气，完全不顾及自己的身体，拼命一招挡开$n的$v！\n",
"$N手中的$w犹如毒蛇一般，从诡异的角度刺来，迫得$n收回$v！\n",
"$N阴险地一笑，$w划着诡异的线路挡开了$n的$v。\n",
"$N将全身的力量集中于$w之上，猛地一刺，正好将$n的$v挡开！\n"

});

string *parry_hand=({

"$N的招式诡异无比，犹如鬼魅一般，一招闪电般的招数封住了$n的进攻！\n",
"$N身形犹如毒蛇，招式犹如鬼魅，在$n摸不清动向之际，$N已然用$w封住了$n的进攻！\n",
"$N突然沙哑的一叫，犹如鬼哭狼嚎，手中$w趁机递出，挡住了$n的进攻。\n",
"$N完全不顾$n的进攻，手中$w一折，刺向$n的额头，$n一惊，连忙抽身撤了回来！\n"

});

string query_parry_action(object me,object victim)
{
   if(victim->query_temp("weapon"))
    return parry_weapon[random(sizeof(parry_weapon))];
   return parry_hand[random(sizeof(parry_hand))];

}

mapping query_action(object me,object weapon)
{
   return action[random(sizeof(action))];
 
}
mapping *query_all_action(object me)
{
return action;	
}

int practice_skill(object me)  
{
   object weapon;
   
   if(me->query_skill("yingzi-jianfa",1)<200)
    return notify_fail("只有影子剑法达到二百级后才可以练习提高！\n");

        if (!objectp(weapon = me->query_temp("weapon"))
        || (string)weapon->query("skill_type") != "sword")
                return notify_fail("你使用的武器不对。\n");
 
 return 1;  
}

string query_type()    
{                      
	return "sword";
}
string query_base()  
{
	return "sword";
}

int valid_learn(object me)
{
	object weapon;
	
	if(me->query("job_office_number/杀手任务")<10)
	 return notify_fail("你的杀手任务功绩太低了，还不能学习影子剑法！\n");
	
	if(me->query_skill("yingzi-jianfa",1)>=me->query("job_office_number/杀手任务")*5)
	 return notify_fail("你必须继续提高杀手任务功绩，才能继续学习影子剑法！\n");
	
	if(!objectp(weapon=me->query_temp("weapon")))
     return notify_fail("手中无剑，如何学习影子剑法？\n");
	 if(weapon->query("skill_type")!="sword")
	return notify_fail("手中无剑，如何学习影子剑法？\n");
	return 1;
}
mapping query_select_action(int i)
{
	if((i)>sizeof(action))
		return 0;
	else return action[i-1];
}


string *query_perform()
{
	return ({"ci","xunming"}); 
}


int skill_damage(object me,object victim)
{
  if(me->query("perform_quest/ci"))
  return 210;
  return 80;
}
