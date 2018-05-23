// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

inherit NPC;
inherit F_MASTER;
inherit F_GUARDER;

#include <ansi.h>
#include "xuedao.h"

mixed ask_skill1();

void create()
{
        set_name("ʤ��", ({ "sheng di", "sheng", "di" }));
        set("long", @LONG
����һ�����ο��������ɮ�ˣ���ɫ��Į����
����ͬ������ߵ�һ�ˡ�
LONG);
        set("gender", "����");
        set("age", 35);
        set("attitude", "peaceful");
        set("shen_type", -1);
        set("str", 22);
        set("int", 21);
        set("con", 24);
        set("dex", 22);
        set("max_qi", 5000);
        set("max_jing", 2500);
        set("neili", 2800);
        set("max_neili", 2800);
        set("jiali", 30);
        set("combat_exp", 250000);

        set_skill("force", 160);
        set_skill("mizong-neigong", 160);
        set_skill("xuehai-mogong", 160);
        set_skill("parry", 160);
        set_skill("blade", 140);
        set_skill("xuedao-daofa", 140);
        set_skill("sword", 160);
        set_skill("mingwang-jian", 160);
        set_skill("dodge", 160);
        set_skill("shenkong-xing", 160);
        set_skill("hand", 140);
        set_skill("dashou-yin", 140);
        set_skill("cuff", 140);
        set_skill("yujiamu-quan", 140);
        set_skill("lamaism", 160);
        set_skill("literate", 140);
        set_skill("martial-cognize", 100);

        map_skill("force", "mizong-neigong");
        map_skill("dodge", "shenkong-xing");
        map_skill("hand", "dashou-yin");
        map_skill("cuff", "yujiamu-quan");
        map_skill("parry", "mingwang-jian");
        map_skill("sword", "mingwang-jian");
        map_skill("blade", "xuedao-daofa");

        prepare_skill("cuff", "yujiamu-quan");
        prepare_skill("hand", "dashou-yin");

        set("chat_chance_combat", 100);
        set("chat_msg_combat", ({
                (: perform_action, "sword.ruo" :),
                (: perform_action, "sword.xiang" :),
                (: perform_action, "hand.yin" :),
                (: perform_action, "cuff.jiang" :),
                (: exert_function, "recover" :),
        }) );

        create_family("Ѫ����", 5, "����");
        set("class", "bonze");

        set("inquiry", ([
                // "���ӿ���" : (: ask_skill1 :),
        ]));

        set("coagents", ({
                ([ "startroom" : "/d/xuedao/shandong3",
                   "id" : "xuedao laozu" ]),
        }));

        set("master_ob", 4);
        setup();
        carry_object("/d/xueshan/obj/y-jiasha")->wear();
        carry_object("/clone/weapon/changjian")->wield();

        add_money("silver", 10);
}

void attempt_apprentice(object ob)
{
        if (! permit_recruit(ob))
                return;

        if( query("class", ob) != "bonze" )
        {
                set_temp("pending/join_bonze", 1, ob);
                command("say �㲻�ǳ����ˣ�����Ѫ�����ǲ��������ġ�"
                        "�����Ҫ���ң���������ʦ�ܡ�");
                return;
        }

        if( query("shen", ob)>0 )
        {
                command("say ������������������Ļ�����\n");
                this_object()->kill_ob(ob);
                return;
        }

        if ((int)ob->query_skill("lamaism", 1) < 50)
        {
                command("say ����Ѫ���ţ���ϰ���ڷ�����Ҫ�ġ�\n");
                return;
        }

        if ((int)ob->query_skill("force") < 70)
        {
                command("say �������������ô���Ȱ��ڹ����ã�\n");
                return;
        }

        command("recruit "+query("id", ob));
}

mixed ask_skill1()
{
        object me;

        me = this_player();

        if( query("can_perform/xuedao-daofa/kai", me) )
                return "�Ҳ��Ƕ��Ѿ��̹�������";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ�����Ѫ���ŵģ��������������㣡";

        if (me->query_skill("xuedao-daofa", 1) < 1)
                return "����ѩɽ������ûѧ��������ʲô��";

        if( query("family/gongji", me)<50 )
                return "����˵�ˣ�զ�ǵľ��������޹�֮�ˡ�";

        if( query("shen", me)>-3000 )
                return "�������Ĵ�����������ѧ����������ʲô�ã�";

        if (me->query_skill("force") < 100)
                return "����ڹ���Ϊʵ��̫���ȥ������������";

        if( query("max_neili", me)<600 )
                return "������������������Ȼ������˼�����ң�";

        if (me->query_skill("xuedao-daofa", 1) < 80)
                return "�����ѩɽ�����������������Ұɡ�";

        message_vision(HIY "$n" HIY "΢΢һЦ����$N" HIY "���������ֹ��˼��䣬Ȼ��"
                       "˳�ְγ���\n��ĸյ������ն�˼��£���ʱֻ������һչ������"
                       "��ʽɷ\n�Ǿ��档$N" HIY "������������ĵ��˵�ͷ��\n" NOR, me,
                       this_object());

        command("nod");
        command("say ����������ˣ��㿴���˶��٣�");
        tell_object(me, HIC "��ѧ���ˡ����ӿ��⡹��\n" NOR);

        if (me->can_improve_skill("blade"))
                me->improve_skill("blade", 1500000);
        if (me->can_improve_skill("xuedao-daofa"))
                me->improve_skill("xuedao-daofa", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/xuedao-daofa/kai", 1, me);
        addn("family/gongji", -50, me);

        return 1;
}