// This program is a part of NITAN MudLIB
// hufei.c ���

#include <ansi.h>
#include "hu.h"

#define LENGYUE  "/clone/unique/lengyue"
#define JUNDAO   "/clone/unique/cwbdao"
#define QUANJING "/clone/book/hujia1"
#define DAOPU    "/clone/book/hujia2"

inherit NPC;
inherit F_MASTER;
inherit F_COAGENT;
inherit F_QUESTER;
// inherit F_NOCLONE;

mixed ask_baodao();
mixed ask_jundao();
mixed ask_quanjing();
mixed ask_daopu();

mixed ask_skill1();
mixed ask_skill2();
mixed ask_skill3();
mixed ask_skill4();
mixed ask_skill5();
mixed ask_skill6();
mixed ask_skill7();
mixed ask_skill8();
mixed ask_skill9();
mixed ask_skill10();
mixed ask_skill11();

mixed  try_to_learn_baishengdao();
mixed  try_to_learn_hujiadao();
mixed  try_to_learn_sixiangbu();

string ask_book();

void create()
{
        object ob;
        set_name("���", ({"hu fei", "hu", "fei"}));
        set("gender", "����");
        set("nickname", HIW "ѩɽ�ɺ�" NOR);
        set("age", 25);
        set("long", @LONG
�����Ǻ�һ��֮�Ӻ�쳣������书��ǿ�����
û���ڽ�����������š�ѩɽ�ɺ�����������
һ����ɫ�������������һ�ѿ������ܾɵĵ���
������ף�����������һͷŨ����ȴ����衣
LONG
        );
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("str", 45);
        set("int", 30);
        set("con", 30);
        set("dex", 30);

        set("qi", 5200);
        set("max_qi", 5200);
        set("jing", 3500);
        set("max_jing", 3500);
        set("neili", 6500);
        set("max_neili", 6500);
        set("jiali", 180);
        set("combat_exp", 6000000);
        set("level", 50);
        set("score", 20000);

        set_skill("force", 640);
        set_skill("lengyue-shengong", 640);
        set_skill("blade", 660);
        set_skill("baisheng-daofa", 660);
        set_skill("hujia-daofa", 660);
        set_skill("dodge", 660);
        set_skill("sixiang-bufa", 660);
        set_skill("parry", 640);
        set_skill("throwing", 660);
        set_skill("mantian-xing", 660);
        set_skill("strike", 640);
        set_skill("tianchang-zhang", 640);
        set_skill("cuff", 640);
        set_skill("hujia-quan", 640);
        set_skill("literate", 480);
        set_skill("martial-cognize", 600);
        set_skill("daojian-xiaotianwai", 800);

        map_skill("force", "lengyue-shengong");
        map_skill("blade", "hujia-daofa");
        map_skill("dodge", "sixiang-bufa");
        map_skill("parry", "hujia-daofa");
        map_skill("throwing", "mantian-xing");
        map_skill("strike", "tianchang-zhang");
        map_skill("cuff", "hujia-quan");

        prepare_skill("strike", "tianchang-zhang");
        prepare_skill("cuff",   "hujia-quan");

        set("no_teach", ([
                "baisheng-daofa" : (: try_to_learn_baishengdao :),
                "hujia-daofa"    : (: try_to_learn_hujiadao :),
                "sixiang-bufa"   : (: try_to_learn_sixiangbu :),
        ]));

        create_family("�������", 7, "����");

        set("inquiry", ([
                "���±���"   : (: ask_baodao :),
                "��������"   : (: ask_jundao :),
                "����ȭ��"   : (: ask_quanjing :),
                "����"       : (: ask_book :),
                "���ҵ���"   : (: ask_daopu :),
                // "�������"   : (: ask_skill1 :),
                // "��������Ͼ���" : (: ask_skill1 :),
                "��ȭ���"   : (: ask_skill2 :),
                // "����̽��"   : (: ask_skill3 :),
                // "�����Ը�"   : (: ask_skill4 :),
                "��������"   : (: ask_skill5 :),
                "�ǹ���˸"   : (: ask_skill6 :),
                // "�۰�Ⱥɽ"   : (: ask_skill7 :),
                // "������"     : (: ask_skill8 :),
                "������ӡ��" : (: ask_skill9 :),
                "ж��׶����" : (: ask_skill10 :),
                "�˷��ص���" : (: ask_skill11 :),
                "��ɨǧ��"   : "�ף�����ʲô��������û��˵����",
                "Ԭ����"     : "���ˣ��������������ˡ�",
                "�ֻ�"       : "�ֻ���������ҷ�ɱ�������ɡ�",
                "�԰�ɽ"     : "�������簡�����������⿴���ء� ",
                "������"     : "��֪ʲôʱ�����ټ����������",
                "����"       : "��֪ʲôʱ�����ټ����������",
                "��һ��"     : "�ɶ�������һ�����ҵ��������Ҵ�û��������",
                "������"     : "������Ҳ��֪���������ֻ����������˵������Ҫȥ������",
                "���˷�"     : "���˷�ͼҸ����������ɳ�Ϊ֪�����ѣ�Ȼ���Ҹ�ȴ�������Ľ��¡�"
        ]));

        set("chat_chance_combat", 120);
        set("chat_msg_combat", ({
                (: perform_action, "blade.xian" :),
                (: perform_action, "blade.zhui" :),
                (: perform_action, "blade.cang" :),
                (: perform_action, "cuff.kuai" :),
                (: perform_action, "strike.jian" :),
                (: perform_action, "dodge.fei" :),
                (: perform_action, "throwing.shan" :),
                (: perform_action, "throwing.xing" :),
                (: exert_function, "recover" :),
                (: exert_function, "powerup" :),
        }));

        set_temp("apply/damage", 100);
        set_temp("apply/unarmed_damage", 100);
        set_temp("apply/armor", 200);

        set("master_ob",5);
        setup();

        if (clonep())
        {
                ob = find_object(LENGYUE);
                if (! ob) ob = load_object(LENGYUE);
                if (! environment(ob))
                {
                        ob->move(this_object());
                        ob->wield();
                } else
                {
                        ob = find_object(JUNDAO);
                        if (! ob) ob = load_object(JUNDAO);
                        if (! environment(ob))
                        {
                                ob->move(this_object());
                                ob->wield();
                        } else
                        {
                                ob = new("/clone/weapon/gangdao");
                                ob->move(this_object());
                                ob->wield();
                        }
                }
        }

        carry_object("/clone/cloth/cloth")->wear();
        set("startroom", "/d/guanwai/xiaowu");
        // check_clone();
}

void attempt_apprentice(object ob)
{
        if (! permit_recruit(ob))
                return;

        if (ob->name(1) == "��һ��")
        {
                command("say �Ҳ٣��������ҵ����Ӱ���");
                return;
        }

        if (ob->name(1) == "���")
        {
                command("say �����̵ģ���զҲ��������֣�");
                return;
        }

        if (ob->name(1) == "����" || ob->name(1) == "����")
        {
                command("say �����������֣������������Ҳ̫�����ˡ�");
                return;
        }

        command("recruit "+query("id", ob));
        command("say �����ұ������㣬���Ǻ��һ�Ҫ��������������");
        return;
}

mixed try_to_learn_baishengdao()
{
        object ob = this_player();

        if( query("can_learn/hufei/baisheng-daofa", ob) )
                return 0;

        if( query("combat_exp", ob)>100000 )
        {
                command("say �����������������ұ㴫�����ʤ������");
                set("can_learn/hufei/baisheng-daofa", 1, ob);
                return 0;
        }

        return "���ʱ����̫ǳ���һ����ȴ�����һЩ�򵥵ĵ�����";
}

mixed try_to_learn_hujiadao()
{
        object ob = this_player();

        if( query("can_learn/hufei/hujia-daofa", ob) )
                return 0;

        if( query("combat_exp", ob)>600000 )
        {
                command("say ��Ȼ������������裬�ҽ��ձ㴫������ҵ�����");
                set("can_learn/hufei/hujia-daofa", 1, ob);
                return 0;
        }

        return "���ʵս���鲻�㣬����������������ҵ�����";
}

mixed try_to_learn_sixiangbu()
{
        object ob = this_player();

        if( query("can_learn/hufei/sixiang-bufa", ob) )
                return 0;

        if( query("combat_exp", ob)>50000 )
        {
                command("say �������书�Ѿ�����һ���Ļ����������Ҿʹ������󲽷���");
                set("can_learn/hufei/sixiang-bufa", 1, ob);
                return 0;
        }

        return "���󲽷�������˵���ڼ���㻹���ȵ춨�û�����˵��";
}

/*
mixed ask_skill1()
{
        object me;

        me = this_player();

        if( query("can_perform/hua-quan/juan", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if (me->query_skill("hua-quan", 1) < 1)
                return "����������ȭ��δ��ѧ�������������У�";

        if( query("family/gongji", me)<350 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if( query("shen", me)<5000 )
                return "�������������ò������������Ȳ�æ���㡣";

        if (me->query_skill("force") < 180)
                return "����ڹ�����㣬ѧ�������С�";

        if (me->query_skill("hua-quan", 1) < 120)
                return "���������ȭ���δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "���˵�ͷ��˵���������Ƚ��⻪ȭ����"
                       "���������㣬�������\n��ᡣ��˵�������$n" HIY
                       "���е�������������Ͼ��죬���ǵ�ͷ��\n��ȭ����"
                       "�ƿ绢������������չ��ͥ��վ��Τ���ױ�����ǰ��"
                       "�ѱ�\n���ź����š��������������������ƿ绢����"
                       "������Գ͵�Ұ���ͥ��\n�����Խ�����ש������$N" HIY
                       "��һ��ϸϸ׽���������㣬�������á�\n" NOR, me,
                       this_object());
        command("nod");
        command("say �����Ի�ȭ�����ŵ����ݲμ������˴�ᣬ�ǴΡ���");
        command("sigh");
        command("say ��������ȭ�����Ƴ���������ȷ�������ɵö໨������ϰ��");
        tell_object(me, HIC "��ѧ���ˡ���������Ͼ��졹��\n" NOR);

        if (me->can_improve_skill("cuff"))
                me->improve_skill("cuff", 1500000);
        if (me->can_improve_skill("hua-quan"))
                me->improve_skill("hua-quan", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/hua-quan/juan", 1, me);
        addn("family/gongji", -350, me);
        return 1;
}
*/

mixed ask_skill2()
{
        object me;

        me = this_player();

        if( query("can_perform/hujia-quan/kuai", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if (me->query_skill("hujia-quan", 1) < 1)
                return "��������ȭ����δ��ѧ�������������У�";

        if( query("family/gongji", me)<100 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if( query("shen", me)<2000 )
                return "�������������ò������������Ȳ�æ���㡣";

        if (me->query_skill("force") < 100)
                return "����ڹ�����㣬ѧ�������С�";

        if (me->query_skill("hujia-quan", 1) < 80)
                return "��ĺ���ȭ�����δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "��΢���˵�ͷ���������һ��������"
                       "����ǰ��ȭ�е���������\n���ף���ʱ����$N" HIY
                       "������ȭ����ʽ��Ϊ���\n" NOR, me,
                       this_object());
        command("nod");
        command("say ��������");
        tell_object(me, HIC "��ѧ���ˡ���ȭ��򡹡�\n" NOR);

        if (me->can_improve_skill("cuff"))
                me->improve_skill("cuff", 1500000);
        if (me->can_improve_skill("hujia-quan"))
                me->improve_skill("hujia-quan", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/hujia-quan/kuai", 1, me);
        addn("family/gongji", -100, me);
        return 1;
}

mixed ask_skill5()
{
        object me;

        me = this_player();

        if( query("can_perform/sixiang-bufa/fei", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if (me->query_skill("sixiang-bufa", 1) < 1)
                return "�������󲽷���δ��ѧ����������ʲô��";

        if( query("family/gongji", me)<1000 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if( query("shen", me)<12000 )
                return "�������������ò������������Ȳ�æ���㡣";

        if (me->query_skill("force", 1) < 150)
                return "����ڹ�����㣬ѧ�������С�";

        if( query("max_neili", me)<1000 )
                return "���������Ϊ��������������������";

        if (me->query_skill("dodge", 1) < 150)
                return "��Ļ����Ṧ�д���ߣ���ʱ��ѧ�������С�";

        if (me->query_dex() < 33)
                return "�������̫���������ϰ��";

        if (me->query_skill("sixiang-bufa", 1) < 150)
                return "������󲽷����δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "��$N" HIY "���˵�ͷ���ȵ�������"
                       "���ˡ����������䣬ֻ��$n" HIY "���\n������"
                       "�����һԾ�������ζ�ʱ���Ʈ������������׽"
                       "����$N" HIY "\nԽ��Խ�����棬�������Ը�̾��\n"
                       NOR, me, this_object());

        command("haha");
        command("say �ɿ������ˣ�");
        tell_object(me, HIC "��ѧ���ˡ��������С���\n" NOR);

        if (me->can_improve_skill("dodge"))
                me->improve_skill("dodge", 1500000);
        if (me->can_improve_skill("sixiang-bufa"))
                me->improve_skill("sixiang-bufa", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/sixiang-bufa/fei", 1, me);
        addn("family/gongji", -1000, me);
        return 1;
}

mixed ask_skill6()
{
        object me;

        me = this_player();

        if( query("can_perform/mantian-xing/shan", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if (me->query_skill("mantian-xing", 1) < 1)
                return "���������Ƕ�δ��ѧ�������������У�";

        if( query("family/gongji", me)<200 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if( query("shen", me)<3000 )
                return "�������������ò������������Ȳ�æ���㡣";

        if (me->query_skill("force") < 100)
                return "����ڹ�����㣬ѧ�������С�";

        if (me->query_skill("mantian-xing", 1) < 100)
                return "��������ǻ��δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "����һЦ����$N" HIY "������ǰ����"
                       "���ڶ���ϸ�����������ǰ�����\n�������Ҫ�㡣��"
                       "����������֣�ʮָ���ţ�һ��һ������������\nһ"
                       "�ֺ��ر�İ������š�\n" NOR, me, this_object());

        command("nod");
        command("say �ղŰ�˵���㶼��ס����");
        tell_object(me, HIC "��ѧ���ˡ��ǹ���˸����\n" NOR);

        if (me->can_improve_skill("throwing"))
                me->improve_skill("throwing", 1500000);
        if (me->can_improve_skill("mantian-xing"))
                me->improve_skill("mantian-xing", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/mantian-xing/shan", 1, me);
        addn("family/gongji", -200, me);
        return 1;
}
mixed ask_skill7()
{
        object me;

        me = this_player();

        if( query("can_perform/chuangwang-dao/xiong", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if (me->query_skill("chuangwang-dao", 1) < 1)
                return "��������������δ��ѧ�������������У�";

        if( query("gongxian", me)<50 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if( query("shen", me)<500 )
                return "�������������ò������������Ȳ�æ���㡣";

        if (me->query_skill("force") < 60)
                return "����ڹ�����㣬ѧ�������С�";

        if (me->query_skill("chuangwang-dao", 1) < 40)
                return "��Ĵ����������δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "Ц��Ц�����ֽ�$N" HIY "�е���ǰ"
                       "��������$N" HIY "�����ֹ��˰��졣Ȼ��\n�ְγ�"
                       "�����������£��ƺ���һ����Ϊ���͵ĵ�����\n"
                       NOR, me, this_object());

        command("nod");
        command("say ���������ڳ������ģ����ֶԿ�ȴ�����á�");
        tell_object(me, HIC "��ѧ���ˡ��۰�Ⱥɽ����\n" NOR);

        if (me->can_improve_skill("blade"))
                me->improve_skill("blade", 1500000);
        if (me->can_improve_skill("chuangwang-dao"))
                me->improve_skill("chuangwang-dao", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/chuangwang-dao/xiong", 1, me);
        addn("gongxian", -50, me);
        return 1;
}
/*
mixed ask_skill8()
{
        object me;

        me = this_player();

        if( query("can_perform/liuhe-dao/shan", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if (me->query_skill("liuhe-dao", 1) < 1)
                return "�������ϵ�����δ��ѧ�������������У�";

        if( query("family/gongji", me)<200 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if( query("shen", me)<3500 )
                return "�������������ò������������Ȳ�æ���㡣";

        if (me->query_skill("force") < 100)
                return "����ڹ�����㣬ѧ�������С�";

        if (me->query_skill("liuhe-dao", 1) < 80)
                return "������ϵ������δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "��$N" HIY "Ц��Ц���γ������嵶"
                       "�������У�˵�����������ϵ�����\nҪ��������չ"
                       "��Ĩ�������硢���������ƣ������ˡ���˵���ֻ"
                       "��\n$n" HIY "���иֵ����²�������������������"
                       "�ͻ��������任�����ַ�\nλ��������һ������"
                       "�������������ֱ�졣\n" NOR, me, this_object());

        command("nod");
        command("say ֻҪ�������������ϵķ�λ�����б㲻����ϰ��");
        tell_object(me, HIC "��ѧ���ˡ�����������\n" NOR);

        if (me->can_improve_skill("blade"))
                me->improve_skill("blade", 1500000);
        if (me->can_improve_skill("liuhe-dao"))
                me->improve_skill("liuhe-dao", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/liuhe-dao/shan", 1, me);
        addn("family/gongji", -200, me);
        return 1;
}
*/
mixed ask_skill9()
{
        object me;

        me = this_player();

        if( query("can_perform/hujia-daofa/xian", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if (me->query_skill("hujia-daofa", 1) < 1)
                return "�������ҵ�����δ��ѧ�������������У�";

        if( query("family/gongji", me)<900 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if (me->query_skill("force") < 160)
                return "����ڹ�����㣬ѧ�������С�";

        if (me->query_skill("hujia-daofa", 1) < 120)
                return "��ĺ��ҵ������δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "�γ������嵶���������С���$N" HIY "Ц"
                       "�������ұ������й��㣬��\n����εֵ�����˵��$n" HIY
                       "�Ѻ��������������������$N" HIY "��˫�ַ���\n������"
                       "���ѣ�������$N" HIY "�ƽ���$N" HIY "��״������Ȼʧ"
                       "Ц���ĵ�������Ҳ\n�Ƶ����ǵ�������˻�����ô�ƵС�"
                       "������Ҳ�����ԣ�����ǰ����\nһ�У�����ʱֻ��$n" HIY
                       "����һ�䣬�й�ֱ����$N" HIY "��������Ȼȫ����\n����"
                       "$N" HIY "��æ�����ұܣ���$n" HIY "�ĵ���ʼ�ղ���$N"
                       HIY "��ǰ��硣\n" NOR, me, this_object());

        command("haha");
        command("say �������������ƿ�ĵ��ƣ������˱���Ҫ�㡣");
        command("say �����Ҵ����˷ﴦ֪�����������������治ǳ��");
        tell_object(me, HIC "��ѧ���ˡ�������ӡ�ơ���\n" NOR);

        if (me->can_improve_skill("blade"))
                me->improve_skill("blade", 1500000);
        if (me->can_improve_skill("hujia-daofa"))
                me->improve_skill("hujia-daofa", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/hujia-daofa/xian", 1, me);
        addn("family/gongji", -900, me);
        return 1;
}

mixed ask_skill10()
{
        object me;

        me = this_player();

        if( query("can_perform/hujia-daofa/zhui", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if (me->query_skill("hujia-daofa", 1) < 1)
                return "�������ҵ�����δ��ѧ�������������У�";

        if( query("family/gongji", me)<600 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if( query("shen", me)<22000 )
                return "�������������ò������������Ȳ�æ���㡣";

        if (me->query_skill("force") < 200)
                return "����ڹ�����㣬ѧ�������С�";

        if (me->query_skill("hujia-daofa", 1) < 140)
                return "��ĺ��ҵ������δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "΢΢��ף������𻰣������������"
                       "��������������ʹ������\n�����е�һʽ������ж��"
                       "�����Ʒ�бб����һ����Ȧ������������\n��������"
                       "����;ȴ��Ȼ���һ�С�����׶�������Ƽ�����ת��"
                       "����\n�����д�����$N" HIY "�������˫Ŀ��Ȼ"
                       "һ�����ƺ�������ʲô��\n" NOR,
                       me, this_object());

        command("nod2");
        command("say ���еľ�Ҫ������ж�����˵ķ������ٸ�������һ����");
        command("say ���š�����̫����������ǧ��������Ϊ��������");
        tell_object(me, HIC "��ѧ���ˡ�ж��׶���ơ���\n" NOR);

        if (me->can_improve_skill("blade"))
                me->improve_skill("blade", 1500000);
        if (me->can_improve_skill("hujia-daofa"))
                me->improve_skill("hujia-daofa", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/hujia-daofa/zhui", 1, me);
        addn("family/gongji", -1300, me);
        return 1;
}

mixed ask_skill11()
{
        object me;

        me = this_player();

        if( query("can_perform/hujia-daofa/cang", me) )
                return "�������Ѿ��̹������𣿻���˵ʲô��";

        if( query("family/family_name", me) != query("family/family_name") )
                return "���ֲ��ǰ����ҵģ�����������ʲô��";

        if( query("family/gongji", me)<2200 )
                return "�����Һ���µµ��Ϊ���������Ȳ�æ���㡣";

        if( query("shen", me)<30000 )
                return "�������������ò������������Ȳ�æ���㡣";

        if (me->query_skill("force") < 250)
                return "����ڹ�����㣬ѧ�������С�";

        if (me->query_skill("hujia-daofa", 1) < 180)
                return "��ĺ��ҵ������δ�������ö���ϰ��ϰ��";

        message_vision(HIY "$n" HIY "������$N" HIY "���죬�����ص���"
                        "��ͷ����Ϊ�����������벻������\n��ĵ�������"
                        "������裬����ʱ�գ���ص�����Ϊʦ�������Ҿ�"
                        "��\n�����װ˷��ص��ƣ�ϣ���պ����ܹ�������Ϊ"
                        "����$n" HIY "���˶٣���\n���Ŷ�$N" HIY "˵��"
                        "�����Һ��ҵ���֮���������������������������"
                        "֪\n�����ۿͣ������Կͷ�������ʤ���ϣ���ʤ��"
                        "�������ҵ������õ�\n�Ĳ��������ʡ������顢��"
                        "�ȵ��ƾ�Զǿ��һ�㵶����չ��Ĩ������\n�硢��"
                        "�����ȵ��ơ�����˷��ص��Ƶı仯ȴ�ֲ�ֹ�ڴ�"
                        "��������\n������˵�������������㣬��������"
                        "�������á���\n" NOR, me, this_object());

        command("nod");
        command("say �ղ�����˵��ȫ��Ҫ�㣬���μ����ġ�");
        command("say ��������һ��Ҫ�ڼ���ϰ�����ܽ������õô��졣");
        tell_object(me, HIC "��ѧ���ˡ��˷��ص��ơ���\n" NOR);

        if (me->can_improve_skill("force"))
                me->improve_skill("force", 1500000);
        if (me->can_improve_skill("dodge"))
                me->improve_skill("dodge", 1500000);
        if (me->can_improve_skill("parry"))
                me->improve_skill("parry", 1500000);
        if (me->can_improve_skill("blade"))
                me->improve_skill("blade", 1500000);
        if (me->can_improve_skill("hujia-daofa"))
                me->improve_skill("hujia-daofa", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        set("can_perform/hujia-daofa/cang", 1, me);
        addn("family/gongji", -2200, me);
        return 1;
}

mixed ask_baodao()
{
        object me;
        object ob;
        object owner;

        me = this_player();

        if( query("family/family_name", me) != "�������" )
                return "�����������ģ�������������ʲô��";

        if( query("family/master_id", me) != query("id") )
                return "���ֲ����ҵĵ��ӣ���������Щʲô��";

        if( query("shen", me)<10000 )
                return "�㲻�����������ˣ��ⱦ���Ҳ��ܸ��㡣";

        if (me->query_skill("hujia-daofa", 1) < 150)
                return "�㻹������������Ѻ��ҵ���������˵�ɡ�";

        ob = find_object(LENGYUE);
        if (! ob) ob = load_object(LENGYUE);
        owner = environment(ob);
        while (owner)
        {
                if (owner->is_character() || ! environment(owner))
                        break;
                owner = environment(owner);
        }

        if (owner == me)
                return "���̵ģ���������ˣ����ı������ǽ���������";

        if (objectp(owner) && owner != this_object())
        {
                if (! owner->is_character())
                        return "���š����±������ڲ��������";

                if( query("family/family_name", owner) == "�������" )
                        return "���±���������ʱ�����ֵ�"+query("name", owner)+
                               "���ã���Ҫ�þ�ȥ�����ɡ�";
                else
                        return "�������±�������������"+query("name", owner)+
                               "���У���ȥ�����һ����ɣ�";
        }
        ob->move(this_object());

        ob = new("/clone/weapon/gangdao");
        ob->move(this_object());
        ob->wield();

        message_vision(CYN "$N" CYN "��Ц�����������ã���������������ȥ�ðɡ�\n" NOR,
                       this_object(), me);
        command("give moon blade to "+query("id", me));
        return 1;
}

mixed ask_jundao()
{
        object me;
        object ob;
        object owner;

        me = this_player();

        if( query("family/family_name", me) != "�������" )
                return "�����������ģ�������������ʲô��";

        if( query("family/master_id", me) != query("id") )
                return "���ֲ����ҵĵ��ӣ���������Щʲô��";

        if( query("shen", me)<10000 )
                return "�㲻�����������ˣ�������Ҳ��ܸ��㡣";

        if (me->query_skill("blade", 1) < 120)
                return "������������ʹ����ô���ô���������";

        ob = find_object(JUNDAO);
        if (! ob) ob = load_object(JUNDAO);
        owner = environment(ob);
        while (owner)
        {
                if (owner->is_character() || ! environment(owner))
                        break;
                owner = environment(owner);
        }

        if (owner == me)
                return "�����Ǳ���������������������������";

        if (objectp(owner) && owner != this_object())
        {
                if (! owner->is_character())
                        return "���š������������ڲ��������";

                if( query("family/family_name", owner) == "�������" )
                        return "��������������ʱ�����ֵ�"+query("name", owner)+
                               "���ã���Ҫ�þ�ȥ�����ɡ�";
                else
                        return "���Ĵ�����������������"+query("name", owner)+
                               "���У���ȥ�����һ����ɣ�";
        }
        ob->move(this_object());

        ob = new("/clone/weapon/gangdao");
        ob->move(this_object());
        ob->wield();

        message_vision(CYN "$N" CYN "���˵�ͷ������������С���г�Ϣ��������"
                       "�������ȥ�ðɡ�\n" NOR, this_object(), me);
        command("give chuangwang baodao to "+query("id", me));
        return 1;
}

mixed ask_quanjing()
{
        object me;
        object ob;
        object owner;

        me = this_player();

        if( query("family/family_name", me) != "�������" )
                return "�����������ģ�������������ʲô��";

        if( query("family/master_id", me) != query("id") )
                return "���ֲ����ҵĵ��ӣ���������Щʲô��";

        if( query("shen", me)<10000 )
                return "�㲻�����������ˣ���ȭ���Ҳ��ܸ��㡣";

        if( query("combat_exp", me)<10000 )
                return "�����ھ���̫ǳ������æ��ȥ�Ķ�ȭ����";

        if (me->query_skill("hujia-quan", 1) > 119)
                return "�����ȭ���������Ѿ���ǳ�ˣ��Ǳ�ȭ�������������Ƕ��ɡ�";

        ob = find_object(QUANJING);
        if (! ob) ob = load_object(QUANJING);
        owner = environment(ob);
        while (owner)
        {
                if (owner->is_character() || ! environment(owner))
                        break;
                owner = environment(owner);
        }

        if (owner == me)
                return "�Һ��ҵļҴ�ȭ���������������ڿ���";

        if (objectp(owner) && owner != this_object())
        {
                if (! owner->is_character())
                        return "���š�����ȭ�����ڲ��������";

                if( query("family/family_name", owner) == "�������" )
                        return "����ȭ��������ʱ�����ֵ�"+query("name", owner)+
                               "ȡȥ���ˣ���Ҫ����ȥ�����ɡ�";
                else
                        return "���Ǻ��ҵļҴ�ȭ������������"+query("name", owner)+
                               "���У���ȥ����������ɣ�";
        }
        ob->move(this_object());
        message_vision(CYN "$N" CYN "��ͷ�������š��Ȿȭ�������ȥ�ɣ���Ҫ�����ж���\n" NOR,
                       this_object(), me);
        command("give hujia quanjing to "+query("id", me));
        return 1;
}

mixed ask_daopu()
{
        object me;
        object ob;
        object owner;

        me = this_player();

        if( query("family/family_name", me) != "�������" )
                return "�����������ģ�������������ʲô��";

        if( query("family/master_id", me) != query("id") )
                return "���ֲ����ҵĵ��ӣ���������Щʲô��";

        if( query("shen", me)<15000 )
                return "�㲻�����������ˣ��⵶���Ҳ��ܸ��㡣";

        if( query("combat_exp", me)<50000 )
                return "�����ھ���̫ǳ���Ȿ���׶�����˵��������ˡ�";

        if (me->query_skill("blade", 1) > 179)
                return "�㵶���������ѳƵ����ǵǷ��켫�ˣ����׻����������Ƕ��ɡ�";

        ob = find_object(DAOPU);
        if (! ob) ob = load_object(DAOPU);
        owner = environment(ob);
        while (owner)
        {
                if (owner->is_character() || ! environment(owner))
                        break;
                owner = environment(owner);
        }

        if (owner == me)
                return "�Һ��ҵļҵ��׾��������������ڿ���";

        if (objectp(owner) && owner != this_object())
        {
                if (! owner->is_character())
                        return "���š����ҵ������ڲ��������";

                if( query("family/family_name", owner) == "�������" )
                        return "���ҵ���������ʱ�����ֵ�"+query("name", owner)+
                               "ȡȥ���ˣ���Ҫ����ȥ�����ɡ�";
                else
                        return "���Ǻ��ҵļҴ���������������"+query("name", owner)+
                               "���У���ȥ����������ɣ�";
        }
        ob->move(this_object());
        message_vision(CYN "$N" CYN "��ͷ�����Ȿ���ҵ��������ȥ���ɣ��в���"
                       "֮�����������ҡ�\n" NOR, this_object(), me);
        command("give hujia daopu to "+query("id", me));
        return 1;
}

int accept_object(object me, object ob)
{
        int i;
        int r;
        if (r = ::accept_object(me, ob))
                return r;
        if( !me || environment(me) != environment() ) return 0;
        if ( !objectp(ob) ) return 0;
        if ( !present(ob, me) ) return notify_fail("��û�����������");
        if( query("id", ob) == "yanji" )
        {
                write(HIC "\n���˵������λ" + RANK_D->query_respect(me)+"����˴����£�����ʵ��֪����Ϊ����\n");
                write(HIC "\n��쳴�ϲ�����������벻����Ҳ�н��죡�������䣬���ֻ���Ϊ���Ρ�\n");
                ob->die();
                set_temp("marks/hujiadaopu", 1, me);
        }
        return 1;
}

string ask_book()
{
        object ob, me;

        me = this_player();
        if( !query_temp("marks/hujiadaopu", me) )
                return "��˵ʲô? \n";
        if( (query("shen", me)<10000) )
                return "����������С֮�֣��������ֻ��ڶ���\n";

        else
        if( (query("shen", me)>100000) )
        {
                if (query("book_count") > 0)
                {
                        write(HIC "���˵��ת�����ֻ�ʬ�����ѳ�һ���ƾ��鱾�����˸��㡣\n");
                        call_out("waiting",0,me,ob);
                        addn("book_count", -1);
                        ob = new ("/clone/book/blade_book");
                        ob->move(this_player());
                        command("rumor "+query("name", me)+"�õ����ҵ�������\n");
                        return "�Ȿ���ױ��⹷����ռ��ʱ�ˣ����ھ�������ɡ�\n";
                }
                else return "�������ˣ����ڵ����Ѳ��ڴ˴���\n";
        }

}

int accept_ask(object me, string topic)
{
        switch (topic)
        {
        case "����" :
        case "����" :
        case "����" :
                return QUEST_D->accept_ask(this_object(), me, topic);
                break;

        case "�������" :
                return MASTER_D->teach_pfm(me, this_object(),
                        ([ "perform" : "can_perform/tianchang-zhang/huang",
                           "name"    : "�������",
                           "sk1"     : "tianchang-zhang",
                           "lv1"     : 100,
                           "sk2"     : "force",
                           "lv2"     : 140,
                           "neili"   : 2000,
                           "gongxian": 1000,
                           "shen"    : 26000, ]));
                break;

        case "���ֶ����" :
                return MASTER_D->teach_pfm(me, this_object(),
                        ([ "perform" : "can_perform/tianchang-zhang/duo",
                           "name"    : "���ֶ����",
                           "sk1"     : "tianchang-zhang",
                           "lv1"     : 100,
                           "sk2"     : "force",
                           "lv2"     : 140,
                           "neili"   : 2000,
                           "gongxian": 1000,
                           "shen"    : 26000, ]));
                break;

        case "���־�" :
                return MASTER_D->teach_pfm(me, this_object(),
                        ([ "perform" : "can_perform/hujia-daofa/po",
                           "name"    : "���־�",
                           "sk1"     : "hujia-daofa",
                           "lv1"     : 120,
                           "sk2"     : "force",
                           "lv2"     : 120,
                           "gongxian": 600,
                           "neili"   : 1600,
                           "shen"    : 28000, ]));
                break;

         case "����ٵ�" :
                 return MASTER_D->teach_pfm(me, this_object(),
                         ([ "perform" : "can_perform/hujia-daofa/pi",
                            "name"    : "����ٵ�",
                            "sk1"     : "hujia-daofa",
                            "lv1"     : 120,
                            "sk2"     : "force",
                            "lv2"     : 120,
                            "gongxian": 600,
                            "neili"   : 1600,
                            "shen"    : 28000, ]));
                 break;
        case "������" :
                return MASTER_D->teach_pfm(me, this_object(),
                        ([ "perform" : "can_perform/mantian-xing/xing",
                           "name"    : "������",
                           "sk1"     : "mantian-xing",
                           "lv1"     : 60,
                           "force"   : 150,
                           "gongxian": 600,
                           "shen"    : 30000, ]));
                break;

       case "����������" :
               return MASTER_D->teach_pfm(me, this_object(),
                       ([ "perform" : "can_perform/daojian-guizhen/jiu",
                          "name"    : "����������",
                          "sk1"     : "daojian-guizhen",
                          "lv1"     : 1000,
                          "reborn"  : 1,
                          "force"   : 300,
                          "gongxian": 3000,
                          "shen"    : 30000, ]));
               break;

        default:
                return 0;
        }
}

/*
void unconcious()
{
        die();
}

*/