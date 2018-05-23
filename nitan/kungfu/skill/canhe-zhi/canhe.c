// This program is a part of NITAN MudLIB

#include <ansi.h>
#include <combat.h>
#include "/kungfu/skill/eff_msg.h";

string name() { return HIW "�κϽ���" NOR; }

inherit F_SSERVER;

int perform(object me, object target)
{
        int damage;
        string msg;
        int ap, dp, slv, clv,p;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(name() + "ֻ�ܶ�ս���еĶ���ʹ�á�\n");

        if( objectp(query_temp("weapon", me)) )
                return notify_fail("�������ֲ���ʹ��" + name() + "��\n");

        if (clv = (int)me->query_skill("canhe-zhi", 1) < 180)
                return notify_fail("��Ĳκ�ָ��Ϊ���ޣ�����ʩչ" + name() + "��\n");

        if (me->query_skill_prepared("finger") != "canhe-zhi")
                return notify_fail("������û��׼��ʹ�òκ�ָ������ʩչ" + name() + "��\n");

        if ((int)me->query_skill("force") < 320)
                return notify_fail("����ڹ���Ϊ̫�����ʩչ" + name() + "��\n");

        if( query("max_neili", me)<6000 )
                return notify_fail("���������Ϊ���㣬����ʩչ" + name() + "��\n");

        if( query("neili", me)<900 )
                return notify_fail("�����������������ʩչ" + name() + "��\n");

        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        damage = damage_power(me, "finger");
        slv = target->query_skill("six-finger", 1);

        ap = attack_power(me, "finger");
        dp = defense_power(target, "dodge");

        msg = HIW "ֻ��$N" HIW "ʮָ��̯����ʱ�ƿ������죬���ɽ�����ָ�⼤"
              "���������$n" HIW "��ֱ��ȥ��\n" NOR;

        if (slv >= 140
            && random(5) == 0
            && slv >= clv - 60 // ����κ�ָ�ȼ��������񽣵ȼ���60������ȡ������Ч��
            &&! target->is_busy()
            && target->query_skill_prepared("finger") == "six-finger")
        {
                msg += HIY "\n����$n" HIY "ббһָ�����ָ����磬�����ݺᣬ��Ȼ"
                       "���죬����$N" HIY "�Ľ���ȫ���ۻأ������Լ���ȥ��\n" NOR +
                       HIR "��������������һ�����죬���Ͼ�����һЩѪ�Σ�\n" NOR;

                me->receive_wound("qi", slv / 3 + random(slv / 4), target);
                p=query("qi", me)*100/query("max_qi", me);
                msg += "( $N" + eff_status_msg(p) + ")\n";

        } else
        if (ap / 2 + random(ap) > dp)
        {
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 150,
                                           HIR "\n��ʱֻ������������һ����$n" HIR
                                           "����������������ʱ���ض���������һ��"
                                           "Ѫ�ꡣ\n" NOR);
        } else
        {
                msg += CYN "\n$n" CYN "��$N" CYN "������ӿ����æ����һԾ��"
                       "�𣬱ܿ�����һ����\n" NOR;
        }

        ap = attack_power(me, "finger");
        dp = defense_power(target, "force");

        if (slv >= 160
            && random(8) == 0
            && slv >= clv - 60  // ����κ�ָ�ȼ��������񽣵ȼ���60������ȡ������Ч��
            &&! target->is_busy()
            && target->query_skill_prepared("finger") == "six-finger")
        {
                msg += HIY "\n����$n" HIY "����Сָһ�죬һ�С����󽣡���ָ��͸��"
                       "�������ĵ�������Ѹ�٣��ٽ�$N" HIY "�����ƻأ�\n" NOR + HIR
                       "��������������һ�����죬���Ͼ�����һЩѪ�Σ�\n" NOR;

                me->receive_wound("qi", slv / 2 + random(slv / 4), target);
                p=query("qi", me)*100/query("max_qi", me);
                msg += "( $N" + eff_status_msg(p) + ")\n";

        } else
        if (ap / 2 + random(ap) > dp)
        {
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 155,
                                           HIR "\nֻ��$n" HIR "һ���Һ�����$N" HIR
                                           "�Ľ���������Ҫ����Ѫ��ģ������Ѫ������"
                                           "ֹ��\n" NOR);
        } else
        {
                msg += CYN "\n$n" CYN "��$N" CYN "������ӿ����æ����һԾ��"
                       "�𣬱ܿ�����һ����\n" NOR;
        }

        ap = attack_power(me, "finger");
        dp = defense_power(target, "parry");

        if (slv >= 180
            && random(10) == 0
            && slv >= clv - 50  // ����κ�ָ�ȼ��������񽣵ȼ���50������ȡ������Ч��
            &&! target->is_busy()
            && target->query_skill_prepared("finger") == "six-finger")
        {
                msg += HIY "\n�ɵ���ʯ֮�䣬$n" HIY "��Ȼ���ƣ����ֶ�Ȼ̽������"
                       "ָ���г彣����$N" HIY "һ�����ǽ��κϽ����������Σ�\n" NOR
                       + HIR "��������������һ�����죬���Ͼ�����һЩѪ�Σ�\n" NOR;

                me->receive_wound("qi", slv / 2 + random(slv / 2), target);
                p=query("qi", me)*100/query("max_qi", me);
                msg += "( $N" + eff_status_msg(p) + ")\n";
                me->start_busy(6);

        } else
        if (ap / 2 + random(ap) > dp)
        {
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 160,
                                           HIR "\n$n" HIR "�����мܣ����ǲ��У�$N"
                                           "��" HIR "���ν�����͸����룬��Ѫ����"
                                           "��������ս��\n" NOR);
        } else
        {
                msg += CYN "\n$n" CYN "��$N" CYN "������ӿ����æ����һԾ��"
                       "�𣬱ܿ�����һ����\n" NOR;
        }
        me->start_busy(3+random(2));
        addn("neili", -400-random(100), me);
        message_combatd(msg, me, target);
        return 1;
}