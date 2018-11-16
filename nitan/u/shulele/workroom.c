inherit ROOM;
#include <ansi.h>

//熟悉地层练习练习所用

string random_name()
{
string *msgs = ({
        "宾头卢尊者","迦诺迦伐蹉尊者","迦诺跋厘惰尊者","苏频陀尊者", 
        "诺巨罗尊者","跋陀罗尊者","迦理迦尊者","罗弗多罗尊者",
        "戌博迦尊者","半托迦尊者","罗罗尊者","那伽犀那尊者",
        "因揭陀尊者","伐那婆斯尊者","阿氏多尊者","注荼半托迦尊者",
        "迦叶尊者","弥勒尊者",
        "观世音菩萨","跋陀和菩萨","罗怜那竭菩萨","憍曰兜菩萨",
        "那罗达菩萨","须深菩萨","摩诃须萨和菩萨","因坻达菩萨",
        "和伦调菩萨","文殊师利菩萨","得大势菩萨","无尽菩萨","宝檀花菩萨",
        "药王菩萨","药上菩萨","弥勒菩萨",
});
return msgs[random(sizeof(msgs))];
}       

string random_look()
{
string str;     
int i;  
object player,who = this_player();
if ( !wizardp(who) )
        return "你两眼一黑，什么也看不清。。。。";
        
if ( time()<query("last_joke")+10 )
        {       
        i = sizeof(who->name(1));       
        return sprintf("%s氏%s,好%s。。。。\n",
                who->name(1)[0..1],
                who->name(1)[2..i],
                who->query("gender")=="女性"?"妹妹":"儿郎",
                );
        }
player = users()[random(sizeof(users()))];
if ( !player || !living(player) )
        return "风平浪静，一切正常。。。。";

i = sizeof(player->name(1));
set("last_joke",time());
str = NOR WHT"\n\t\t是日，西天雷音寺内，诸相菩萨罗汉与之佛祖论法。\n\n\t\t佛祖：今日听闻%s"NOR WHT"处有一%s氏%s者，与我佛甚是有缘，何当渡之？
\n\t\t%s：%s氏者虽为有缘之人，然其家贫，恐无入我门之资。。。。\n\n\t\t佛祖：。。。。\n\n\t\t%s：今日亦无别等要紧之事，不如散去。。。。\n\n\t\t\t。。。。。。
\n\t\t一干人等净数散去。。。。\n\n"NOR;
str = sprintf(str,
                player->name(1)[0..1],
                player->name(1)[2..i],
                random_name(),
                player->name(1)[0..1],
                random_name(),
                );
message("shout",str,users());
return "\n本次玩笑对象是"+player->name(1);
}

void create ()
{
set ("short", HIW"兔子"NOR YEL"窝"NOR);
set ("long",YEL"\n
深山深处的兔子窝，地下四通八达，不愧"NOR WHT"狡兔三窟"NOR YEL"的称号。
洞口长满了绿油油的"NOR HIG"青草"NOR YEL"，茁壮的向上生长着，显示着其
无比强烈的生命。人类与之比起，又算得了什么？\n
微风抚过，"NOR HIG"绿草"NOR YEL"们随风摆动，享受着它们短暂生命中难得
平静的谢意。
一缕"NOR HIC"阳光"NOR YEL"(light)透过"NOR HIG"青草"NOR YEL"射进洞中，映出斑影婆娑。\n\n
"NOR);
set("exits", ([ 
        "wiz" : "/d/wiz/wizroom",
        "kz" : "/d/city/kezhan",
]));

set("item_desc",([
        "light" : (: random_look :),
]));

set("objects", ([
    "/u/shulele/npc/feng" : 1,
 
]));
set("outdoors", "changan");
set("chat_room",1);
set("no_time",1);
set("mai_corpse_able",1);
set("freeze", 1); 
set("if_bed",1);
set("sleep_room",1);
set("no_upgrade_age",1);
set("valid_startroom", 1);
set("hs_world",1);
set("no_clean_up",1);
setup();
}

void init()
{
if ( !wizardp(this_player()) || this_player()->query("id")!="snowtu" )
      return;
add_action("do_xiugai","xiugai");
add_action("do_set","set");
add_action("do_temp","temp");
add_action("do_lo","lo");
}

int do_lo(string arg)
{
        object who;
                   string msg = @MSG
NORBLK                 ▂     ▂                    NOR
NORBLK               NORBLK▊NORHBWHT  NORBLK▎  NORHBWHTBLK▍ NORBLK▎      NOR
NORYEL               NORBLK▋NORHBWHT  NORHBWHTWHT NORBLK▎ NORHBWHTBLK▎NORWHT▋NOR
NORYEL               NORBLK▋NORHBWHT  NORWHT▊NORBLK▋NORHBWHT  NORHBWHTHIW NORBLK▎NOR
NORBLK               NORBLK▊NORHBWHTBLK_.-~~-._ NORBLK▍NOR
NORBLK             ◢NORHIR◢NORHBWHTHIR◣    ◢NORHIR◣NORBLK◣NOR
NORBLK             NORHIR◢NORHIR██NORHBWHTHIW●●NORHIR██NORHIR◣NOR
NORBLK           NORBLK▉ NORWHT▁▃▄▄▄▃NORBLK█ NOR
NORBLK          ◢NORWHT▅NORHBWHTBLK __--~~~~--__ NORWHT▅NORBLK◣NOR
NORBLK         NORHBWHTBLK▍ .-~            ~-.  NORBLK▍NOR
NORBLK       NORBLK▋NORHBWHTBLK  |                  |  NORBLK▍NOR
NORBLK      ▕NORHBWHTBLK▎  ~-._.---~~---._.-~   NORBLK▌NOR
NORBLK      NORBLK▊NORHBWHTBLK▏      ▁      ▁       NORWHT▊NORBLK▎NOR
NORBLK      NORBLK▊NORHBWHTBLK▎▁▄NORWHT▃▆NORHBWHTBLK      NORWHT▇▄NORHBWHTBLK▄▂ NORWHT▊NORBLK▏NOR
NORBLK      ▁NORHBWHTBLK◣NORWHT▅NORHBWHTBLK                 NORWHT▇▅NORHBWHTBLK◢NORBLK▁__NOR
NORBLK  NORBLK▌NORWHT▆NORHBWHTHIR◥NORHIR◣ NORHBWHTBLK▄▂     ●   ,  ▁▃NORWHT NORHIR◢NORHBWHTHIR◤ NOR
NORHIW  NORHBWHTBLK▍NORHIR██NORHBWHTHIR◥NORBLK██ NORHBWHTBLK◣^-_  _--◢NORBLK▆NORBLK██NORHBWHTHIR◤NORHIR█NORHBWHTWHT█NORBLK▍NOR
NORBLK▌NORHBWHTWHT     NORHBWHTBLK▁ NORBLK︳██▆NORHBWHTBLK◣▔◢NORBLK▆█   ▁NORHBWHTBLK︳NORHBWHTWHT    NORWHT▌NOR
NORBLK  NORHBWHTBLK▍ NORWHT▊NORBLK▎NORHBWHTBLK▍NORBLK◣      ▆▆██  █NORWHT▋NORHBWHTBLK▎ NORHBWHTWHT█ NOR
NORBLK   NORHBWHTYEL NORHBWHTBLK▂NORBLK▏NORBLK▊NORHBWHTBLK▎NORWHT▇▆▄▃▃▃▄▅▇NORHBWHTBLK NORBLK▋ NORBLK▅▃▆NOR
NORBLK        ▕NORHBWHTBLK▍      NORHBWHTWHT            NORWHT▌NORHIW              NOR
NORBLK         NORHBWHTBLK▋                   NORWHT▋NORBLK▏  NORYEL          NOR
NORBLK NORYEL         NORHBWHTYEL NORHBWHTBLK◣     ▁▃▁     NORWHT◤NORBLK▎NOR
NORBLK           NORBLK◣▁▂▄▆▆▅▃▂◢NOR
MSG;
        

        this_player()->set("long",msg);
        return 1;
}       
int do_set(string arg)
{
object me = this_player();
if ( !wizardp(me) || !arg || (arg!="int" && arg!="string") )
    return 0;
this_object()->set_temp("edit_type",arg);
write("====== set 参数为 "+arg+" ======\n");
write("ok.");
return 1;
}
int do_temp(string arg)
{
        if( !arg )  return 0;
        if( !wizardp(this_player()) )
                return 0;

}

int do_lianhua(string arg)
{
        object hua,who;
        
        if( !wizardp(this_player()) )
                return 0;
        if( !arg )
                return 0;
        who = find_player(arg);
        if( !who )
                return 0;
        hua = new("/d/nanhai/obj/lianhua/lianhua.c");
        if( !hua )
                return 0;
        if( !hua->move(who) )
        {
                destruct(hua);
                return 0;
        }
        if( who->query("莲花/"+hua->query("armor_type")) )
        {
                destruct(hua);
                return 0;
        }
        hua->set("莲花/lvl",50);
        hua->set("莲花/addType",({"kee","eff_kee","sen","eff_sen","force","mana"}));
        hua->set("owner_id",who->query("id"));
        hua->set("save_file",(who->query("id")+"/"+hua->query("armor_type")));
        if( hua->save() )   
                who->set("莲花/"+hua->query("armor_type"),1);
        return 1;
}
int do_xiugai(string arg)
{
object who,me = this_player();
mixed temp;
string data,id;
if ( !wizardp(me) || !arg )  
        return 0;
if ( !query_temp("edit_type") )   
        return notify_fail("======== set 参数没设 ========\n");
if ( query_temp("edit_type")=="int" )  
        {
        if ( sscanf(arg,"%s %s %d",id,data,temp)!=3 )  
                return 0;
        }
else if ( query_temp("edit_type")=="string" )  
        {
        if ( sscanf(arg,"%s %s %s",id,data,temp)!=3 )  
                return 0;
        }
else    return notify_fail("======== set 参数没设 ========\n");
who = find_player(id);
if ( !who || !living(who) )
        return notify_fail("======== "+id+" 不在线或状态异常 ========\n");
if ( stringp(temp) && !me->query("env/set") )
tell_object(me,who->name(1)+" "+data+" 设置为 "+temp+" 。\n");
write("ok.\n");
return 1;
}       
