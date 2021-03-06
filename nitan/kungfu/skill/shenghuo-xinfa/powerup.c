#include <ansi.h>

inherit F_CLEAN_UP;

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        int skill;

        if (target != me)
                return notify_fail("你只能用圣火心法来提升自己的战斗力。\n");

        if ((int)query("neili", me) < 150)
                return notify_fail("你的内力不够。\n");

        if ((int)query_temp("powerup", me))
                return notify_fail("你已经在运功中了。\n");

        skill = me->query_skill("force");
        addn("neili", -150, me);
        message_combatd(HIM "$N默运圣火心法，脸色先由黄翻紫，紧接着由紫翻蓝，再由蓝翻红，最后又恢复为黄色，甚为诡异。\n" NOR,me);
        addn_temp("apply/attack", skill*2/5, me);
        addn_temp("apply/dodge", skill*2/5, me);
        addn_temp("apply/parry", skill*2/5, me);
        set_temp("powerup", 1, me);

        me->start_call_out((:call_other, __FILE__, "remove_effect", me, skill * 2 / 5 :), skill);
        if (me->is_fighting()) me->start_busy(3);
        return 1;
}

void remove_effect(object me, int amount)
{
        if (query_temp("powerup", me))
        {
                addn_temp("apply/attack", -amount, me);
                addn_temp("apply/dodge", -amount, me);
                addn_temp("apply/parry", -amount, me);
                delete_temp("powerup", me);
                tell_object(me, "你的圣火心法运行完毕，长长地吐了口气，将内力收回丹田。\n");
        }
}
