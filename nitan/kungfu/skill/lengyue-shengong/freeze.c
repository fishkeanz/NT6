// This program is a part of NITAN MudLIB
// freeze.c

#include <ansi.h>

inherit F_SSERVER; 

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        int ap;
        int dp;
        int damage;
        string msg;

        if (target == me || ! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! me->is_fighting(target))
                return notify_fail("你只能用寒气攻击战斗中的对手。\n");

        if (me->query_skill("lengyue-shengong", 1) < 150)
                return notify_fail("你的冷月神功火候不够，无法运用寒气。\n");

        if( query("neili", me)<1000 )
                return notify_fail("你的内力不够!");

        msg = HIW "$N" HIW "默运冷月神功，一股寒气迎面扑向$n"
              HIW "，四周登时雪花飘飘。\n" NOR;

        ap=attack_power(me, "force");
        dp=defense_power(target, "force");

        me->start_busy(2);

        if (ap / 2 + random(ap) > random(dp))
        {
                damage = damage_power(me, "force");
                target->receive_damage("qi", damage, me);
                target->receive_wound("qi", damage/2, me);
                if( query("neili", target)>damage )
                        addn("neili", damage, target);
                else
                        set("neili", 0, target);

                msg += HIG "$n" HIG "忽然觉得一阵透骨寒意，霎时间"
                       "浑身的血液几乎都要凝固了。\n" NOR;
                if (! target->is_busy()) target->start_busy(1);

                set_temp("lengyue-shengong_freeze", 1, target);
                target->start_call_out((: call_other, __FILE__, "remove_effect", target :),
                                       me->query_skill("lengyue-shengong") / 5);
        } else
                msg += HIY "$n" HIY "感到一阵寒意自心底泛起，连忙"
                       "运动抵抗，堪勘无事。\n" NOR;

        message_combatd(msg, me, target);

        return 1;
}

void remove_effect(object me)
{
        if ((int)query_temp("lengyue-shengong_freeze", me))
        {
                delete_temp("lengyue-shengong_freeze", me);
                tell_object(me, HIY "你感到身边的寒气渐渐散去了。\n" NOR);
        }
}

