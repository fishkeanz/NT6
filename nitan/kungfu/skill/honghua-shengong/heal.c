// heal.c
// Last Modified by winder on Mar. 10 2000

#include <ansi.h>

int exert(object me, object target)
{
  if ( userp(me) && !wizardp(me) && 
  !query("perform/heal", me) && 
  !query("can_perform/honghua-shengong/heal", me) && 
  !query_temp("murong/xingyi", me) )
   return notify_fail("����ʹ�õ��ڹ���û�����ֹ��ܡ�");

        if( me->is_fighting() )
                return notify_fail("ս�����˹����ˣ�������\n");

        if( me->is_busy() )
                return notify_fail("��������æ���أ����п��˹���\n");

        if( (int)me->query_skill("honghua-shengong", 1) < 20)
                return notify_fail("��ĺ컨����Ϊ��������\n");

        if( query("neili", me)<50 )
                return notify_fail("�������������\n");
/*
        if( query("eff_qi", me) >= query("max_qi", me) )
                return notify_fail(HIR"��û�����ˣ��������������ˣ�\n"NOR);
*/
        if( query("eff_qi", me)<query("max_qi", me)/2 )
                return notify_fail("���Ѿ����˹��أ�ֻ��һ��������������Σ�գ�\n");

        write( HIW "��ȫ�����ɣ���������ʼ�˹����ˡ�\n" NOR);
        message("vision", HIW + me->name() + "�³�һ����Ѫ����ɫ�������ö��ˡ�\n" NOR, environment(me), me);

        me->receive_curing("qi", 10 + (int)me->query_skill("force")/5 );
        addn("neili", -50, me);
        set("jiali", 0, me);
        me->start_busy(1);

        return 1;
}

int help(object me)
{
        write(WHT"\n�컨��֮���ƣ�"NOR"\n");
        write(@HELP

        ʹ�ù�Ч��
                Ϊ�Լ�����

        ����Ҫ��
                �컨��20��
                ����50
HELP
        );
        return 1;
}