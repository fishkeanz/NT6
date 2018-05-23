// This program is a part of NITAN MudLIB
// suoming.c ��������

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;
string name() { return "��������"; }

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int damage;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail("������������ֻ����ս���жԶ���ʹ�á�\n");

        if( !objectp(weapon=query_temp("weapon", me)) ||
              query("skill_type", weapon) != "blade" )
                return notify_fail("��ʹ�õ��������ԡ�\n");

        if (me->query_skill("force") < 200)
                return notify_fail("����ڹ�����Ϊ����������ʹ����һ������\n");

        if (me->query_skill("xiuluo-dao", 1) < 135)
                return notify_fail("������޵�����Ϊ������Ŀǰ����ʹ������������\n");

        if( query("neili", me)<250 )
                return notify_fail("�����������������ʹ������������\n");

        if (me->query_skill_mapped("blade") != "xiuluo-dao")
                return notify_fail("��û�м������޵���������ʹ������������\n");

       if (! living(target))
              return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = HIC "$N" HIC "����ɱ����ʢ��һ�����е�" + weapon->name() +
              HIC "����������$n" + HIC "���Ź�ס��\n" NOR;

        ap = attack_power(me, "blade");
        dp = defense_power(target, "parry");

        if (ap / 2 + random(ap) > dp)
        {
                damage = damage_power(me, "blade");
                addn("neili", -180, me);
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 45,
                                           HIR "������$p" HIR "�ҽ�������һ����Ѫ����" HIR
                                           "���׵ĵ����н�����\n" NOR);
                me->start_busy(1 + random(2));
        } else
        {
                addn("neili", -60, me);
                me->start_busy(3);
                msg += CYN "����$p" CYN "�����ֿ죬ֻ����������������һ��"
                       CYN "������$p" CYN "��$P" CYN "����ʽȫ��������\n" NOR;
        }
        message_combatd(msg, me, target);

        return 1;
}