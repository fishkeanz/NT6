inherit ROOM;

void create()
{
        set("short", "���Ƶ�");
        set("long", @LONG
���ƴ��λ��������������棬����������ضΣ���ʯ���̳ɵĽ�
�棬���������࣬һ�����ϣ�һ�ɵƺ���̣���Ϊ���֡�Ц������������
�������������һƬ��������Ķ��䣬�㲻����Գ����������ʹ�ͣ����
��һ���������Ͼ�Ҫ�����ڳ����ˣ�ԶԶ��ȥ�����Կ����سǵĹٱ�����
����Ŷӳ��ǵ�·�ˡ�����Ķ�����һ�仨�꣬�������������������
�µ�����ĵ����������³��ľ���ꡣ
LONG);
        set("outdoors", "luoyang");
        set("no_clean_up", 0);
        set("exits", ([
                  "east" : __DIR__"miaopu",
                  "north" : __DIR__"sroad6",
                  "south" : __DIR__"nandoor",
                  "west" : __DIR__"muqipu",
        ]));

	set("coor/x", -6990);
	set("coor/y", 2110);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}