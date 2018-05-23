inherit NPC;
void create()
{
        string *names = ({"С��"});
        set_name( names[random(sizeof(names))], ({ "baby tiger","tiger"}));
        set("vendetta_mark","tiger");
        set("race", "Ұ��");
        set("gender", "����");
        set("age", 20);
        set("long", "����һֻ�ɰ���С����\n");

        set("str", 40);
        set("cor", 100);
        set("cps", 22);
        set("max_qi", 2000);
        set("max_jing", 1000);
        set("max_neili", 3000);
        set("attitude", "peaceful");
/*
        set("chat_chance", 2);
        set("chat_msg", ({
                "С������������צ��ʾ����\n"
        }) );
*/
        set("limbs", ({ "ͷ��", "����", "ǰ��", "���", "β��" }) );
        set("verbs", ({ "bite", "claw" }) );
        set("combat_exp", 100000);
        set("bellicosity", 5 );

        set_temp("apply/attack", 30);
        set_temp("apply/parry", 30);
        set_temp("apply/unarmed_damage", 30);
        setup();
        carry_object(__DIR__"obj/tiger_skin");
}