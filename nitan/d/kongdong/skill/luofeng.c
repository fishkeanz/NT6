// 落风六式    luofeng.c
//   created by qyz on Feb 4, 1997

inherit SKILL;

mapping *action = ({
        ([     
                "action":               
"只见$n身随意转，一招「浮云无定」，倏地往一旁挪开了三尺，避过了$N这一招。",
                "lian" :
"$N身随意转，一招「浮云无定」，倏地往一旁挪开了三尺。",
                "jibie":          8,
                "sen"  :      5,
                "dodge":                0,
                "parry":                0,
                "damage":                15,
                "damage_type":  "瘀伤"
        ]),
        ([      "action":
"$n使出一招「轻舟泛月」，向一旁飘然纵出，轻轻着地。",
                "lian" :
"$N使出一招「轻舟泛月」，向一旁飘然纵出，轻轻着地。",
                "jibie":15,
                "sen"  :5,
                "dodge":                0,
                "parry":                0,
                "damage":                20,
                "damage_type":  "瘀伤"
        ]),
        ([      
                "action":               
"$n身形微晃，一招「落日浮云」，有惊无险地避开了$N这一招。",
                "lian" :
"$N身形微晃，一招「落日浮云」，向一边闪去。",
                "jibie":25,
                "sen"  : 5,
                "dodge":                0,
                "parry":                0,
                "damage":                15,
                "damage_type":  "瘀伤"
        ]),
        ([    
                "action":               
"$n漫不经心的向左一个「日落沙明」，刚好避过$N的凌厉攻势。",
                "lian" :
"$N漫不经心的向左一个「日落沙明」飘了过去。",
                "jibie":33,
                "sen"  : 5,
                "dodge":                0,
                "parry":                0,
                "damage":                20,
                "damage_type":  "瘀伤"
        ]),
        ([      
                "action":               
"$n身形向后一纵，使出一招「落叶秋风」，避过了$N的攻击。",
                "lian" :
"$N身形向后一纵，使出一招「落叶秋风」，退后丈馀。",
                "jibie":40,
                "sen"  : 6,
                "dodge":                0,
                "parry":         0,
                "damage":                15,
                "damage_type":  "瘀伤"
        ]),
        ([      
                "action":               
"可是$n使一招「风飘落日」，身子轻轻飘了开去。",
                "lian" :
"$N使一招「风飘落日」，身子轻轻向后飘了开去。",
                "jibie":50,
                "sen"  : 6,
                "dodge":                0,
                "parry":         0,
                "damage":                15,
                "damage_type":  "瘀伤"
        ]),
});
/*
string *parry_msg = ({
        "$n一招「盘古开天」，提腿立足，双掌变爪，架开了$N。\n",
        "$n右拳全力击出，迫使$N不得不收招自卫。\n",
        "$n架开了$N的攻势。\n",
});
*/
int valid_learn(object me)
{
    return 1;
}

int valid_lian( object me )
{
//        int encu;
//        encu=me->query_encumbrance() * 100 / me->query_max_encumbrance();
//        if( encu < 10 )
//        return notify_fail("轻功必须负重练习。\n");

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

string query_xiuwei_type()
{
	return "dodge";
}

// 返回这路武功的等级, 第十等返回0, 第一等返回9.
int query_dengji()
{
	return 3;
}
/*
string query_parry_msg(object weapon)
{   
    if(!weapon)
        return parry_msg[random(sizeof(parry_msg))];
}
*/
int effective_level(){  return 10; }

string *query_lian()
{
	string *lian_msg;
	mapping lian_act;

	lian_msg = allocate( sizeof( action ) );
	for ( int i = 0; i < sizeof( action ); i++ )
	{
		lian_act = action[ i ];
		lian_msg[ i ] = (string)lian_act[ "lian" ];
	}
	return lian_msg;
}

