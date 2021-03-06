// jinglisuck.c

#include <ansi.h>

inherit F_DBASE;
inherit F_SSERVER;

int exert(object me, object target)
{
        int sp, dp, suck;

        if (! target) target = offensive_target(me);

        if (! objectp(target) || target == me)
                return notify_fail("你要吸取谁的精元？\n");

        if( query("id", target) == "muren" )
                return notify_fail("你要吸取谁的精元？\n");

        if( query("race", target) != "人类" )
                return notify_fail("搞错了！只有人才能有精元！\n");

        if (me->is_busy()) return notify_fail("你正忙着呢。\n");

        if( query_temp("sucked", me) )
                return notify_fail("你刚刚吸取过精力！\n");

        if( objectp(query_temp("weapon", me)) )
                return notify_fail("你必须空手才能施用化功大法吸人精元！\n");

        if ((int)me->query_skill("huagong-dafa", 1) < 90)
                return notify_fail("你的化功大法功力不够，不能吸取对方的精元！\n");

        if( query("jingli", me)<5 )
                return notify_fail("你的精力不够，不能使用化功大法。\n");

        if( query("max_jingli", me)*2>me->query_current_jingli_limit()*3 )
                return notify_fail("你的精力修为似乎已经达到了瓶颈，再吸取也是徒劳。\n");

        if( me->query_skill("huagong-dafa",1)<query("max_jingli", me)/200 )
                return notify_fail("你的化功大法太低了，再吸取也是徒劳。\n");

        if( query("max_jingli", target) <= 100 || query("no_suck", target) )
                return notify_fail(target->name() + "已经精元涣散，你已经无法从他体内吸取任何精元了！\n");

        if( query("max_jingli", target)<query("max_jingli", me)/10 )
                return notify_fail(target->name() + "的精力修为远不如你，你无法从他体内吸取精元！\n");

        message_combatd(
                HIY "$N全身骨节爆响，双臂暴长数尺，掌缘猛地向$n的天灵拍了下去！\n\n" NOR,
                me, target );

        if (living(target))
        {
                if (! target->is_killing(me))
                        if (playerp(target))
                                target->want_kill(me);
                        target->kill_ob(me);
        }

        sp=me->query_skill("force",1)*3/2+query("level", me)*20+
             me->query_skill("martial-cognize", 1);

        dp=target->query_skill("dodge")+query("level", target)*20+
             target->query_skill("martial-cognize", 1);

        set_temp("sucked", 1, me);

        if (dp / 2 + random(dp) < sp || ! living(target))
        {
                tell_object(target, HIR "你只觉天顶骨裂，精元如洪水般自会宗穴奔腾而出！\n" NOR);
                tell_object(me, HIG "你觉得" + target->name() + "的精元自手掌源源不绝地流了进来。\n" NOR);

                suck = 100 + (me->query_skill("huagong-dafa", 1) - 90) / 2;

                addn("max_jingli", -suck, target);
                addn("max_jingli", suck, me);

                if( query("max_jingli", target)<1 )
                        set("max_jingli", 0, target);

                me->start_busy(7);
                if (! target->is_busy())
                target->start_busy(7);
                addn("jingli", -5, me);
                // remove_call_out("del_sucked");
                call_out("del_sucked", 10, me);
        }
        else
        {
                message_combatd(HIY "可是$p看破了$P的企图，内力猛地一震，借势溜了开去。\n" NOR, me, target);
                me->start_busy(2 + random(5));
                // remove_call_out("del_sucked");
                call_out("del_sucked", 15, me);
        }

        return 1;
}

void del_sucked(object me)
{
        if( query_temp("sucked", me) )
                delete_temp("sucked", me);
}

