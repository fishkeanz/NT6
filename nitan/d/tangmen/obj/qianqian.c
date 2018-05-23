// qianqianbiao.c                       ����ǧǧ��

#include <weapon.h>
#include <ansi.h>
inherit ITEM;

void create()
{
        set_name("����ǧǧ��" NOR, ({"biao"}));
        set_weight(50);

        if (clonep())
                set_default_object(__FILE__);
        else
        {
                set("unit", "ö");
                set("long", "����һö�Ĵ��������Ƶİ����������ޱȡ�\n");
                set("value", 0);
        }
        setup();
}

void init()
{
        add_action("do_perform","use");
}

int do_perform(string arg)
{
        object me, target;
        string msg;

        me = this_player();
        
        me->clean_up_enemy();
        target = me->select_opponent();

        if ( !arg || arg != "qian")
                return 0;
        
        if ( !(me->is_fighting()) )
                return notify_fail("����ǧǧ��ֻ����ս����ʹ�á�\n");
 
        if( (int)me->query_skill("tangmen-throwing", 1) < 180 )
                return notify_fail("������Ű����ַ�������죬�޷�ʹ������ǧǧ�ڡ�\n");

        if( query("max_neili", me)<3000 )
                return notify_fail("���������Ϊ�������޷�ʹ������ǧǧ�ڡ�\n");

        if( query_temp("xinqian", me) )
                return notify_fail("��æ��ս�����޷�����ʹ������ǧǧ���ˡ�\n");

        if( query("neili", me)<1500 )
                return notify_fail("�������������\n");
        
        if( query("yanli", me)<100 )
                return notify_fail("�������̫���ˣ�Ŀ�겻��ȷ��\n");       

        msg = HIR"\n$NͻȻ����һֹ���ӻ�������һö���ƴ�С�İ�����һ������$n��ȥ��\nֻ��$n�����������������Ĺ�Ӱ��ȴ������һ˿������\n"NOR;
        message_vision(msg, me, target);
        addn("neili", -1500, me);
        set_temp("xinqian", 1, me);

        tell_object(target, HIR"\n�㼱æ��������ϣ���ܹ��㿪��������һ����\n"NOR);
        target->start_busy(6);

        remove_call_out("effect_biao");
        call_out("effect_biao", 8, me, target);

        return 1;
}

int effect_biao(object me, object target)
{
        int myskill, trskill;
        string msg;

        myskill = (int) (2 * (int)me->query_skill("dodge") + (query("combat_exp",me) / 1000) + (int)me->query_skill("tangmen-biaofa", 1)); 
        trskill = (int) (2 * (int)target->query_skill("dodge") + (query("combat_exp",target) / 1000)); 

        if ( ( myskill >= trskill ) && ( random(100) != 0 ) )
                {
                        msg = HIR"��Ȼ�������Ĺ�Ӱһ����û��$n����һ�٣����һ����Ѫ�����������\n"NOR;
                        message_vision(msg, me, target);
                        tell_object(target, HIR"��ֻ�����ؿ�һ�����ĵ���ʹ����ͷһ��ֻ����ö�����Ѿ������Ƕ������Ŀڡ�\nѪ������Լ�м���С��[����ǧǧ...]��"NOR);
                        delete_temp("xinqian", me);
                        me->start_busy(5);
                        this_object()->move(target);
                        target->unconcious();
                }
        else
                {
                        tell_object(target, HIR"��Ȼ�������Ĺ�Ӱһ����û��������һ����æ��������ȫ����\n"NOR);
                        msg = HIR"$n˫�ۼ��裬��������Ʒ�֮����ֻ������һ�����죬������ö�����ķɿ�ȥ��\n"NOR;
                        message_vision(msg, me, target);
                        if( query("neili", target)<800 )
                                set("neili", 0, target);
                        else
                                addn("neili", -1000, target);
                        delete_temp("xinqian", me);
                        me->start_busy(3);
                        destruct(this_object());
                }
        return 1;
}