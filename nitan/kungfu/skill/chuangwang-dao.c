inherit SKILL;

mapping *action = ({
([      "action": "$N����$wбָ��һ�С�ɳ�������������һ�٣�һ����$n��$l��ȥ",
        "force" : 20,
        "dodge" : 10,
        "parry" : 25,
        "damage": 5,
        "lvl"   : 0,
        "skill_name"  : "ɳ�����",
        "damage_type" : "����"
]),
([      "action": "$Nһ�С�����ն��������������㣬$wһ��һ�գ�ƽ�л���$n�ľ���",
        "force" : 30,
        "dodge" : 30,
        "parry" : 10,
        "damage": 10,
        "lvl"   : 20,
        "skill_name"  : "����ն��",
        "damage_type" : "����"
]),
([      "action": "$Nչ���鲽������Ծ�䣬һ�С�����������������һ��������ն��$n",
        "force" : 40,
        "dodge" : 35,
        "parry" : 25,
        "damage": 11,
        "lvl"   : 40,
        "skill_name"  : "��������",
        "damage_type" : "����"
]),
([      "action": "$Nһ�С�������������$w�󿪴��أ����϶��»���һ���󻡣���ֱ����$n",
        "force" : 60,
        "dodge" : 36,
        "parry" : 25,
        "damage": 17,
        "lvl"   : 60,
        "skill_name"  : "��������",
        "damage_type" : "����"
]),
([      "action": "$N����$wһ����һ�С�ǧ����������˫�ֳ����������У�����$n���ؿ�",
        "force" : 80,
        "dodge" : 34,
        "parry" : 35,
        "damage": 21,
        "lvl"   : 80,
        "skill_name"  : "ǧ������",
        "damage_type" : "����"
]),
([      "action": "$N����$w��ʹ��һ�С����ƽ��������������ã����ҿ�����������$n",
        "force" : 90,
        "dodge" : 41,
        "parry" : 38,
        "damage": 27,
        "lvl"   : 100,
        "skill_name"  : "���ƽ���",
        "damage_type" : "����"
]),
});


int valid_enable(string usage) { return usage == "blade" || usage == "parry"; }

int valid_learn(object me)
{
        if( query("max_neili", me)<50 )
                return notify_fail("���������Ϊ������\n");

        if ((int)me->query_skill("force") < 10)
                return notify_fail("����ڹ����̫ǳ��\n");

        if ((int)me->query_skill("blade", 1) < (int)me->query_skill("chuangwang-dao", 1))
                return notify_fail("��Ļ�������ˮƽ���ޣ��޷���������Ĵ���������\n");

        return 1;
}

string query_skill_name(int level)
{
        int i;
        for (i = sizeof(action)-1; i >= 0; i--)
                if (level >= action[i]["lvl"])
                        return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
        int i, level;
        level = (int) me->query_skill("chuangwang-dao", 1);
        for (i = sizeof(action); i > 0; i--)
                if (level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}

int practice_skill(object me)
{
        object weapon;

        if( !objectp(weapon=query_temp("weapon", me)) || 
            query("skill_type", weapon) != "blade" )
                return notify_fail("��ʹ�õ��������ԡ�\n");

        if( query("qi", me)<60 )
                return notify_fail("�����������������������\n");

        if( query("neili", me)<60 )
                return notify_fail("�����������������������\n");

        me->receive_damage("qi", 45);
        addn("neili", -43, me);
        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"chuangwang-dao/" + action;
}