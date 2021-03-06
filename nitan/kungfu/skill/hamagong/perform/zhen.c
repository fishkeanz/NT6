// This program is a part of NITAN MudLIB
// hama.c 蛤蟆功

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string name() { return "蟾震九天"; }

#include "/kungfu/skill/eff_msg.h";

int perform(object me, object target)
{
        int damage, p, ap, dp;
        string msg, dodge_skill;

        if (! target) target = offensive_target(me);

        if (! target || !target->is_character() ||
            ! me->is_fighting(target) ||
            ! living(target))
                return notify_fail("蟾震九天绝招只能对战斗中的对手使用。\n");

        if ((int)me->query_skill("hamagong", 1) < 100)
                return notify_fail("你的蛤蟆功还不够熟练，不能使用蟾震九天对敌！\n");

        if( query_temp("weapon", me) )
                return notify_fail("你必须空手才能使用「蟾震九天」！\n");

        if (me->query_skill_mapped("force") != "hamagong")
                return notify_fail("你的内功中无此功夫。\n");

        if (me->query_skill_prepared("strike") != "hamagong" ||
            me->query_skill_mapped("strike") != "hamagong")
                return notify_fail("你必须先将蛤蟆功运用于掌法之中才行。\n");

        if( query("max_neili", me)<1500 )
                return notify_fail("你现在内力不足，使不出蛤蟆功的绝招蟾震九天！\n");

        if( query("neili", me)<400 )
                return notify_fail("你现在内力不足，使不出蛤蟆功的绝招蟾震九天！\n");

        if ((int)me->query_skill("strike", 1) < 100)
                return notify_fail("你的掌法不够娴熟，使不出蛤蟆功的绝招蟾震九天！\n");

        if (me->query_str() < 30 )
                return notify_fail("你的力量太小，使不出用蛤蟆绝招蟾震九天！\n");

        msg = BLU "\n$N身子蹲下，左掌平推而出，使的正是$N生平最得意的「蟾震九天」绝招，掌风直逼$n而去！\n"NOR;

        ap = attack_power(me, "force");
        dp = defense_power(target, "force");

        if (ap / 2 + random(ap) > dp)
        {
                me->start_busy(1);
                if (! target->is_busy())
                        target->start_busy(1);

                addn("neili", -200, me);

                damage = damage_power(me, "force");
                if( query("neili", me)>random(query("neili", target)) )
                        damage += query("jiali", me);
                damage *= 2;

                target->receive_damage("qi", damage, me);
                target->receive_wound("qi", damage/2, me);

                p=query("eff_qi", target)*100/query("max_qi", target);
                msg += COMBAT_D->damage_msg(damage, "内伤");
                msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
                call_out("perform2", 1, me, target, p, damage);
        } else
        {
                me->start_busy(2);
                if (! target->is_busy())
                        target->start_busy(1);

                addn("neili", -100, me);
                tell_object(target, HIY"你但觉一股微风扑面而来，风势虽然不劲，然已逼得自己呼吸不畅，知道不妙，连忙跃开数尺。\n" NOR);
                dodge_skill = target->query_skill_mapped("dodge");
                if( !dodge_skill ) dodge_skill = "dodge";
                msg += SKILL_D(dodge_skill)->query_dodge_msg(target, 1);
        }
        message_combatd(msg, me, target);
        return 1;
}

int perform2(object me, object target, int p, int damage)
{
        int ap, dp;
        string msg, dodge_skill;

        if( !target || !me || !living(me)
           || !me->is_fighting(target)
           || environment(target)!=environment(me) || !living(target))
                return 0;

        if( query("neili", me)<500 )
                return notify_fail("你待要再发一掌，却发现自己的内力不够了！\n");

        msg = BLU "\n$N左掌劲力未消，右掌也跟着推出，功力相叠，「蟾震九天」掌风排山倒海般涌向$n！\n"NOR;

        ap = attack_power(me, "force");
        dp = defense_power(target, "dodge");

        if (ap / 2 + random(ap) > dp)
        {
                me->start_busy(1);
                if (! target->is_busy())
                        target->start_busy(1);

                addn("neili", -300, me);

                if( query("neili", me)>random(query("neili", target)) )
                        damage += query("jiali", me);
                damage *= 2;
                target->receive_damage("qi", damage, me);
                target->receive_wound("qi", damage/2, me);
                p=query("qi", target)*100/query("max_qi", target);
                msg += COMBAT_D->damage_msg(damage, "内伤");
                msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
                call_out("perform3", 1, me, target, p, damage);
        }
        else
        {
                me->start_busy(2);
                if (! target->is_busy())
                        target->start_busy(1);
                addn("neili", -200, me);
                tell_object(target, HIY"你喘息未定，又觉一股劲风扑面而来，连忙跃开数尺，狼狈地避开。\n" NOR);
                dodge_skill = target->query_skill_mapped("dodge");
                if( !dodge_skill ) dodge_skill = "dodge";
                msg += SKILL_D(dodge_skill)->query_dodge_msg(target, 1);
        }
        message_vision(msg, me, target);
        return 1;
}

int perform3(object me, object target, int p, int damage)
{
        int ap, dp;
        string msg, dodge_skill;

        if(! target || ! me || environment(me) != environment(target)) return 1;
        if(!living(target))
              return notify_fail("对手已经不能再战斗了。\n");

        if( query("neili", me)<700 )
                return notify_fail("你待要再发一掌，却发现自己的内力不够了！\n");

        msg = BLU "\n$N双腿一登，双掌相并向前猛力推出，$n连同身前方圆三丈全在「蟾震九天」劲力笼罩之下！\n"NOR;

        ap = attack_power(me, "force");
        dp = defense_power(target, "parry");

        if (ap / 2 + random(ap) > dp)
        {
                me->start_busy(1);
                if (! target->is_busy())
                        target->start_busy(2);

                addn("neili", -400, me);

                if( query("neili", me)>random(query("neili", target)) )
                        damage += query("jiali", me);
                damage *= 2;
                target->receive_damage("qi", damage, me);
                target->receive_wound("qi", damage/2, me);
                p=query("eff_qi", target)*100/query("max_qi", target);
                msg += COMBAT_D->damage_msg(damage, "瘀伤");
                msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
        } else
        {
                me->start_busy(2);
                if (! target->is_busy())
                        target->start_busy(1);
                addn("neili", -300, me);
                addn("jingli", -100, target);
                tell_object(target, HIY"你用尽全身力量向右一纵一滚，摇摇欲倒地站了起来，但总算躲开了这致命的一击！\n" NOR);
                dodge_skill = target->query_skill_mapped("dodge");
                if( !dodge_skill ) dodge_skill = "dodge";
                msg += SKILL_D(dodge_skill)->query_dodge_msg(target, 1);
        }

        message_combatd(msg, me, target);
        return 1;
}
