inherit SKILL;

string *dodge_msg = ({
        "$nһʽ����ɽ��ˮ�������ӳ���һת��$Nֻ������ǰһ����ʧȥ��Ŀ�ꡣ\n",
        "$nһʽ����ڤ���ġ���˫�ŵ�أ�����ͻȻ�θ�������������Ʈ����$N����\n",
        "$nһʽ�����Ŀ��С�������һ�Σ���ʱ��������Ӱһ���ӳ�����$N����ǰ��\n",
        "$nһʽ�����ڤ�ǡ������弱ת����Ϊһ���׹⣬��ò����ĳ�����$N����Χ��\n",
});

int valid_enable(string usage)
{
        return usage == "dodge" || usage == "move";
}

int valid_learn(object me)
{
        return 1;
}

string query_dodge_msg(string limb)
{
        return dodge_msg[random(sizeof(dodge_msg))];
}

int practice_skill(object me)
{
        if( query("qi", me)<45 )
                return notify_fail("�������̫���ˣ���������Ӱ������\n");

        me->receive_damage("qi", 35);
        return 1;
}