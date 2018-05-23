// jiuhua.c

#include <ansi.h>
#include "medicine.h"

void create()
{
        set_name(HIG "���Ķ���ɢ" NOR, ({"qingxin san", "san"}));
        set_weight(50);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIG "����һ������ɫ�ķ�ĩ�����зǳ���������Ч����\n" NOR);
                set("base_unit", "��");
                set("base_value", 3000);
                set("base_weight", 60);
                set("only_do_effect", 1);
                set("mixture", ([
                        "herb" : ([ "chuanwu"  : 1,
                                    "xuejie"   : 1,
                                    "yjhua"    : 1,
                                    "xiongdan" : 1,
                                    "shancha"  : 1,
                                    "gsliu"    : 1,
                                    "zzfen"    : 1, ]),
                        "neili" : 100,
                        "jing"  : 50,
                        "time"  : 3,
                        "min_level" : 80,
                        "medical" : 100,
                ]));
        }
        setup();
}

int do_effect(object me)
{
        mapping my;

        if( time()-query_temp("last_eat/qingxin", me)<20 )
        {
                write("��շ��ù�ҩ����ҩ�Է�����Ч���Ժ���ܼ������á�\n");
                return 1;
        }

        my = me->query_entire_dbase();
        if (my["eff_jing"] == my["max_jing"])
        {
                write("�����ھ���״̬�ܺã��������" + name() + "��\n");
                return 1;
        }

        set_temp("last_eat/qingxin", time(), me);

        message_vision(HIG "$N" HIG "һ���죬������" + name() +
                       HIG "������ȥ����ö��ˡ�\n" NOR, me);

        me->receive_curing("jing", 150);
        me->receive_heal("jing", 150);

        me->start_busy(1);

        add_amount(-1);
        if (query_amount() < 1)
                destruct(this_object());

        return 1;
}