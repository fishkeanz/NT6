// pingyan2.c ��Ұ
// Modify By River@SJ 99.06
#include <ansi.h>
inherit ROOM;
void create()
{
	set("short", GRN"��Ұ"NOR);
        set("long", @LONG
�����ǵ���Ķ�������Ŀ����һƬ��������ͷ�Ĵ���Ұ���ǻ��
ɽ����ǧ����Ļ�ɽ�Ҷѻ����ɣ�ƽҰ��һȺ÷��¹���ڵ�ͷ�Բݣ�
��ΧһƬƽ�������޿ɲ�֮����Զ����ɽ��������ѩ��Щ��ȴ�Ǽ�Ŀ
���̣����ɴ�أ�������滨��������Ϊ�������ޡ�
LONG );
	set("exits",([
		"south" : __DIR__"yanshi",
		"north" : __DIR__"shishan",
                "west" : __DIR__"pingyan3",
                "east" : __DIR__"pingyan4",
	]));
	set("objects",([
		"/d/mingjiao/npc/lu" :  1,
	]));
	set("outdoors","����");
	setup();
}

void init()
{
      string a,b,c;
      a =query("exits/west");
      b =query("exits/north");
      c =query("exits/east");
       set("exits/north", a);
       set("exits/west", c);
       set("exits/east", b);
}