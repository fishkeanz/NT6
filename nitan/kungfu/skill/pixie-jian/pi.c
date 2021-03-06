#include <ansi.h>
#include <combat.h>

#define PI "「" HIW "群邪辟易" NOR "」"

inherit F_SSERVER;

string *finger_name = ({ "左手中指", "左手无名指", "左手食指",
                         "右手中指", "右手无名指", "右手食指", }); 

int perform(object me, object target)
{
        string msg;
        string name;
        object weapon;
        int i;
        int skill;
        int count;
        
        /*
        if( userp(me) && !query("can_perform/pixie-jian/pi", me) )
                return notify_fail("你所使用的外功中没有这种功能。\n");
        */
        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! me->is_fighting(target))
                return notify_fail(PI "只能对战斗中的对手使用。\n");

        weapon=query_temp("weapon", me);

        if( weapon && query("skill_type", weapon) != "sword" && 
            query("skill_type", weapon) != "pin" )
                return notify_fail("你使用的武器不对，难以施展" PI "。\n");

        if( query_temp("weapon", me) )
                name = "手中" + weapon->name();
        else
                name = finger_name[random(sizeof(finger_name))];

        skill = me->query_skill("pixie-jian", 1);

        if (skill < 120)
                return notify_fail("你的辟邪剑法不够娴熟，难以施展" PI "。\n");

        if( query("neili", me)<200 )
                return notify_fail("你现在的真气不足，难以施展" PI "。\n");

        if (weapon && me->query_skill_mapped("sword") != "pixie-jian")
                return notify_fail("你没有准备使用辟邪剑法，难以施展" PI "。\n");

        if (! weapon && me->query_skill_prepared("unarmed") != "pixie-jian")
                return notify_fail("你没有准备使用辟邪剑法，难以施展" PI "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        msg = HIW "$N" HIW "身形忽然变快，蓦的冲向$n" HIW "，" + name +
              HIW "幻作数道虚影，顿时无数星光一齐射向$n" HIW "！\n" NOR;
        message_combatd(msg, me, target);

        count = skill / 4;
        addn_temp("apply/attack", count, me);
        addn_temp("apply/damage", count, me);
        addn_temp("apply/unarmed_dage", count, me);

        addn_temp("pixie-jian/hit_msg", 1, me);

        for (i = 0; i < 9; i++)
        {
                if (! me->is_fighting(target))
                        break;

                if (random(3) == 0 && ! target->is_busy())
                        target->start_busy(1);

                COMBAT_D->do_attack(me, target, weapon, 0);
        }
        addn("neili", -100, me);
        me->start_busy(1 + random(7));
        addn_temp("apply/attack", -count, me);
        addn_temp("apply/damage", -count, me);
        addn_temp("apply/unarmed_dage", -count, me);
        delete_temp("pixie-jian/hit_msg", me);
        return 1;
}
