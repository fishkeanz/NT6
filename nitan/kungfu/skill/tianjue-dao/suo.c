#include <ansi.h>
#include <combat.h>

#define SUO "��" HIW "�����" NOR "��"

inherit F_SSERVER;

int perform(object me, object target)
{
        string msg, wn;
        object weapon;
        int level;

        if( userp(me) && !query("can_perform/tianjue-dao/suo", me) )
                return notify_fail("����ʹ�õ��⹦��û�����ֹ��ܡ�\n");

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(SUO "ֻ�ܶ�ս���еĶ���ʹ�á�\n");

        if( !objectp(weapon=query_temp("weapon", me)) || 
            query("skill_type", weapon) != "blade" )
                return notify_fail("��ʹ�õ��������ԣ�����ʩչ" SUO "��\n");

        if (target->is_busy())
                return notify_fail(target->name() + "Ŀǰ���Թ˲�Ͼ���ŵ������ɡ�\n");

        if ((int)me->query_skill("tianjue-dao", 1) < 50)
                return notify_fail("���������������죬����ʩչ" SUO "��\n");

        if (me->query_skill_mapped("blade") != "tianjue-dao")
                return notify_fail("��û���������������ʩչ" SUO "��\n");

        if( query("neili", me)<120 )
                return notify_fail("�����ڵ���������������ʩչ" SUO "��\n");

        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        wn = weapon->name();
                           
        msg = HIC "\n$N" HIC "ŭХһ����ʩչ�����С�" HIW "�����" HIC "��"
              "������" + wn + HIC "������ܲ�͸�磬��Ȼ���������������$n" HIC
              "�����ڵ���֮�¡�"NOR;

        message_sort(msg, me, target);

        level = me->query_skill("tianjue-dao", 1);

        addn("neili", -120, me);

        if (level + random(level) > target->query_skill("dodge"))
        {
                msg = HIY "$N" HIY "������$n" HIY "��ʽ�е���ʵ����æ"
                      "��ס�Լ�ȫ����һʱ������Ӧ�ԣ�\n" NOR; 
                target->start_busy(2 + random(level / 26));
                me->start_busy(random(2));
        } else
        {
                msg = CYN "����$N" CYN "��������С�Ĳ��У�û�б�"
                      "$n" NOR + CYN "��ʽ������\n" NOR;
                      
                me->start_busy(2);
        }
        message_combatd(msg, target, me);

        return 1;
}