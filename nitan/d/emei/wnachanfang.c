inherit ROOM;

void create()
{
      set("short", "禅房");
        set("long", @LONG
这里是一间安静宽敞的禅房，乃是庵中弟子打坐修行之所。地下
整整齐齐地放着许多蒲团。几位年轻师太和俗家女弟子正肃容入定。
LONG );
      set("exits", ([
          "west" : __DIR__"wnadian",
      ]));
      set("sleep_room", 1);
      set("no_clean_up", 0);
      set("no_fight", 1);
        set("coor/x", -460);
        set("coor/y", -220);
        set("coor/z", 90);
        setup();
      replace_program(ROOM);
}