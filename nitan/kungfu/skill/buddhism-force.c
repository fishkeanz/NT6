// buddhism-force.c

#include <ansi.h>

inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }
int valid_force(string force) { return 1; }

int query_neili_improve(object me)
{
        int lvl;
        lvl = (int)me->query_skill("buddhism-force", 1);
        return lvl * lvl * 22 * 15 / 100 / 200;
}

int valid_learn(object me)
{
        int lvl;

        lvl = (int)me->query_skill("buddhism-force", 1);

        if( query("gender", me) == "����" )
                return notify_fail("���޸����ԣ�������������������ղ����񹦡�\n");

        if( query("sex/times", me) || query("gender", me) != "����" )
                return notify_fail("ѧ��ղ����񹦱�����ͯ��֮����\n");

        return ::valid_learn(me);
}

int practice_skill(object me)
{
        return notify_fail("��ղ�����ֻ����ѧ(learn)�������������ȡ�\n");
}
string exert_function_file(string func)
{
        return __DIR__"buddhism-force/" + func;
}


void skill_improved(object me)
{
        if( (int)me->query_skill("buddhism-force", 1) == 120 &&
             !query("sl_gift/con", me) )
        {
                addn("con", 1, me);
                set("sl_gift/con", 1, me);
                tell_object(me, HIM "\n��Ľ�ղ�����ѧ�����ɣ��������ĸ��ǡ�\n" NOR);
        }
}