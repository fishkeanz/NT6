#include <ansi.h>
#include <combat.h>

#define HANXING "��" HIY "���ǰ˴�" NOR "��"

#include "/kungfu/skill/eff_msg.h";

inherit F_SSERVER;

int perform(object me, object target)
{
        int skill, i, p, n;
        int my_exp, ob_exp;
        string pmsg;
        string msg;
        object weapon;

        if( userp(me) && !query("can_perform/hanxing-throwing/hanxing", me) )
                return notify_fail("����ʹ�õ��⹦��û�����ֹ��ܡ�\n");

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(HANXING "ֻ����ս���жԶ���ʹ�á�\n");

        if( !objectp(weapon=query_temp("handing", me)) || 
            query("skill_type", weapon) != "throwing" )
                return notify_fail("����������û�����Ű���������ʩչ" HANXING "��\n");

        if (weapon->query_amount() < 8)
                return notify_fail("����Ҫ����ö��������ʩչ" HANXING "��\n");

        if ((skill = me->query_skill("hanxing-bada", 1)) < 100)
                return notify_fail("��ĺ��ǰ˴򲻹���죬����ʩչ" HANXING "��\n");

        if( query("neili", me)<100 )
                return notify_fail("�������������㣬����ʩչ" HANXING "��\n");

        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        addn("neili", -80, me);
        weapon->add_amount(-3);

        msg= HIY "$N" HIY "ͻȻ�������һ�����������ڿ���ص�һ˲"
             "�䣬$n" HIY "��Ȼ���ּ��㺮����˸������Ϯ���Լ���\n" NOR;

        me->start_busy(2);
        my_exp=query("combat_exp", me)+skill*skill/10*skill;
        ob_exp=query("combat_exp", target);
        if (random(my_exp) > ob_exp)
        {
                msg += HIR "���$p" HIR "��Ӧ����������$P" + HIR "һ" +
                       query("base_unit", weapon)+weapon->name()+
                       HIR "��\n" NOR;
                target->receive_wound("qi", skill / 3 + random(skill / 3), me);
                COMBAT_D->clear_ahinfo();
                weapon->hit_ob(me,target,query("jiali", me)+120);

                p=query("qi", target)*100/query("max_qi", target);

                if (stringp(pmsg = COMBAT_D->query_ahinfo()))
                        msg += pmsg;
                        msg += "( $n" + eff_status_msg(p) + " )\n";
                message_combatd(msg, me, target);
        } else
        {
                msg += HIG "����$p" HIG "���ݲ��ȣ����ɵ�������$P" HIG "������" +
                       weapon->name() + HIG "��\n" NOR;
                message_combatd(msg, me, target);
        }

        me->reset_action();
        return 1;
}