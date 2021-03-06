// This program is a part of NITAN MudLIB
// huaxue.c

#include <ansi.h>

#define HUAXUE "「" HIR "神刀化血" NOR "」"

inherit F_SSERVER;

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        object weapon;
        string msg;
        int skill;

        if( query_temp("huaxue", me) )
                return notify_fail(HIG "你已经运起" + HUAXUE + HIG "了。\n");

        /*
        if (! me->is_fighting())
                return notify_fail(HUAXUE + "只能在战斗中使用。\n");
        */

        if( query("neili", me)<200 )
                return notify_fail("你的真气不够！\n");

        if ((int)me->query_skill("force") < 150)
                return notify_fail("你的蚩尤狂战决修为不够，不能使用" + HUAXUE + "！\n");

        weapon=query_temp("weapon", me);

        if( !objectp(weapon) || query("skill_type", weapon) != "blade" )
                return notify_fail("你没有装备刀，难以施展" + HUAXUE + "。\n");

        msg = HIR "$N" HIR "凝神闭目，右手持刀，左手顺着" + weapon->name() +
              HIR "刀刃一抹，鲜血顿时将" + weapon->name() + HIR "染红。\n待到$N"
              HIR "双目睁开，顿时杀气冲天！\n" NOR;

        skill = (int)me->query_skill("moshen-xinfa", 1);

        addn("neili", -150, me);
        addn_temp("apply/damage", skill*5, me);
        addn_temp("apply/unarmed_damage", skill*5, me);
        addn_temp("apply/blade", skill, me);
        set_temp("huaxue", 1, me);

        me->start_call_out((: call_other, __FILE__, "remove_effect",
                           me, skill :), skill);

        message_combatd(msg, me);
        return 1;
}

void remove_effect(object me, int amount)
{
        if( query_temp("huaxue", me) )
        {
                addn_temp("apply/damage", -amount*5, me);
                addn_temp("apply/unarmed_damage", -amount*5, me);
                addn_temp("apply/blade", -amount, me);
                delete_temp("huaxue", me);
                tell_object(me, "你的" + HUAXUE + "运行完毕，将内力收回丹田。\n");
        }
}