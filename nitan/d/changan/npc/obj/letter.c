//letter.c

inherit ITEM;

void create()
{
        set_name("����", ({"xin", "letter"}));
        set_weight(100);
          if (clonep())
                  set_default_object(__FILE__);
          else 
        {
                  set("unit", "��");
                  set("long", "һ�������۵��ţ�\n");
        }

          set("is_monitored",1);
          setup();
}

void destruct_me(object where, object me)
{
          message_vision("$nһ�䣬���������Ʈȥ����\n",where,me);
          destruct (me);
}

void init()
{
          object me = this_object();
          object where = environment();
          if (interactive(where))
        {
                    if( query("owned", me) )
                {
                              if( query("owned", me) != query("id", where) )
                        {
                                call_out("destruct_me",1,where,me);
                        }
                } else
                {
                              set("owned",query("id",  where), me);
                }
        }
}