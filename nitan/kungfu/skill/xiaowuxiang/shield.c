// This program is a part of NITAN MudLIB
// shield.c 护体神功

#include <ansi.h>

inherit F_CLEAN_UP;

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        int skill;

        if( target != me ) 
                return notify_fail("你只能用小无相功来运护体神功。\n");

        if( query("neili", me)<100 )
                return notify_fail("你的内力不够。\n");
        if( query_temp("shield", me) )
                return notify_fail("你已经在运功\中了。\n");

        skill = me->query_skill("force");
        addn("neili", -100, me);
        me->receive_damage("qi", 0);

        message_combatd(
        MAG "$N长吸一口气，运起护体神功，身上衣衫缓缓涨起。ⅵ\n" NOR, me);

        addn_temp("apply/armor", skill/2, me);
        // me->addn_temp("apply/defense",  skill / 2);
        set_temp("shield", 1, me);

        me->start_call_out( (: call_other, __FILE__, "remove_effect", me, skill/2 :), skill/2);

        if( me->is_fighting() ) me->start_busy(3);

        return 1;
}

void remove_effect(object me, int amount)
{
        addn_temp("apply/armor", -amount, me);
        // me->addn_temp("apply/defense", - amount);
        delete_temp("shield", me);
        tell_object(me, "你的护体神功运行完毕，将内力收回丹田。\n");
}
