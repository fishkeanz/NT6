// Last Modified by winder on Nov. 17 2000
// wumu-yishu.c

#include <ansi.h>

inherit ITEM;
void init()
{
        add_action("do_du", "du");
}

void create()
{
        set_name( "���������项", ({ "wumu yishu", "yishu" }));
        set_weight(200);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long",
                        GRN
                        "\n���������������ǰ���µġ��ƽ�Ҫ������\n"
                        "ֻ����һҳ��д��ʮ�˸����֣�\n"
                        "������ѡ����ѵϰ��\n"
                        "�����ͷ��������\n"
                        "���ϼ��ɣ�ͬ�ʿࡱ\n"NOR
                );
                set("value", 10000000);
                set("material", "paper");
                set("no_get", 1);
                set("no_drop", 1);
                set("treasure", 1);
        }
}

int do_du(string arg)
{
        object me = this_player();
        object where = environment(me);
        object ob;

        if (!id(arg)) return 0;
        if (!present(this_object(), this_player()))
                return 0;

//if( me->query("wumu+int")) 
        if( query("mark/+int", this_player()) )
        {
                write("�������һ����������ѽ������\n");
        }
        else
        {
                addn("int", 1, me);
                write(HIY"�����ַ��ģ�����һ��һ��֮�䣬�޲���������Եе��������������̾��\n"NOR);
                write(HIR"�����������ˣ�\n"NOR);
                set("mark/+int", 1, this_player());
        }
        return 1;
}