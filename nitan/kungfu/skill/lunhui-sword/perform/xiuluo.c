#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string name() { return HIR "���޽���" NOR; } 

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int damage;
        string sexs;
         
        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(name() + "ֻ����ս���жԶ���ʹ�á�\n");

        if( !objectp(weapon=query_temp("weapon", me)) || 
              query("skill_type", weapon) != "sword" )
                return notify_fail("��ʹ�õ��������ԣ�����ʩչ" + name() + "��\n");

        if (me->query_skill("lunhui-sword", 1) < 500)
                return notify_fail("��������ֻؽ���Ϊ����������ʩչ" + name() + "��\n");

        if (me->query_skill_mapped("sword") != "lunhui-sword")
                return notify_fail("��û�м��������ֻؽ�������ʩչ" + name() + "��\n");

        if (me->query_skill_mapped("sword") != "lunhui-sword")
                return notify_fail("��û�м��������ֻؽ�������ʩչ" + name() + "��\n");

        if( query("neili", me)<2000 )
                return notify_fail("�����������������ʩչ" + name() + "��\n");

        if (! living(target))
               return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        sexs=query("gender", me) == "����"?"�����������":"����Ů����������";
        msg = HIR "$N" HIR "����������ɲ�Ǽ䣬�������ƣ���̬Ʈ��֮�������紫˵�е�" + sexs + "����Ȼ�䣬$N" HIR 
              "����" + weapon->name() + HIR "����һ�����⣬����һ�����ν�������$n" HIR "������һ˿Ԥ�ס�\n" NOR;

        ap = attack_power(me, "sword"); 
        dp = defense_power(target, "parry");
        
        if (ap * 4 / 5 + random(ap) > dp)
        {
                damage = damage_power(me, "sword");
                damage *= 3;
                addn("neili", -2000, me);
                me->start_busy(2 + random(4));
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 400,
                                           HIR "ֻ��$n" HIR "һ���ҽУ�$N" HIR +
                                           weapon->name() + HIR "����һ����һ������Ƕ"
                                           "��$p" HIR "���ڳ���������һ��Ѫ�ꡣ\n"
                                           NOR);
                if (random(3) == 1)
                {
                        target->receive_damage("jing", damage / 40, me);
                        target->receive_wound("jing", damage / 80, me);
                        target->start_busy(3 + random(4));
                        msg = msg + target->name() +  HIM "ͻȻ�о���ͷ��ľѣ���������ܵ������޽�����Ӱ�졣\n" NOR;
                }
        } else
        {
                addn("neili", -1000, me);
                me->start_busy(4);
                msg += CYN "$n" CYN "��$P" CYN "������ӿ���������׵�"
                       "������æ������Ų������������\n" NOR;
        }
        message_combatd(sort_msg(msg), me, target);

        return 1;
}