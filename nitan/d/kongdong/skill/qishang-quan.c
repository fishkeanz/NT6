// qishang-quan

inherit SKILL;

mapping *action = ({
    ([  "action":"$N左手握拳护胸，右手使出一招「烘云托月」，穿出击向$n的$l",
        "lian"  :"$N左手握拳护胸，右手使出一招「烘云托月」，慢慢体会其中韵味。",
        "zhaoshi":"「烘云托月」",
        "jibie" : 5,
        "sen" : 5,
        "damage" : 5,
        "damage_type":  "瘀伤"
    ]),
    ([  "action":"$N右手虚晃，使出一招「虚敲实应」，左掌化虚为实击向$n的$l",
        "lian"  :"$N右手虚晃，使出一招「虚敲实应」，左掌化虚为实，缓缓击出。",
        "zhaoshi" : "「虚敲实应」",
        "jibie" : 10,
        "sen"  : 10,
        "damage" : 10,
        "damage_type":  "瘀伤"
    ]),
    ([  "action":"$N使出七伤拳法「行云流水」，弓步上前，双掌同时拍向$n的$l",
        "lian" : "$N使出七伤拳法「行云流水」，弓步向前，双掌向空中一拍。",
        "zhaoshi" : "「行云流水」",
        "jibie" : 20,
        "sen" : 15,
        "damage" : 15,
        "damage_type":  "瘀伤"
    ]),
    ([  "action":"$N使出一招「大落墨」，身形腾空跃起，对准$n的$l连续拍出三掌",
        "lian"  :"$N使出一招「大落墨」，身形腾空跃起，对着空气乱拍一通。",
        "zhaoshi" : "「大落墨」",
        "jibie" : 30,
        "sen" : 20,
        "damage" : 20,
        "damage_type":  "瘀伤"
    ]),
    ([  "action":"$N左掌扬起，右掌五指一伸一划，一招「倒卷帘」击向$n的$l",
        "lian"  :"$N左掌扬起，右掌五指一伸一划，使出一招「倒卷帘」，好象在捏苍蝇。",
        "zhaoshi" : "「倒卷帘」",
        "jibie" : 35,
        "sen" : 30,
        "damage" : 30,
        "damage_type":  "瘀伤"
    ]),
    ([  "action":"$N身形飞舞，一招「哨探寻」，双掌同时向$n的$l出掌攻击",
        "lian" : "$N身形飞舞，使出一招「哨探寻」，双掌一出即回，虎虎生风。",
        "zhaoshi" : "「哨探寻」",
        "jibie" : 40,
        "sen" : 40,
        "damage" : 50,
        "damage_type":  "瘀伤"
    ]),
    ([  "action":"$N大喝一声，一招「连山断岭」，掌力倾吐，掌风罩向$n全身",
        "lian" : "$N大喝一声，一招「连山断岭」，掌力倾吐，化出一阵掌风，足以开碑裂石。",
        "zhaoshi" : "「连山断岭」",
        "jibie" : 50,
        "sen" : 50,
        "damage" : 60,
        "damage_type":  "瘀伤"
    ]),
});

string *parry_msg = ({
    "$n双手轻点，一招「拨云见日」，轻而易举的挡开了$N的一击。\n",
    "只见$n足不点地，一招「顺水推舟」，将$N的攻势化于无形。\n",
    "但见$n不退反进，双手一扬，使出一招「叶底摘花」，立刻化险为夷。\n",
});
    
int valid_lian(object me)
{   
    if (me->query_temp("weapon"))
        return notify_fail("练七伤拳必须空手。\n");
    if ( me->query("str") < 18 )
        return notify_fail("你体质太弱，无法练七伤拳。\n");
    if ( me->query("force") < 5 )
        return notify_fail("你内力不足，一练便有生命危险。\n");
    me->receive_damage("kee",5);
    me->add("force",-5);
    return 1;
}

int valid_learn(object me)
{
    if ( me->query_temp("weapon") || me->query_temp("secondary_weapon") )
            return notify_fail("师父说道：“你先把手中兵器放下。”\n");
    if ( (int)me->query("max_force") < 100 )
            return notify_fail("你的内力太弱，练七伤拳会走火入魔。\n");
    return 1;
}

mapping query_action(int jibie)
{ 	
    int i, size;
	size = sizeof( action );
	for ( i = 0; i < sizeof( action ); i++ )
		if ( action[ i ][ "jibie" ] > jibie )
		{
			size = i;
			break;
		}
    return action[ random( size ) ];
}

string query_parry_msg(object weapon)
{   
    return parry_msg[random(sizeof(parry_msg))];
}

string perform_action_file(string action)
{   
    return SKILL_D("qishang-quan/") + action;
}

string query_xiuwei_type() { return "unarmed"; }

int query_dengji() { return 5; }

string query_zhaoshi( int jibie )
{
    int i;
    for ( i = 0; i < sizeof(action); i++ )
    {
        if ( action[ i ][ "jibie" ] >= jibie )
            return action[ i ][ "zhaoshi" ];
    }
    return "";
}
