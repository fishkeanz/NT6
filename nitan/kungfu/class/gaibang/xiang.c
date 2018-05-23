// xiang.c

#include <ansi.h>;
inherit NPC;
inherit F_MASTER;

#include "fight.h"

void create()
{
        set_name("���", ({"xiang zhanglao", "xiang", "zhanglao"}));
        set("nickname", HIR "ؤ������" NOR);
        set("gender", "����");
        set("title", "ؤ��Ŵ�����");
        set("age", 59);
        set("attitude", "peaceful");
        set("class", "beggar");
         set("beggarlvl", 9);

        set("str", 30);
        set("int", 24);
        set("con", 30);
        set("dex", 24);

        set("qi", 2000);
        set("max_qi", 2000);
        set("jing", 800);
        set("max_jing", 800);
        set("neili", 2000);
        set("max_neili", 2000);
        set("jiali", 200);

        set("combat_exp", 1500000);
        set("score", 200000);
        set("shen", 100000);

        set_skill("force", 150);             // �����ڹ�
        set_skill("huntian-qigong", 150);    // ��������
        set_skill("dodge", 150);                   // ��������
        set_skill("xiaoyaoyou", 150);        // ��ң��
        set_skill("parry", 150);             // �����м�
        set_skill("hand", 150);              // �����ַ�
        set_skill("suohou-hand", 150);       // ����������
        set_skill("blade", 150);             // ��������
        set_skill("liuhe-dao", 150);       // �������ϵ�
        set_skill("begging", 100);           // �л�����
        set_skill("checking", 100);          // ����;˵
        set_skill("training", 100);          // Ԧ����
        set_skill("strike",85);  // �����Ʒ�
        set_skill("lianhua-zhang",85); // ������
        set_skill("dragon-strike",85);

        map_skill("strike","lianhua-zhang");
        prepare_skill("strike","lianhua-zhang");

        map_skill("force", "huntian-qigong");
        map_skill("hand",  "suohou-hand");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("blade", "liuhe-dao");
        map_skill("parry", "liuhe-dao");
        prepare_skill("hand",  "suohou-hand");

        //set("party/party_name", HIC"ؤ��"NOR);
        //set("party/rank", RED"�Ŵ�����"NOR);
        //set("party/level", 9);
        create_family("ؤ��", 18, "����");

        setup();

        carry_object(WEAPON_DIR"gangdao")->wield();
        carry_object(__DIR__"obj/cloth")->wear();
}

void attempt_apprentice(object ob)
{
        command("hmm");
        command("say �߿����Ҳ���ͽ��");
}