#include <ansi.h>
inherit ITEM;

void create()
{
        set_name(HIW "��������ƪ��" NOR, ({ "yueqi book", "yueqi", "book" }));
        set_weight(200);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "��");
                set("long", HIW "�����ŹŴ������ıʼǣ�������û�¶�"
                            "���Կ���(read)��\n" NOR);
                set("value", 50);
                set("material", "paper");
        }
        setup();
}

void init()
{
        add_action("do_read", "read");

}

int do_read(string arg)
{
        if (arg == "yueqi book" || arg == "yueqi" || arg == "book")
                return notify_fail(WHT "\n"
"-----------------------------------------------------\n\n"
"                   �й�����������\n\n"
"    ɪ��ɪ���Ŵ���������������ʷ��Զ���ݡ����񡷼��أ�\n"
"�Ŵ��������������������У�����ɪ���೪�衣ս����\n"
"�غ�֮��ʢ�С���ɪ֮�衱��κ���ϱ���ʱ�ڣ�ɪ�ǰ�����\n"
"�͸�ĳ�������������ʱ���������֡��Ժ���ֻ���ڹ�͢��\n"
"�ֺͶ������� \n\n"
"   ��������������������������� �������ȡ�\n"
"����ԭʼ��ᣬ������«έ���Ƴɵ�έ�ߡ�������������ȣ�\n"
"�������ز�ͬ���������ơ���С�б��ƴ���ǰ�������ɳ�\n"
"������Ժ����ڵ������������ս����У���Ʊ����Ϊ\n"
"�����ʾ����Ԫ������������Ľ�Ϊ���µę�����\n"
"����Ϊ˫�����Ƶę��\n\n"
"   �ӣ��Ŵ�������������ڹŴ������������෨�����𲿡�\n"
"��ʢ������ͭʱ�������ڹŴ����������������ǵ�λ��Ȩ��\n"
"���������������������ڳ�Ƹ������ȸ����ǵ䡢��������\n"
"�������У��㷺ʹ�������֡��û��ӵ����Ĳ��Ͳ�Ĳ��ɷ�\n"
"����Ƶ����������������һ��Ϊ��С�������̡�������ͭ��\n"
"���ϼ�ʵ���͸�ʴ����ǧ�����������������˹Ŵ���ʵ����\n"
"�졢 �����˷ḻ�Ĺ����Ų������ǲ��ֳ����ز����ڽ�����\n"
"���������Ρ���ʷ�����֡����ס�ұ�����ڽ̵ȸ����Ļ�\n"
"�ں���һ�����γ����ص��л������Ļ���\n\n"
"   �ࣺ ������Ҫ��ʯ��ʯ���������ʯ����ʯ����������\n"
"���Σ�����΢���Ρ���С�񱡸��졣�����ͭ���ɣ��ʵ���\n"
"˫��ṹ������ΪԲ��״�������͵���������״����ͷ����\n"
"�������������㡣�������أ������������ι̡����������\n"
"�����ң�ÿ���ַ�Ϊ���飬һ��Ϊ���������ġ���ȹ�ϵ��\n"
"�У�һ��Ϊʮ������������Ϊ���������Ķȹ�ϵ�������ǰ�\n"
"��ͬ���ɣ�������ϵġ�\n\n"
"   ��:  �������������ڴ�Լ��ǧ����ǰ���̴������ϵĳ�\n"
"�Ρ��Ŵ����������϶����������İ������෨�������ࡣ\n\n"
"   ��:  ����������ԭʼ����ѳ��֡�������Դ���������\n"
"��������йأ��������Ľ����������ɵ�����������\n\n"
"   �ģ� ����������׹������й��֣���˵����Ĺģ�Ƥ��\n"
"����û���Ϊ����֮�á����ģ����������û��Ĺġ�������\n"
"���ģ�����������֮�������ġ�\n\n"
"  �ǵѣ��ǵѣ��������������Լ��ǧ�������ң�Ϊ��ʯ��\n"
"ʱ����������ǵ����ݹ���������Լ20���ס�ֱ��Լһ��\n"
"�ף����ܡ��ܲ࿪�߸����ף��޴��׺͵�Ĥ�ס�����ʱ�轫\n"
"�ǵ�б�֣����˴������챣��һ������б�ȡ�\n\n" HIG
"                 һ�����������ǧ��\n\n"
"                 ������ɳ����òδ��\n\n" WHT
"-----------------------------------------------------\n"
"\n\n" NOR);
}