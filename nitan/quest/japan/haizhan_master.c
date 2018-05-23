// /job/japan/haizhan_master.c
// by akuma 10:57 AM 11/30/2003
// source by naihe  2003-06-19  ��ï��

#include <ansi.h>
inherit ITEM;

#define B1                "��Ұ�Ľ���"                // ��  - YEL, HIY
#define B2                "�н�������"                // ����  - MAG, HIM
#define B3                "�ֻ�����"                // �ʹ�  - YEL, HIY
#define B4                "�о��ٵ�ͷ��"                // ����  - CYN, HIC

string game_now = "�ȴ�ָʾ�����";
string att_now = "not_ok";
int target_hp, target_damage;
string pic_me, pic_target;

void new_game_set( int amount, string mark );
void map();
void check_hp();
object get_me();
string get_msg( int x, int y, string type );
void xunbaotu();
void fight_start( string who );
void be_attack();
void target_check_hp();

int finded_baowu( int which, string name )
{
        int score;
        object me = get_me();

        if ( !me ) return 0;

        addn("be_finded", 1);
        set("b" + which, HIW + query("b" + which) );
        map();
        write(HIC"̫�����ˣ������ҵ���" + name + "��\n"NOR);

        if ( query("be_finded") == 4 ) {
                message_vision("$N�ɹ����ҵ�ȫ���ļ�ս��Ʒ������������ƽ�Ľ�����\n", me);
                addn("balance", 2000000, me);
        }
        return 1;
}

object get_me()                // ��á�me��
{
        object me;

        if ( !query("owner_id") ) return 0;

        me = find_player( query("owner_id") );
        if ( !me ) return 0;
        return me;
}

int damage( int type )  // ��ù������� 6-11
{
        if ( !type || type < 1 || type > 5 ) type = 1;

        switch( type ) {
                case 1: return ( 3 + random(1) );
                case 2: return ( 5 + random(2) );
                case 3: return ( 8 + random(3) );
                case 4: return ( 12 + random(4) );
                case 5: return ( 15 + random(5) );
        }
        return (6 + random(6));
}

void del_me()
{
        object me = get_me();

        if ( !me ) return;

        me->move("/d/tianjin/navy03");
        delete_temp("hanzai_cond", me);
        set("job_done", "fleet_job", me);
        addn("job/fleet", 1, me);
        destruct( this_object() );
}

void create()
{
        set_name(HIY"��γ��"NOR, ({"jingwei yi","yi"}));
        set_weight(1);
        if ( clonep() )
                set_default_object(__FILE__);
        else {
                set("long", @LONG
����ս������Ҫ���ߣ��������ſ��Խ��м��纣սս����

��ѯս��״̬�����룺 combat check
�˳�ս��״̬�����룺 combat over
LONG);
                set("unit", "��");
                set("value", 0);
        }
        set("no_get", 1);
        set("no_drop",1);
        set("no_steal",1);
        set("no_give",1);
        setup();
}

void init()
{
        add_action("do_game", "combat");
        add_action("do_go", "ship");
        add_action("do_liaowang", "liaowang");
        add_action("do_attack", "fire");
}


void new_game(object me)
{
        if ( !me ) return del_me();

        set("owner_id", getuid(me));
        set("owner_name",query("name", me));

        new_game_set( random(3) + 3, "����" );
        new_game_set( 2, "��Զ��" );
        new_game_set( 2, "����̽����" );
        new_game_set( 2, "Ѱ��ͼ" );
        new_game_set( 1, "������" );
        new_game_set( 5, "��½ͧ" );
        new_game_set( 12, "��ͧ" );
        new_game_set( 8, "����" );
        new_game_set( 8, "Ѳ��" );
        new_game_set( 5, "Ǳͧ" );
        new_game_set( 2, "��Ұ��" );

        new_game_set( 1, B1);
        new_game_set( 1, B2);
        new_game_set( 1, B3);
        new_game_set( 1, B4);

        new_game_set( 1, "��ǰ�콢λ��");

        set("att_�ⲫ", 999999);
        set("att_����", 40);
        set("att_ʯ��", 10);
        set("att_����", 5);
        set("att_ȼ�յ�", 3);
        set("att_����", 2);
        set("hp", 150);

        set_temp("haizai_cond", "waiting_go", me);
        write("\n============ ���óɹ������ڽ�����Ϸ���� ============\n");

        set("b1", YEL"��Ұ�Ľ���"NOR);
        set("b2", YEL"�н�������"NOR);
        set("b3", YEL"�ֻ�����"NOR);
        set("b4", CYN"�о��ٵ�ͷ��"NOR);

        call_out("game_start", 1);
}

void new_game_set( int amount, string mark )
{
        int x, y;        // ����� x, y �� x�������ڼ��ᣬy�Ǻ����ڼ�����

        while ( amount-- ) {
                x = 1 + random(100);
                y = 1 + random(160);

                if ( query( "aaa(" + x + "," + y + ")" ) ) {
                        amount ++;
                        continue;
                }
                set( "aaa(" + x + "," + y + ")" , mark );
                if ( mark == "��ǰ�콢λ��" ) {
                        set("now_x", x);
                        set("now_y", y);
                }
                if ( mark == "����" )
                        set("island(" + x + "," + y + ")", "new" );
        }
}

void game_start()
{
        object me = get_me();

        if ( !me ) return del_me();

        set_temp("haizhan_cond", "ok", me);
        message_vision("$N������뺣սս�������ұ���ˮʦ�໥ЭӦ\n", me);

        write("\nս��Ŀ�꣺�����ձ�ˮʦ�콢��\n");
        map();
}
////////////////////////////////////
/////////  ��Ҳ�������  ///////////
////////////////////////////////////
int do_game(string arg)
{
        int exp;
        object me = this_player(), who;

        who = get_me();

        if ( !who || me != who ) return 0;

        if ( !arg || ( arg!= "check" && arg != "ck" && arg != "over") ) return 0;

        if( !query_temp("haizhan_cond", me))return 0;

        if ( arg == "check" || arg == "ck" ) {
                map();
                return 1;
        }
        if ( arg == "over" ) {
                delete_temp("haizhan_cond", me);
                message_vision("$N��;�˳���ս��\n", me);
                exp = 1000 + random(1000);
                write(HIR"\n����������ӣ���ʧ����ֵ" + exp + "�㡣\n"NOR);
                addn("combat_exp", -exp, me);
                remove_call_out("del_me");
                call_out("del_me", 1);
                return 1;
        }
        return 1;
}

int do_go(string arg)
{
        int x, y, old_x, old_y, i;
        string *dirs = ({ "north", "south", "west", "east", "northwest", "northeast", "southwest", "southeast", });
        string here, msg;
        object me = this_player(), who, obj;

        who = get_me();

        if ( !who || who != me ) return 0;
        if ( member_array( arg , dirs ) == -1 ) return 0;

        if( query_temp("haizhan_cond", me) != "ok")return notify_fail("���ڲ����ƶ������ڣ�"+game_now+"��\n");

        x = query("now_x");
        y = query("now_y");

        old_x = x;
        old_y = y;

        switch ( arg ) {
                case "north": x--; break;
                case "south": x++; break;
                case "west" : y--; break;
                case "east" : y++; break;
                case "northwest" : x--; y--; break;
                case "northeast" : x--; y++; break;
                case "southwest" : x++; y--; break;
                case "southeast" : x++; y++; break;
        }
        if ( x < 1 || y < 1 || x > 12 || y > 18 ) return notify_fail("��������Ѿ��޷�ǰ���ˣ�\n");

        set("be_finded(" + old_x + "," + old_y + ")", 1); 

        if ( query( "island(" + old_x + "," + old_y + ")" ) == "new" || query( "island(" + old_x + "," + old_y + ")" ) == "old" ) {
                set("island(" + old_x + "," + old_y + ")", "old");
                set("aaa("+old_x+","+old_y+")", "�ɵĵ���");
        }
        if ( !query( "island(" + old_x + "," + old_y + ")" ) ) delete("aaa(" + old_x + "," + old_y + ")" );

        here = query( "aaa(" + x + "," + y + ")" );

        set("aaa(" +x+ "," +y+ ")", "��ǰ�콢λ��");
        set("now_x", x);
        set("now_y", y);

        // �����жϣ�����
        if ( here == "����" ) {
                addn("att_����", 10);
                addn("att_ʯ��", 4);
                addn("att_����", 2);
                addn("att_ȼ�յ�", 1);
                addn("att_����", 1);
                addn("hp", 20 + random(11));
                map();
                write(HIG"���ֵ��죡�õ��˵�ҩ�����ʹ�ֻ������\n"NOR);
                if ( random(100) > 80 ) {
                        write(HIG"ͻȻ��һ���ձ���ʿ�ӵ������δ������������������\n"NOR);
                        obj = new(__DIR__"killer3");
                        set("target",query("id",  me), obj);
                        obj->move(environment(me));
                }
                return 1;
        }
        if ( here == "��½ͧ" ) {
                write(HIG"ͻȻ��һ���ձ���ʿ�ӵ�½ͧ�����������������\n"NOR);
                obj = new(__DIR__"killer3");
                map();
                set("target",query("id",  me), obj);
                obj->move(environment(me));
                return 1;
        }
        if ( here == "������" ) {
                addn("hp", 30 + random(21));
                map();
                write(HIC"�����޲����ϣ���ֻ�õ���������\n"NOR);
                return 1;
        }
        if ( here == B1 ) return finded_baowu( 1, B1 );

        if ( here == B2 ) return finded_baowu( 2, B2 );

        if ( here == B3 ) return finded_baowu( 3, B3 );

        if ( here == B4 ) return finded_baowu( 4, B4 );

        if ( here == "��Զ��" ) {
                game_now = "������ ��ָ� liaowang x y��";
                map();
                write(HIM"��������Զ�������� (liaowang x y) ָ��鿴�õ��ܱߺ�������Σ� \n"NOR);
                set_temp("haizhan_cond", "wangyuanjing", me);
                return 1;
        }
        if ( here == "����̽����" ) {
                map();
                write(HIM"�����˱���̽���ǣ�\n"NOR);

                msg = HIM"\n������̽���С����Զ�̽�� " + x + "," + y + " ����������\n\n";
                msg += get_msg( x - 1, y - 1, "bjy" );
                msg += get_msg( x - 1, y, "bjy" );
                msg += get_msg( x - 1, y + 1, "bjy" );
                msg += get_msg( x, y - 1, "bjy" );
                msg +=  get_msg( x, y,   "bjy" );
                msg +=  get_msg( x, y + 1, "bjy" );
                msg +=  get_msg( x + 1, y - 1, "bjy" );
                msg +=  get_msg( x + 1, y, "bjy" );
                msg +=  get_msg( x + 1, y + 1, "bjy" );
                msg += "\n̽��Σ�����򱨾���ϡ�\n\n"NOR;
                write( msg );
                return 1;
        }
        if ( here == "Ѱ��ͼ" ) {
                map();
                write(HIC"������һ��Ѱ��ͼ��\n"NOR);
                xunbaotu();
                return 1;
        }
        if ( here == "��ͧ" || here == "����" || here == "Ѳ��" || here == "Ǳͧ" || here == "��Ұ��" ) {
                game_now = "�� " + here + " ս���У� �ȴ����˹���";
                set_temp("haizhan_cond", "fighting", me);
                att_now = "not_ok";
                map();
                write(HIR"��⡭�������� " + here + "���������ս��״̬��\n"NOR);
                set("who_name", here);
                remove_call_out("fight_start");
                call_out("fight_start", 1, here);
                return 1;
        }
        map();
        write(HIW"�����ں�����ʹ����δ���ֵ��顭��\n"NOR);
        return 1;
}

void map()
{
        int x, y, temp = 0;
        string the_map = HIC"  -- "HIR"HP��" + query("hp") + "  ս��Ʒ��" + query("be_finded") + NOR"\n    ", temp_map, temp2;

        for ( x = 1; x < 103; x++ ) {
                for( y = 1; y < 40; y++ ) {
                        if ( temp == 39 ) {
                                temp = 0;
                                the_map += "\n    ";
                        }
                        temp2 = "(" + x + "," + y + ")";

                        if ( query( "be_finded" + temp2) ) temp_map = HIC"��"NOR;
                        else if ( query("weixian" + temp2) ) temp_map = HIR"��"NOR;
                        else temp_map = HIC"��"NOR;

                        if ( query( "aaa" + temp2 ) == "����" ) temp_map = HIG"��"NOR;
                        if ( query( "aaa" + temp2 ) == "�ɵĵ���" ) temp_map = HIG"��"NOR;
                        if ( query( "aaa" + temp2 ) == "��ǰ�콢λ��" ) temp_map = BLINK+HIY"��"NOR;

                        the_map += temp_map;
                        temp++;
                }
        }
        the_map += "\n\n";

        the_map += sprintf( "      %s  %s  %s  %s  \n", query("b1"), query("b2"), query("b3"), query("b4") );

        the_map += sprintf(HIW"  �ⲫ����  ������%d  ʯ�ڣ�%d  ���ڣ�%d   ȼ�յ���%d  ���ף�%d\n"NOR,
                query("att_����"), query("att_ʯ��"), query("att_����"), query("att_ȼ�յ�"), query("att_����") );

        write( HIY"================================================\n"NOR + the_map +
                HIY"================================================\n"NOR"��ǰλ�ã�(" + query("now_x") + "," + query("now_y") + ")  ���ڣ� " +
                game_now + " \n");
}

void check_hp()
{
        int x, y, temp = 0;
        string the_map, temp_map, temp2;
        object me = get_me();

        if ( !me ) call_out("del_me", 1);

        if ( query("hp") > 0 ) return;

        the_map = HIR"\n  -- ʧ�ܽ������(��Ѱ��" + query("be_finded") + "��ս��Ʒ)\n\n        "NOR;

        for( x = 1; x < 11; x++ ) {
                for ( y = 1; y < 17; y++ ) {
                        if ( temp == 16 ) {
                                temp = 0;
                                the_map += "\n        ";
                        }
                        temp2 = query("aaa("+x+","+y+")");
                        temp_map = HIC"��"NOR;

                        if ( temp2 == "�ܴ�" ) temp_map = HIR"��"NOR;
                        if ( temp2 == "Ѳ��" ) temp_map = HIR"Ѳ"NOR;
                        if ( temp2 == "����" ) temp_map = HIR"��"NOR;
                        if ( temp2 == "Ǳͧ" ) temp_map = HIR"Ǳ"NOR;
                        if ( temp2 == "��Ұ��" ) temp_map = HIR"��"NOR;



                        if ( temp2 == "����" ) temp_map = HIG"��"NOR;
                        if ( temp2 == "�ɵĵ���" ) temp_map = HIG"��"NOR;
                        if ( temp2 == "��ǰ�콢λ��" ) temp_map = BLINK+HIB"��"NOR;

                        if ( temp2 == B1 || temp2 == B2 || temp2 == B3 || temp2 == B4 )
                                temp_map = HIY"Ʒ"NOR;

                        if ( temp2 == "��Զ��" ) temp_map = HIM"��"NOR;
                        if ( temp2 == "����̽����" ) temp_map = HIM"��"NOR;
                        if ( temp2 == "������" ) temp_map = HIM"��"NOR;
                        if ( temp2 == "Ѱ��ͼ" ) temp_map = HIM"ͼ"NOR;

                        the_map += temp_map;
                        temp++;
                }
        }
        the_map += "\n\n";

        write( the_map );
        delete_temp("haizhan_cond", me);
        game_now = "�Ѿ�������";

        message_vision("$N��������������\n", me);
        if ( random( (int)me->query_skill("swimming", 1) ) < 80 ) {
                set_temp("last_damage_from", "��������������", me);
                me->die();
        }
        else {
                tell_object(me, CYN"������򰶱���ȥ����Ȼ�����Ϳ��������������\n"NOR);
                me->unconcious();
        }
        remove_call_out("game_over_over");
        call_out("game_over_over", 1);
        return;
}

void game_over_over()
{
        write(HIR"ս�������ˡ���\n"NOR);
        remove_call_out("del_me");
        call_out("del_me", 1);
        return;
}

int do_liaowang( string arg )
{
        int x, y;
        string msg;
        object me = this_player(), who;

        who = get_me();

        if ( !who || who != me ) return 0;

        if( query_temp("haizhan_cond", me) != "wangyuanjing")return 0;

        if ( !arg || sscanf( arg, "%d %d", x, y ) != 2 ) return notify_fail("��������ȷ���꣡x -> �������ڼ���  y -> �������ڼ���\n");

        if ( x < 1 || x > 10 || y < 1 || y > 16 ) return notify_fail("��������ȷ���꣡x -> �������ڼ���  y -> �������ڼ���\n");


        msg = HIM"�����С���������鿴 " + x + "," + y + " ����������\n\n";

        msg += get_msg( x-1,y-1, "wyj" );
        msg += get_msg( x-1,y,   "wyj" );
        msg += get_msg( x-1,y+1, "wyj" );
        msg += get_msg( x,  y-1, "wyj" );
        msg +=  get_msg( x,  y,   "wyj" );
        msg +=  get_msg( x,  y+1, "wyj" );
        msg +=  get_msg( x+1,y-1, "wyj" );
        msg +=  get_msg( x+1,y,   "wyj" );
        msg +=  get_msg( x+1,y+1, "wyj" );

        msg += "\n�鿴��ϡ�\n"NOR;

        write( msg );
        set_temp("haizhan_cond", "ok", me);
        game_now = "�ȴ��ƶ�����";
        return 1;
}

string get_msg( int x, int y, string type )
{
        string msg;

        if ( x<1 || x>10 || y<1 || y>16 ) return "";

        msg = query("aaa(" + x + "," + y + ")" );

        if ( !msg ) {
                if( type == "wyj" ) return "(" + x + "," + y + ") -----------\n";
                return "";
        }
        if ( msg == "��ͧ" || msg == "����" || msg == "Ѳ��" || msg == "Ǳͧ" || msg == "��Ұ��" ) {
                set("weixian(" + x + "," + y + ")", 1);
                if( type == "wyj" ) return "(" + x + "," + y + ") " + msg + "\n";
                return "("+x+","+y+") ��Σ��Ǳ����\n";
        }
        if ( type == "wyj" ) return "(" + x + "," + y + ") " + msg + "\n";
        else return "";
}

void xunbaotu()
{
        int x, y;
        string msg;

        for ( x = 1; x < 11; x++ ) {
                for ( y = 1; y < 17; y++ ) {
                        msg = query("aaa(" +x+ "," +y+ ")" );
                        if ( msg == B1 || msg == B2 || msg == B3 || msg == B4 ) {
                                write( HIM"(" + x + "," + y + ")    �ô�����ս��Ʒ\n"NOR);
                                return;
                        }
                }
        }
}

void fight_start( string who )
{
        switch ( who ) {
                case "��ͧ": pic_target = "��"; target_damage = 1; break;
                case "����": pic_target = "��"; target_damage = 2; break;
                case "Ѳ��": pic_target = "��"; target_damage = 4; break;
                case "Ǳͧ": pic_target = "��"; target_damage = 6; break;
                case "��Ұ��": pic_target = "��"; target_damage = 10; break;
        }
        if ( who == "��Ұ��" ) target_hp = 110 + random(31);
        else target_hp = 70 + random(31);
        pic_me = "��";

        remove_call_out("be_attack");
        call_out("be_attack", 1);
}

void be_attack()
{
        int damage;
        string pic1, pic2;

        damage = target_damage + random(3);
        addn("hp", -damage);

        pic1 = HIR + pic_target + " " + query("who_name") + "("+ target_hp + ")"NOR;
        pic2 = sprintf("    "HIG"(%|4d)������ "HIR"��"NOR, query("hp"), pic_me);

        write( sprintf("\n                     _ _ _ _ _ _ _ _ _ _       ATTACK��\n            "HIR"-%-2d"NOR"    /                     `"HIY"��"NOR +
                "\n%s                        %s\n�֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡�\n\n", damage, pic2,  pic1));

        write(HIY"��������ܵ���" + query("who_name") +  "�Ĺ�������ֻ��" + damage + "��\n"NOR);
        check_hp();
        call_out("can_att", 1);
}

void can_att()
{
        if ( query("hp") < 1 ) return;

        write(HIY"�ֵ��㹥���ˡ�(fire ��ʽ)\n"NOR);
        write( sprintf(HIG"����ʣ�ࣺ    �ⲫ������    ������%d    ʯ�ڣ�%d    ���ڣ�%d    ȼ�յ���%d    ���ף�%d\n"NOR,
                query("att_����"), query("att_ʯ��"), query("att_����"), query("att_ȼ�յ�"), query("att_����") ) );
        game_now = "�� " + query("who_name") + " ս���С��ȴ�����ָ���";
        att_now = "ok";
}

int do_attack(string arg)
{
        int damage, miss;
        string target_name = query("who_name"), pic1, pic2, temp;
        object me = this_player(), who;

        who = get_me();
        if ( !who || who != me ) return 0;

        if ( att_now != "ok" ) return notify_fail("���ڲ������㹥����ʱ��\n");

        if ( !arg || ( arg != "�ⲫ" && arg != "����" && arg != "ʯ��" && arg != "����" && arg != "ȼ�յ�" && arg != "����" ) )
                return notify_fail("��ѡ����������ԡ�\n");

        if ( query("att_"+ arg ) < 1 ) return notify_fail("���������Ѿ�û���ˡ�\n");
        addn( "att_"+ arg , -1 );

        temp = arg;
        switch ( arg ) {
                case "�ⲫ":
                        miss = 5;
                        damage = damage(1);
                        break;
                case "����":
                        miss = 10;
                        if ( target_name == "��ͧ" ) damage = damage(5);
                        if ( target_name == "����" ) damage = damage(4);
                        if ( target_name == "Ѳ��" ) damage = damage(3);
                        if ( target_name == "Ǳͧ" ) damage = damage(2);
                        if ( target_name == "��Ұ��" ) damage = damage(1);
                        break;
                case "ʯ��":
                        miss = 15;
                        if ( target_name == "��ͧ" ) damage = damage(4);
                        if ( target_name == "����" ) damage = damage(1);
                        if ( target_name == "Ѳ��" ) damage = damage(5);
                        if ( target_name == "Ǳͧ" ) damage = damage(3);
                        if ( target_name == "��Ұ��" ) damage = damage(2);
                        break;
                case "����":
                        miss = 20;
                        if ( target_name == "��ͧ" ) damage = damage(2);
                        if ( target_name == "����" ) damage = damage(3);
                        if ( target_name == "Ѳ��" ) damage = damage(5);
                        if ( target_name == "Ǳͧ" ) damage = damage(1);
                        if ( target_name == "��Ұ��" ) damage = damage(4);
                        break;
                case "ȼ�յ�":
                        miss = 30;
                        if ( target_name == "��ͧ" ) damage = damage(1);
                        if ( target_name == "����" ) damage = damage(5);
                        if ( target_name == "Ѳ��" ) damage = damage(2);
                        if ( target_name == "Ǳͧ" ) damage = damage(4);
                        if ( target_name == "��Ұ��" ) damage = damage(3);
                        break;
                case "����":
                        miss = 20;
                        if ( target_name == "��ͧ" ) damage = damage(3);
                        if ( target_name == "����" ) damage = damage(2);
                        if ( target_name == "Ѳ��" ) damage = damage(1);
                        if ( target_name == "Ǳͧ" ) damage = damage(4);
                        if ( target_name == "��Ұ��" ) damage = damage(5);
                        break;
        }
        pic1 = HIR"��" + " " + query("who_name") + "("+ target_hp + ")"NOR;
        pic2 = sprintf("    "HIG"(%|4d)������ %s"NOR, query("hp"), pic_me);

        write( sprintf("\n       %|8s��    _ _ _ _ _ _ _ _ _ _\n                   "HIY"��"NOR"                     \\    "HIR"-%d"NOR +
                "\n%s                        %s\n�֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡֡�\n\n", temp, damage, pic2,  pic1));
        if ( random(100) > miss ) {
                target_hp -= damage;
                write(HIY"ʹ��" + temp + "��" + query("who_name") + "���й������𺦶Է�" + damage + "��\n"NOR);
        }
        else {
                write(HIY"Ȼ����û�д���" + query("who_name") + "��\n"NOR);
        }
        att_now = "not_ok";
        remove_call_out("target_check_hp");
        call_out("target_check_hp", 1);
        return 1;
}

void target_check_hp()
{
        int exp, pot;
        string msg;
        mapping reward;
        object me = get_me();

        if ( !me ) {
                remove_call_out("del_me");
                call_out("del_me", 1);
                return;
        }
        if ( target_hp < 1 ) {
                addn("sunk_enemy", 1);
                if ( query("who_name") == "��Ұ��" ) {
                        tell_object(me, "��ɹ��ؽ��ձ������콢�������������\n");
                        reward=TASK_D->get_reward(me,5000,3,1000+random(500),query("job/fleet", me));
                        exp = reward["exp_reward"];
                        pot = reward["pot_reward"];
                        addn("combat_exp", exp, me);
                        addn("potential", pot, me);
                        addn("job/jiawu", 1, me);
                        tell_object(me, "�㱻������" + exp + "�㾭��ֵ��" + pot + "��Ǳ�ܡ�\n");
                        msg="�����ġ�����ˮʦ(beiyangshuishi)��"+query("name", me)+HIC"�ɹ��ػ������ձ������콢������Ұ�š�\n";
                        message("channel:chat", HIC + msg + NOR, users());
                        remove_call_out("del_me");
                        call_out("del_me", 1);
                        return;
                }
                if ( query("sunk_enemy") > 10 ) {
                        tell_object(me, "��ɹ��ػ������ҵн������Է����ˡ�\n");
                        reward=TASK_D->get_reward(me,1500,3,500+random(500),query("job/jiawu", me));
                        exp = reward["exp_reward"];
                        pot = reward["pot_reward"];
                        addn("combat_exp", exp, me);
                        addn("potential", pot, me);
                        addn("job/jiawu", 1, me);
                        tell_object(me, "�㱻������" + exp + "�㾭��ֵ��" + pot + "��Ǳ�ܡ�\n");
                        msg="�����ġ�����ˮʦ(beiyangshuishi)��"+query("name", me)+HIC"�ɹ��ػ������ձ������콢�Ĺ�����\n";
                        message("channel:chat", HIC + msg + NOR, users());
                        remove_call_out("del_me");
                        call_out("del_me", 1);
                        return;
                }
                game_now = "�ȴ��ƶ�����";
                addn("hp", 1 + (random(5)) );
                if( query("hp") > 120 ) set("hp", 120);
                map();
                write(HIG"\n������ " + query("who_name") + " ������������Ĳ��������������ɣ�\n\n"NOR);
                set_temp("haizhan_cond", "ok", me);
                return;
        }
        be_attack();
}