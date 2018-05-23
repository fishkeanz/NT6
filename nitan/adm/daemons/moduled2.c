// This program is a part of NT MudLIB
// moduled.c
// �޸�֧��ͬʱ�������װ by lonely

/*
// ����װ����װ���ݸ�ʽ
<1>
<mod_name>$HIC$���$NOR$</mod_name>
<bas>5</bas>
<num>12</num>
<bas_prop>
add_busy:5
add_weak:5
reduce_busy:5
avoid_weak:5
</bas_prop>
<adv_prop>
avoid_locked:5
add_damage:20
add_skill:50
avoid_busy:5
counter_damage:20
add_locked:5
reduce_damage:5
</adv_prop>
</1>

// ������װ����װ��ʽ...
<M3000>
<mod_name>$CYNGRN$��֮����$NOR$</mod_name>
<mod_parts>
A3000:$CYNGRN$��֮����$NOR$
A3001:$CYNGRN$�����ۼ�$NOR$
A3002:$CYNGRN$��������$NOR$
A3003:$CYNGRN$��֮����$NOR$
A3004:$CYNGRN$����֮��$NOR$
A3005:$CYNGRN$�����ּ�$NOR$
A3006:$CYNGRN$����ս��$NOR$
</mod_parts>
<bas>3</bas>
<num>6</num>
<bas_prop>
avoid_locked:20
add_locked:20
add_busy:10
</bas_prop>
<adv_prop>
reduce_busy:20
avoid_busy:20
add_weak:10
avoid_weak:20
add_damage:20
reduce_damage:20
</adv_prop>
</M3000>

// ��������װ������ʽ...
<A3002>
<type>armor/surcoat</type>
<name>$CYNGRN$��������$NOR$</name>
<id>dragon surcoat</id>
<value>10000</value>
<mod_level>M3000</mod_level>
<mod_name>$CYNGRN$��֮����$NOR$</mod_name>
<require>
level:20
con:30
</require>
<quality_level>2</quality_level>
<armor_prop>
armor:10
</armor_prop>
<enchase>
apply_prop/armor1:10
apply_prop/str:2
apply_prop/dex:1
apply_prop/dodge:1
flute:2
used:2
</enchase>
<insert>
[1]
id:magic stone
name:��ʯ
apply_prop/armor1:10
apply_prop/str:2
[/1]
[2]
id:magic stone
name:��ʯ
apply_prop/dex:1
apply_prop/dodge:1
[/2]
</insert>
</A3002>
*/

#include <ansi.h>

#define EQUIP_LIST      CONFIG_DIR "equiplist/"
#define NCH_CHANNEL(x) CHANNEL_D->channel_broadcast("nch", "��װ���飺"+(string)x)

class eqmodule
{
        string mod_level;       // ��װ��� ��Ϊ������ȡ����
        string mod_name;        // ��װ����
        mapping mod_parts;      // ��װ�������
        int bas;                // ��װ���������������
        int num;                // ��װ�������
        mapping bas_prop;       // ��װ��ϻ�������
        mapping adv_prop;       // ��װ��ϸ߼�����
        mapping add_prop;       // ��װ��ϲ������ԣ���װ����Ӧ����bas_prop+adv_prop+add_prop
}

class eqmodule load_module(string modlevel);
string module_stats(object obj, object owner);
int valid_module(class eqmodule mod_stats);
void equip_module(object ob, object owner);
void unequip_module(object ob, object owner);

void create()
{
        seteuid(ROOT_UID);
}

// ������װ�����ͳ��в����ߴ�ӡ������Ϣ,Ӧ����look.c����
string module_stats(object ob, object owner)
{
        class eqmodule mod_stats;
        string modlevel, mark_level, modname, *apply, *eq_files, *key;
        mapping modparts, data;
        mapping stats;
        int i, j, n, bas, num, len = 0;
        string msg, str, eq_str;

        // ��ʼ������
        msg = "";
        modlevel = query("mod_level", ob);
        modname = query("mod_name", ob);
        mod_stats = new(class eqmodule);
        // ������װ��ȡmod_level
        if( ob->is_item_make() )
        {
                if( query_temp("modequip_min_level_"+modname, owner) ) // ��ȡ��װ����͵ȼ�
                        modlevel = query_temp("modequip_min_level_"+modname, owner);

                if( query_temp("modequip_mark_level_"+modname, owner) )
                        mark_level = query_temp("modequip_mark_level_"+modname, owner);
                else
                        mark_level = modlevel;
        }
        else
                mark_level = modlevel;

        if( classp(query_temp("modequip_"+mark_level+"_"+modname+"_stats", owner)) && query_temp("modequip_"+mark_level+"_"+modname+"_name", owner) )
                mod_stats = query_temp("modequip_"+mark_level+"_"+modname+"_stats", owner); // ֱ�Ӵ�������ϻ�ȡ��װ����
        else
                mod_stats = load_module(modlevel); // ������װ����

        // ���Ϸ���
        if( valid_module(mod_stats) )
        {
                num = mod_stats->num;   // ��װ��������
                // ������װ������������װ����
                if( query_temp("modequip_"+mark_level+"_"+modname+"_name", owner) || !ob->is_item_make() )
                        modparts = mod_stats->mod_parts;

                // ����δ������װ����
                else
                        modparts = query_temp("modparts_" + mark_level+"_"+modname, owner);
                
                if( !mapp(modparts) ) modparts = ([]);
                apply = keys(modparts);
                
                msg = sprintf(HIW"  %s(%d/%d"NOR")\n",
                               modname,
                               query_temp("modamount_"+mark_level+"_"+modname, owner), num);
                msg+= sprintf(NOR "������������������������\n");

                data = query_temp("modparts_" + mark_level+"_"+modname, owner);
                if( !mapp(data) ) data = ([]);
                eq_files = keys(data);

                for( i=0;i<sizeof(apply);i++ )
                {
                        eq_str = "  ";

                        if( member_array(apply[i], eq_files) != -1 )
                                eq_str = WHT "��" NOR;

                        if( query("mod_mark", ob) == apply[i] )
                                if( query_temp("mod_equip", ob) )
                                        eq_str = HIR"��"NOR;
                                else
                                        eq_str = RED"��"NOR;

                        str = sprintf(NOR "��%s%-18s"NOR"��\n",eq_str, modparts[apply[i]]);
                        msg += str;
                }
                if( ob->is_item_make() && member_array(query("mod_mark", ob), apply) == -1 )
                {
                        eq_str = RED"��"NOR;
                        str = sprintf(NOR "��%s%-18s"NOR"��\n",eq_str, ob->name(1));
                        msg += str;
                }

                msg += sprintf(NOR "������������������������\n");
                bas = mod_stats->bas;
                if( query_temp("modamount_"+mark_level+"_"+modname, owner) >= bas )
                        eq_str = HIR;
                else
                        eq_str = HIK;
                //msg += sprintf(HIG"  װ��Ч��"NOR")\n");
                msg += sprintf(NOR "%s������������������������"NOR"\n", eq_str);

                str = sprintf("%d��:", bas);
                msg += sprintf(NOR "%s��"NOR"%-20s%s��"NOR"\n", eq_str, str, eq_str);
                stats = copy(mod_stats->bas_prop);
                key = keys(stats);
                if( sizeof(key) > 0 )
                {
                        for( i=0;i<sizeof(key);i++ )
                        {
                                str = EQUIPMENT_D->chinese(key[i], stats[key[i]]);
                                str = HIG + str + NOR;
#ifndef LONELY_IMPROVED
                                len = color_len(str);
#endif
                                msg += sprintf(NOR "%s��"NOR"%-" + (20 + len) + "s%s��"NOR"\n",
                                                       eq_str, str, eq_str);
                        }
                }
                if( query_temp("modamount_"+mark_level+"_"+modname, owner) == num )
                        eq_str = HIR;
                else
                        eq_str = HIK;
                str = sprintf("%d��:", num);
                msg += sprintf(NOR "%s��"NOR"%-20s%s��"NOR"\n", eq_str, str, eq_str);
                stats = copy(mod_stats->adv_prop);
                key = keys(stats);
                if( sizeof(key) > 0 )
                {
                        for( i=0;i<sizeof(key);i++ )
                        {
                                str = EQUIPMENT_D->chinese(key[i], stats[key[i]]);
                                str = HIG + str + NOR;
#ifndef LONELY_IMPROVED
                                len = color_len(str);
#endif
                                msg += sprintf(NOR "%s��"NOR"%-" + (20 + len) + "s%s��"NOR"\n",
                                                       eq_str, str, eq_str);
                        }
                }

                if( query_temp("modamount_"+mark_level+"_"+modname, owner) >= bas )
                        eq_str = HIR;
                else
                        eq_str = HIK;
                str = sprintf("Add:");
                msg += sprintf(NOR "%s��"NOR"%-20s%s��"NOR"\n", eq_str, str, eq_str);
                stats = copy(mod_stats->add_prop);
                key = keys(stats);
                if( sizeof(key) > 0 )
                {
                        for( i=0;i<sizeof(key);i++ )
                        {
                                str = EQUIPMENT_D->chinese(key[i], stats[key[i]]);
                                str = HIG + str + NOR;
#ifndef LONELY_IMPROVED
                                len = color_len(str);
#endif
                                msg += sprintf(NOR "%s��"NOR"%-" + (20 + len) + "s%s��"NOR"\n",
                                                       eq_str, str, eq_str);
                        }
                }
                msg += sprintf(NOR "%s������������������������"NOR"\n", eq_str);
        }
        return msg;
}

// ���ݼ��һ����װ�Ƿ�Ϸ������Ƿ���ڣ������װ���ݻ��ǳ�ʼ�����ݣ����ǲ����ڡ�
int valid_module(class eqmodule mod_stats)
{
        if( mod_stats->mod_level == "" )
                return 0;

        if( mod_stats->mod_name == "" )
                return 0;
        /*
        if( !(mod_stats->mod_parts) )
                return 0;

        if( !(mod_stats->bas_prop) )
                return 0;

        if( !(mod_stats->adv_prop) )
                return 0;

        if( !(mod_stats->add_prop) )
                return 0;
        */
        return 1;
}

// ������Ų�����װ����
class eqmodule load_module(string modlevel)
{
        string filename, this_file, temp_str;
        object ob;
        class eqmodule item;
        string *field;
        string modname, names, bas_prop, adv_prop;
        int i, bas, num;
        mapping stats;
        string key;
        mixed value;

        if( modlevel[0] == 'M' )
        {
                if ( modlevel[1] == '0' && strlen(modlevel) == 5)
                        filename = EQUIP_LIST+"basic.mod";
                else if ( modlevel[1] == '1' && strlen(modlevel) == 5)
                        filename = EQUIP_LIST+"advance.mod";
                else if ( modlevel[1] == '2' && strlen(modlevel) == 5)
                        filename = EQUIP_LIST+"expert.mod";
                else if ( modlevel[1] == '3' && strlen(modlevel) == 5)
                        filename = EQUIP_LIST+"boss.mod";
        }
        else if( modlevel == "1" || modlevel == "2" || modlevel == "3" || modlevel == "4" )
        {
                // �������װ����װ��
                filename = EQUIP_LIST + "ultra.mod";
        }

        // ��ʼ��һ���յ���װ�ļ�
        item = new(class eqmodule);
        item->mod_level = "";
        item->mod_name = "";
        item->mod_parts = ([]);
        item->add_prop = ([]); // װ���������ӵ���װ����

        // �ļ�Ϊ�ա����ؿ�ֵ��
        if( !stringp(filename) || file_size(filename) < 0 )
                return item;

        this_file = read_file(filename);

        // ������װ��Ų�����������ļ���������ƥ����װ����
        sscanf( this_file, "%*s<"+modlevel+">%s</"+modlevel+">%*s", temp_str);
        sscanf( temp_str, "%*s<mod_name>%s</mod_name>%*s", modname);
        sscanf( temp_str, "%*s<mod_parts>\n%s\n</mod_parts>%*s", names);
        sscanf( temp_str, "%*s<bas>%d</bas>%*s", bas);
        sscanf( temp_str, "%*s<num>%d</num>%*s", num);
        sscanf( temp_str, "%*s<bas_prop>\n%s\n</bas_prop>%*s", bas_prop);
        sscanf( temp_str, "%*s<adv_prop>\n%s\n</adv_prop>%*s", adv_prop);

        item->mod_level = modlevel;
        item->mod_name = trans_color(modname, 3); // ת����ɫ

        if( bas ) item->bas = bas;
        if( num ) item->num = num;

        // �������Ա����������Թؼ��ֺ���װ���ԡ�
        if (bas_prop != "")
        {
                stats = ([]);
                field = explode(bas_prop, "\n");
                for (i=0;i<sizeof(field);i++)
                {
                        //field[i] = replace_string(field[i], " ", "");
                        if( sscanf( field[i], "%s:%d", key, value ) != 2 )
                            sscanf( field[i], "%s:%s", key, value );
                        // ���踴�ӵĽ���
                        stats[key] = value;
                }
                item->bas_prop = copy(stats);
        }

        if (adv_prop != "")
        {
                stats = ([]);
                field = explode(adv_prop, "\n");
                for (i=0;i<sizeof(field);i++)
                {
                        //field[i] = replace_string(field[i], " ", "");
                        if( sscanf( field[i], "%s:%d", key, value ) != 2 )
                            sscanf( field[i], "%s:%s", key, value );
                        // ���踴�ӵĽ���
                        stats[key] = value;
                }
                item->adv_prop = copy(stats);
        }

        // �����ļ����������ļ����ƺ���װ�����������֡�
        if( sizeof(names) )
        {
                stats = ([]);
                field = explode(names, "\n");
                for (i=0;i<sizeof(field);i++)
                {
                        //field[i] = replace_string(field[i], " ", "");
                        field[i] = trans_color(field[i], 3); // ת����ɫ
                        sscanf( field[i], "%s:%s", key, value );
                        stats[key] = value;
                }
                item->mod_parts = copy(stats);
        }

        return item;
}

// װ����װ����
// ob��װ���� ownerװ����
void equip_module(object ob, object owner)
{
        class eqmodule mod_stats;
        class eqmodule tmp_stats;
        mapping data, stats, prop, applied_prop;
        mapping modparts;
        string *apply;
        string modname;
        string modlevel, mark_level, file, name;
        object *inv, *obs;
        int i, j, n, flag = 0;
        int bas, num, change = 0;

        if( ob->is_item_make() )
        {
                // ����װ����װ�������޷�������װ����
                if( ob->item_owner() != query("id", owner) )
                        return;
        }

        modlevel = query("mod_level", ob); // ��װ����š��ȼ�
        modname = query("mod_name", ob);   // modname ���ж��Ƿ�����װ

        inv = all_inventory(owner);
        if( ob->is_item_make() )        // �����װ����
        {
                obs = filter_array(inv,
                        (: query("equipped", $1) && query("mod_name", $1) == $(modname) &&
                           $1->is_item_make() && $1->item_owner() == $(query("id", owner)) :));
                /*
                n = sizeof(obs);
                for( i=0; i<n; i++ ) {
                        if( to_int(query("mod_level", obs[i])) < to_int(modlevel) )
                                modlevel = query("mod_level", obs[i]); // ��ȡ��װ����͵ȼ�
                }
                */

                // ����ȼ���仯
                if( !query_temp("modequip_min_level_"+modname, owner) )
                        set_temp("modequip_min_level_"+modname, modlevel, owner); // ��װ����͵ȼ�
                if( to_int(modlevel) < to_int(query_temp("modequip_min_level_"+modname, owner)) )
                {
                        set_temp("modequip_min_level_"+modname, modlevel, owner); // ��װ����͵ȼ�
                        change = 1;
                }
                else
                        modlevel = query_temp("modequip_min_level_"+modname, owner); // ��װ����͵ȼ�

                if( !query_temp("modequip_mark_level_"+modname, owner) )    // ��װ�ı�ǵȼ�����װ���ĵ�һ����װ�ȼ�Ϊ���
                        set_temp("modequip_mark_level_"+modname, modlevel, owner);
        }
        else
                obs = filter_array(inv,
                        (: query("equipped", $1) && query("mod_name", $1) == $(modname) &&
                           query("mod_level", $1) == $(modlevel) && !$1->is_item_make() :));

        n = sizeof(obs); // װ������װ����
        mod_stats = new(class eqmodule);
        // ��ȡ��װ����
        mod_stats = load_module(modlevel);
        // ���Ϸ���
        if( !valid_module(mod_stats) )
                return;

        bas = mod_stats->bas;
        num = mod_stats->num;

        // ������̻���ʱ����base_name��Ϊmark��������ò��������Ϊmark
        file = query("mod_mark", ob);
        name = query("name", ob);

        if( ob->is_item_make() )
                mark_level = query_temp("modequip_mark_level_"+modname, owner);
        else
                mark_level = modlevel;

        // �����װ���
        modparts=query_temp("modparts_"+mark_level+"_"+modname, owner);
        if( !mapp(modparts) || !sizeof(modparts) ) modparts = ([]);
        modparts[file] = name; 
        set_temp("modparts_"+mark_level+"_"+modname, modparts, owner);

        // �����װ����
        addn_temp("modamount_"+mark_level+"_"+modname, 1, owner);

        // װ������װ���ñ��
        set_temp("mod_equip", 1, ob);

        if( n < bas-1 ) return; // С�����ټ�����װ�������޷�������װ����
        else if( n == bas-1 )
        {
                // ��������װ����ob�����ü�����װ����
                mod_stats->mod_name = modname;  // ������װ���Ʋ���һ��
                if( ob->is_item_make() )        // ������װ�Ĳ�������
                        mod_stats->mod_parts = ([ file : name ]);

                set_temp("mod_active", 1, ob);  // lookʱ����ɫ�仯
                stats = copy(query("enchase/mod_prop", ob));  // �̻�ʱ��ӵ�е�����ͬʱ����
                if( !sizeof(stats) ) stats = ([]);
                for( i=0; i<n; i++ ) {
                        if( ob->is_item_make() )
                        {
                                data = copy(mod_stats->mod_parts);
                                data[query("mod_mark", obs[i])] = obs[i]->name(1);
                                mod_stats->mod_parts = copy(data);
                        }

                        set_temp("mod_active", 1, obs[i]);  // lookʱ����ɫ�仯
                        prop = copy(query("enchase/mod_prop", obs[i]));
                        if( !sizeof(prop) ) continue;
                        apply = keys(prop);
                        for( j=0; j<sizeof(apply); j++ ) {
                                if( undefinedp(stats[apply[j]]) )
                                        stats[apply[j]] = prop[apply[j]];
                                else
                                        stats[apply[j]] += prop[apply[j]];
                        }
                }
                if( sizeof(stats) )
                        mod_stats->add_prop = copy(stats); // ��װ��������

                set_temp("modequip_"+mark_level+"_"+modname+"_stats", mod_stats, owner);  // ������ϱ����װ����
                set_temp("modequip_"+mark_level+"_"+modname+"_name", modname, owner);     // ������ϱ���Ѿ��������װ����

                if( sizeof(stats) ) apply = keys(stats);
                else apply = ({});
                applied_prop = query_temp("apply", owner);
                if( !mapp(applied_prop) )
                        applied_prop = ([]);
                for( i = 0; i<sizeof(apply); i++ )
                {
                        if (apply[i] == "int" || apply[i] == "str" ||
                            apply[i] == "con" || apply[i] == "add_skill" ||
                            apply[i] == "max_qi" || apply[i] == "max_jing")
                                flag = 1;

                        if( undefinedp(applied_prop[apply[i]]) )
                                applied_prop[apply[i]] = stats[apply[i]];
                        else
                                applied_prop[apply[i]] += stats[apply[i]];
                }

                stats = copy(mod_stats->bas_prop); // ��װ��������
                apply = keys(stats);
                for( i = 0; i<sizeof(apply); i++ )
                {
                        if (apply[i] == "int" || apply[i] == "str" ||
                            apply[i] == "con" || apply[i] == "add_skill" ||
                            apply[i] == "max_qi" || apply[i] == "max_jing")
                                flag = 1;

                        if( undefinedp(applied_prop[apply[i]]) )
                                applied_prop[apply[i]] = stats[apply[i]];
                        else
                                applied_prop[apply[i]] += stats[apply[i]];
                }

                set_temp("apply", applied_prop, owner); // ��װ�������Լ�����������Ч
                if( flag ) CHAR_D->setup_char(owner);
                return;
        }
        else if( n == num-1 )
        {
                if( !classp(tmp_stats = copy(query_temp("modequip_"+mark_level+"_"+modname+"_stats", owner))) ||
                    !query_temp("modequip_"+mark_level+"_"+modname+"_name", owner) )
                        error("��װ���Գ��ֻ��ң��������ύbug��admin��\n");

                // ��װȫ����
                mod_stats->mod_name = modname;  // ������װ���Ʋ���һ��
                if( ob->is_item_make() )
                {
                        data = copy(tmp_stats->mod_parts);
                        data[file] = name;
                        mod_stats->mod_parts = copy(data);
                }

                set_temp("mod_active", 1, ob);

                applied_prop=query_temp("apply", owner);
                if( !mapp(applied_prop) )
                        applied_prop = ([]);

                stats = copy(tmp_stats->add_prop);
                if( !sizeof(stats) ) stats = ([]);
                prop = copy(query("enchase/mod_prop", ob)); // ��װ��������
                if( sizeof(prop) )
                {
                        apply = keys(prop);
                        for( i=0; i<sizeof(apply); i++ ) {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                if( undefinedp(applied_prop[apply[i]]) )
                                        applied_prop[apply[i]] = prop[apply[i]];
                                else
                                        applied_prop[apply[i]] += prop[apply[i]];

                                if( undefinedp(stats[apply[i]]) )
                                        stats[apply[i]] = prop[apply[i]];
                                else
                                        stats[apply[i]] += prop[apply[i]];
                        }
                }
                if( sizeof(stats) )
                        mod_stats->add_prop = copy(stats); // ��װ��������

                set_temp("modequip_"+mark_level+"_"+modname+"_stats", mod_stats, owner);  // ������ϱ����װ����
                set_temp("modequip_"+mark_level+"_"+modname+"_name", modname, owner);     // ������ϱ���Ѿ��������װ����
                if( ob->is_item_make() )
                        set_temp("modequip_itemmake_level", to_int(modlevel), owner);     // ������ϱ��ȫ�׵�������װ�ȼ�

                if( change )
                {
                        stats = copy(tmp_stats->bas_prop); // ���ڿ���װ�ȼ��仯����װ�������Ի�仯����ȥ��ԭ�е�
                        apply = keys(stats);
                        for( i=0; i<sizeof(apply); i++ ) {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                applied_prop[apply[i]] -= stats[apply[i]];
                        }

                        stats = copy(mod_stats->bas_prop); // ��װ��������
                        apply = keys(stats);
                        for( i = 0; i<sizeof(apply); i++ )
                        {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                if( undefinedp(applied_prop[apply[i]]) )
                                        applied_prop[apply[i]] = stats[apply[i]];
                                else
                                        applied_prop[apply[i]] += stats[apply[i]];
                        }
                }
                stats = copy(mod_stats->adv_prop);      // ��װ�߼�����
                apply = keys(stats);
                for( i = 0; i<sizeof(apply); i++ )
                {
                        if (apply[i] == "int" || apply[i] == "str" ||
                            apply[i] == "con" || apply[i] == "add_skill" ||
                            apply[i] == "max_qi" || apply[i] == "max_jing")
                                flag = 1;

                        if( undefinedp(applied_prop[apply[i]]) )
                                applied_prop[apply[i]] = stats[apply[i]];
                        else
                                applied_prop[apply[i]] += stats[apply[i]];
                }

                set_temp("apply", applied_prop, owner);
                if( flag ) CHAR_D->setup_char(owner);
                return;
        }
        else
        {
                // �Ѿ�������װ���ԣ������µ���װ��������
                // ���ȼ���Ѿ��������װ�����Ƿ��������
                if( !classp(tmp_stats = copy(query_temp("modequip_"+mark_level+"_"+modname+"_stats", owner))) ||
                    !query_temp("modequip_"+mark_level+"_"+modname+"_name", owner) )
                        error("��װ���Գ��ֻ��ң��������ύbug��admin��\n");

                mod_stats->mod_name = modname;  // ������װ���Ʋ���һ��

                if( ob->is_item_make() )
                {
                        data = copy(tmp_stats->mod_parts);
                        data[file] = name;
                        mod_stats->mod_parts = copy(data);
                }

                set_temp("mod_active", 1, ob);

                applied_prop=query_temp("apply", owner);
                if( !mapp(applied_prop) )
                        applied_prop = ([]);

                stats = copy(tmp_stats->add_prop);
                if( !sizeof(stats) ) stats = ([]);
                prop = copy(query("enchase/mod_prop", ob)); // ��װ��������
                if( sizeof(prop) )
                {
                        apply = keys(prop);
                        for( i=0; i<sizeof(apply); i++ ) {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                if( undefinedp(applied_prop[apply[i]]) )
                                        applied_prop[apply[i]] = prop[apply[i]];
                                else
                                        applied_prop[apply[i]] += prop[apply[i]];

                                if( undefinedp(stats[apply[i]]) )
                                        stats[apply[i]] = prop[apply[i]];
                                else
                                        stats[apply[i]] += prop[apply[i]];
                        }
                }
                if( sizeof(stats) )
                        mod_stats->add_prop = copy(stats); // ��װ��������

                set_temp("modequip_"+mark_level+"_"+modname+"_stats", mod_stats, owner);  // ������ϱ����װ����
                set_temp("modequip_"+mark_level+"_"+modname+"_name", modname, owner);     // ������ϱ���Ѿ��������װ����

                if( change )
                {
                        stats = copy(tmp_stats->bas_prop); // ���ڿ���װ�ȼ��仯����װ�������Ի�仯����ȥ��ԭ�е�
                        apply = keys(stats);
                        for( i=0; i<sizeof(apply); i++ ) {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                applied_prop[apply[i]] -= stats[apply[i]];
                        }

                        stats = copy(mod_stats->bas_prop); // ��װ��������
                        apply = keys(stats);
                        for( i = 0; i<sizeof(apply); i++ )
                        {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                if( undefinedp(applied_prop[apply[i]]) )
                                        applied_prop[apply[i]] = stats[apply[i]];
                                else
                                        applied_prop[apply[i]] += stats[apply[i]];
                        }
                }

                set_temp("apply", applied_prop, owner);
                if( flag ) CHAR_D->setup_char(owner);
                return;
        }
}


// ж����װ����
// ob��װ���� ownerװ����
void unequip_module(object ob, object owner)
{
        class eqmodule mod_stats;
        class eqmodule tmp_stats;
        mapping data, stats, prop, applied_prop;
        mapping modparts;
        string *apply;
        string modname;
        string modlevel, mark_level, file, name;
        object *inv, *obs;
        int i, j, n, flag = 0;
        int bas, num, change = 0;

        if( ob->is_item_make() ) {
                if( ob->item_owner() != query("id", owner) )
                        return;
        }

        modlevel = query("mod_level", ob);
        modname = query("mod_name", ob);
        mod_stats = new(class eqmodule);

        inv = all_inventory(owner);
        if( ob->is_item_make() )        // �����װ����
        {
                obs = filter_array(inv,
                        (: query("equipped", $1) && query("mod_name", $1) == $(modname) &&
                           $1->is_item_make() && $1->item_owner() == $(query("id", owner)) :));

                modlevel = query_temp("modequip_min_level_"+modname, owner); // ��װ����͵ȼ�
                if( query("mod_level", ob) == modlevel ) // ��װ�ȼ����ܻ�仯
                {
                        modlevel = "4";
                        n = sizeof(obs);
                        for( i=0; i<n; i++ ) {
                                if( obs[i] == ob ) continue;
                                if( to_int(query("mod_level", obs[i])) < to_int(modlevel) )
                                        modlevel = query("mod_level", obs[i]); // ��ȡ��װ����͵ȼ�
                        }
                        set_temp("modequip_min_level_"+modname, modlevel, owner); // ��װ����͵ȼ�
                        if( modlevel != query("mod_level", ob) )
                                change = 1;
                }
                if( !modlevel ) modlevel = query("mod_level", ob);

        }
        else
                obs = filter_array(inv,
                        (: query("equipped", $1) && query("mod_name", $1) == $(modname) &&
                           query("mod_level", $1) == $(modlevel) && !$1->is_item_make() :));

        n = sizeof(obs);
        mod_stats = new(class eqmodule);
        // ��ȡ��װ����
        mod_stats = load_module(modlevel);
        // ���Ϸ���
        if( !valid_module(mod_stats) )
                return;

        // �����װ����ob�Ƿ�����װ���һ��
        file = query("mod_mark", ob);
        name = query("name", ob);

        if( ob->is_item_make() )
                mark_level = query_temp("modequip_mark_level_"+modname, owner);
        else
                mark_level = modlevel;

        // �����װ�ļ�
        modparts=query_temp("modparts_"+mark_level+"_"+modname, owner);
        if( !mapp(modparts) || !sizeof(modparts) )
                modparts = ([]);
        
        map_delete(modparts, file);
        set_temp("modparts_"+mark_level+"_"+modname, modparts, owner);

        if( ob->is_item_make() && !sizeof(modparts) )
        {
                delete_temp("modequip_min_level_"+modname, owner);
                delete_temp("modequip_mark_level_"+modname, owner);
        }

        // �����װ����
        addn_temp("modamount_"+mark_level+"_"+modname, -1, owner);
        if( query_temp("modamount_"+mark_level+"_"+modname, owner)<0 )
                delete_temp("modamount_"+mark_level+"_"+modname, owner);

        // ɾ��װ����װ�������
        delete_temp("mod_equip", ob);
        delete_temp("mod_active", ob); // lookʱ����ʾmod_prop��ɫ�仯

        /*
        if( query_temp("modamount_"+mark_level+"_"+modname, owner) != n )
                error("��װװ���������ֻ��ң��������ύbug��admin��\n");
        */

        bas = mod_stats->bas;
        num = mod_stats->num;

        // ���onwer����װ������װ����+1δ�ﵽ��װ�������ʱ��
        // ��Ϊû���ӹ���װ���ԣ����Բ���Ҫִ��ɾ��onwer��װ����
        if( n < bas ) return;
        else if( n == bas ) {
                if( !classp(tmp_stats = copy(query_temp("modequip_"+mark_level+"_"+modname+"_stats", owner))) ||
                    !query_temp("modequip_"+mark_level+"_"+modname+"_name", owner) )
                        error("��װ���Գ��ֻ��ң��������ύbug��admin��\n");

                // ����bas����ж����װ��������������װ�����в������Ժͻ�������
                for( i=0; i<sizeof(obs); i++ )
                {
                        if( obs[i] == ob ) continue;
                        delete_temp("mod_equip", obs[i]);   // ȡ��װ�����
                        delete_temp("mod_active", obs[i]);  // ȡ��������
                }

                applied_prop = query_temp("apply", owner);
                stats = copy(tmp_stats->add_prop); // ��������
                apply = keys(stats);
                for( i=0; i<sizeof(apply); i++ ) {
                        if (apply[i] == "int" || apply[i] == "str" ||
                            apply[i] == "con" || apply[i] == "add_skill" ||
                            apply[i] == "max_qi" || apply[i] == "max_jing")
                                flag = 1;

                        applied_prop[apply[i]] -= stats[apply[i]];
                }

                stats = copy(tmp_stats->bas_prop); // ��������
                apply = keys(stats);
                for( i=0; i<sizeof(apply); i++ ) {
                        if (apply[i] == "int" || apply[i] == "str" ||
                            apply[i] == "con" || apply[i] == "add_skill" ||
                            apply[i] == "max_qi" || apply[i] == "max_jing")
                                flag = 1;

                        applied_prop[apply[i]] -= stats[apply[i]];
                }

                delete_temp("modequip_"+mark_level+"_"+modname+"_stats", owner);
                delete_temp("modequip_"+mark_level+"_"+modname+"_name", owner);

                set_temp("apply", applied_prop, owner);
                if (flag) CHAR_D->setup_char(owner);
                return;
        }
        else if( n == num ) {
                if( !classp(tmp_stats = copy(query_temp("modequip_"+mark_level+"_"+modname+"_stats", owner))) ||
                    !query_temp("modequip_"+mark_level+"_"+modname+"_name", owner) )
                        error("��װ���Գ��ֻ��ң��������ύbug��admin��\n");
                
                mod_stats->mod_name = modname;  // ������װ���Ʋ���һ��
                if( ob->is_item_make() )
                {
                        data = copy(tmp_stats->mod_parts);
                        map_delete(data, file);
                        mod_stats->mod_parts = copy(data);
                }
                // ȡ����װ�Ĳ����������Լ���װ�߼�����
                applied_prop = query_temp("apply", owner);
                stats = copy(tmp_stats->add_prop);
                prop = copy(query("enchase/mod_prop", ob)); // ��װ��������
                if( sizeof(prop) )
                {
                        apply = keys(prop);
                        for( i=0; i<sizeof(apply); i++ ) {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                applied_prop[apply[i]] -= prop[apply[i]];
                                stats[apply[i]] -= prop[apply[i]];
                        }
                        mod_stats->add_prop = copy(stats);
                }

                set_temp("modequip_"+mark_level+"_"+modname+"_stats", mod_stats, owner);
                if( ob->is_item_make() )
                        delete_temp("modequip_itemmake_level", owner); // ɾ����װ�ȼ���suit special����

                stats = copy(tmp_stats->adv_prop);      // ��װ�߼�����
                apply = keys(stats);
                for( i = 0; i<sizeof(apply); i++ )
                {
                        if (apply[i] == "int" || apply[i] == "str" ||
                            apply[i] == "con" || apply[i] == "add_skill" ||
                            apply[i] == "max_qi" || apply[i] == "max_jing")
                                flag = 1;

                        applied_prop[apply[i]] -= stats[apply[i]];
                }
                if( change )
                {
                        stats = copy(tmp_stats->bas_prop); // ���ڿ���װ�ȼ��仯����װ�������Ի�仯����ȥ��ԭ�е�
                        apply = keys(stats);
                        for( i=0; i<sizeof(apply); i++ ) {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                applied_prop[apply[i]] -= stats[apply[i]];
                        }

                        stats = copy(mod_stats->bas_prop); // ��װ��������
                        apply = keys(stats);
                        for( i = 0; i<sizeof(apply); i++ )
                        {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                if( undefinedp(applied_prop[apply[i]]) )
                                        applied_prop[apply[i]] = stats[apply[i]];
                                else
                                        applied_prop[apply[i]] += stats[apply[i]];
                        }
                }

                set_temp("apply", applied_prop, owner);
                if( flag ) CHAR_D->setup_char(owner);
                return;
        }
        else
        {
                if( !classp(tmp_stats = copy(query_temp("modequip_"+mark_level+"_"+modname+"_stats", owner))) ||
                    !query_temp("modequip_"+mark_level+"_"+modname+"_name", owner) )
                        error("��װ���Գ��ֻ��ң��������ύbug��admin��\n");
                
                mod_stats->mod_name = modname;  // ������װ���Ʋ���һ��
                if( ob->is_item_make() )
                {
                        data = copy(tmp_stats->mod_parts);
                        map_delete(data, file);
                        mod_stats->mod_parts = copy(data);
                }
                // ����ȡ����װ�����Ĳ�������
                applied_prop = query_temp("apply", owner);
                stats = copy(tmp_stats->add_prop);
                prop = copy(query("enchase/mod_prop", ob)); // ��װ��������
                if( sizeof(prop) )
                {
                        apply = keys(prop);
                        for( i=0; i<sizeof(apply); i++ ) {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                applied_prop[apply[i]] -= prop[apply[i]];
                                stats[apply[i]] -= prop[apply[i]];
                        }
                        mod_stats->add_prop = copy(stats);
                }

                set_temp("modequip_"+mark_level+"_"+modname+"_stats", mod_stats, owner);

                if( change )
                {
                        stats = copy(tmp_stats->bas_prop); // ���ڿ���װ�ȼ��仯����װ�������Ի�仯����ȥ��ԭ�е�
                        apply = keys(stats);
                        for( i=0; i<sizeof(apply); i++ ) {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                applied_prop[apply[i]] -= stats[apply[i]];
                        }

                        stats = copy(mod_stats->bas_prop); // ��װ��������
                        apply = keys(stats);
                        for( i = 0; i<sizeof(apply); i++ )
                        {
                                if (apply[i] == "int" || apply[i] == "str" ||
                                    apply[i] == "con" || apply[i] == "add_skill" ||
                                    apply[i] == "max_qi" || apply[i] == "max_jing")
                                        flag = 1;

                                if( undefinedp(applied_prop[apply[i]]) )
                                        applied_prop[apply[i]] = stats[apply[i]];
                                else
                                        applied_prop[apply[i]] += stats[apply[i]];
                        }
                }
                set_temp("apply", applied_prop, owner);
                if (flag) CHAR_D->setup_char(owner);
                return;
        }
}

// �Ƴ���װ�ؼ�Ч��
void remove_effect(object me, string name, int level)
{
        int i;
        string *jifa_list;

        if( query_temp("suit_special", me) )
        {
                delete_temp("suit_special");
                jifa_list = query_temp("suit/jifa_list", me);
                if( sizeof(jifa_list) )
                {
                        for (i = 0; i < sizeof(jifa_list); i ++)
                        {
                                addn_temp("apply/" + jifa_list[i], -1 * level, me);
                        }
                }
                delete_temp("suit/jifa_list", me);
                tell_object(me, HIC "�����װ�ؼ�" + name + HIC "������ϣ�\n");
                return;
        }
}

void remove_effect2(object me, string name, int damage, int armor)
{
        if( query_temp("suit_defense", me) )
        {
                addn_temp("apply/damage", -1 * damage, me);
                addn_temp("apply/armor", -1 * armor, me);

                delete_temp("suit_defense", me);
                tell_object(me, HIC "�����װ�ؼ�" + name + HIC "������ϣ�\n");
                return;
        }
}

// �������� /cmds/std/suit.c ʹ����װ�ؼ�
public int suit_special(object me, string arg)
{
        int i;
        mapping jifa_map;
        string *jifa_list;
        int slvl;

        slvl = query_temp("modequip_itemmake_level", me);

        /*
        �����⸽����װ�ؼ� �ǳ�֮��(stars)��ʩչ(special stars <��Ŀ>)
        �����������ĳ��״̬ 50% �������� 10 ���ӡ�
        ���ӹ��������Ӷ����������мܡ������ڹ�������Ч���������˺�
        �������֡������С������˺����������С�˫���˺���
        �����˺����ֿ������������С����з������о�Ч������ȡЧ��
        */


        if( slvl < 2 )
        {
                return notify_fail("��Ŀǰ����װ�ȼ��������޷�ʹ����װ�ؼ���\n");
        }

        sscanf(arg, "special %s", arg);
        if (slvl == 2)
        {
                if( arg != "changying" )
                        return notify_fail("��Ŀǰ���߱�����װ�ؼ���\n");

                if( query_temp("suit_special", me) )
                        return notify_fail("�������������ô����ؼ��У����Ժ���ʹ�á�\n");

                if( query("neili", me) < query("max_neili", me))
                        return notify_fail("���ʣ���������㣬�޷�ʩչ��װ������\n");
                set("neili", 0, me);

                message_combatd(HIC "$Nһ��������ȫ��װ��ɲ�Ǽ�������ʣ��������֣�\n" NOR, me);
                set_temp("suit_special", 100, me);

                jifa_list = ({});
                jifa_map = me->query_skill_map();
                if( sizeof(jifa_map) )
                {
                        jifa_list = keys(jifa_map);

                        set_temp("suit/jifa_list", jifa_list, me);
                        for (i = 0; i < sizeof(jifa_list);i ++)
                        {
                                addn_temp("apply/" + jifa_list[i], 200, me);
                        }
                }

                me->start_call_out((: call_other,  __FILE__, "remove_effect", me, HIC "����" NOR, 200 :), 300);
                return 1;
        }

        if( slvl == 3 )
        {
                if( arg != "zongheng" && arg != "tianzhidao" )
                        return notify_fail("��Ŀǰ���߱�����װ�ؼ���\n");

                if( arg == "tianzhidao" )
                {
                        if( me->query_temp("suit_special") )
                                return notify_fail("�������������ô����ؼ��У����Ժ���ʹ�á�\n");

                        if( query("neili", me) < query("max_neili", me))
                                return notify_fail("���ʣ���������㣬�޷�ʩչ��װ������\n");
                        set("neili", 0, me);

                        message_combatd(HIG "$Nʩչ����װ��������֮������˲��ȫ��������ɫ��â��\n" NOR, me);
                        set_temp("suit_special", 150, me);

                        jifa_list = ({});
                        jifa_map = me->query_skill_map();
                        if( sizeof(jifa_map) )
                        {
                                jifa_list = keys(jifa_map);

                                set_temp("suit/jifa_list", jifa_list, me);
                                for (i = 0; i < sizeof(jifa_list);i ++)
                                {
                                        addn_temp("apply/" + jifa_list[i], 400, me);
                                }
                        }
                        me->start_call_out((: call_other,  __FILE__, "remove_effect", me, HIG "��֮��" NOR, 400:), 600);
                        return 1;
                }
                if( arg == "zongheng" )
                {
                        if( query_temp("suit_defense", me) )
                                return notify_fail("�������������ô����ؼ��У����Ժ���ʹ�á�\n");

                        if( query("neili", me) < query("max_neili", me))
                                return notify_fail("���ʣ���������㣬�޷�ʩչ��װ������\n");
                        set("neili", 0, me);

                        message_combatd(HIG "$Nʩչ����װ�������ݺ᡹���������һ��һ�������ӹ�����ת��$N���ܣ�\n" NOR, me);
                        set_temp("suit_defense", 1, me);
                        addn_temp("apply/damage", 10000, me);
                        addn_temp("apply/armor", 5000, me);

                        me->start_call_out((: call_other,  __FILE__, "remove_effect2", me, HIG "��֮��" NOR, 10000, 5000 :), 300);
                        return 1;
                }
        }

        if( slvl == 4 )
        {
                if (arg != "juedai" && arg != "tianjiao")
                        return notify_fail("��Ŀǰ���߱�����װ�ؼ���\n");

                if (arg == "tianjiao")
                {
                        if( query_temp("suit_special", me) )
                                return notify_fail("�������������ô����ؼ��У����Ժ���ʹ�á�\n");

                        if( query("neili", me) < query("max_neili", me))
                                return notify_fail("���ʣ���������㣬�޷�ʩչ��װ������\n");
                        set("neili", 0, me);

                        message_combatd(HIM "$N���ֶ����������޷���������������дʣ������֮�꡹��Ȼ�ŷ�������\n" NOR, me);
                        set_temp("suit_special", 200, me);

                        jifa_list = ({});
                        jifa_map = me->query_skill_map();
                        if( sizeof(jifa_map) )
                        {
                                jifa_list = keys(jifa_map);

                                set_temp("suit/jifa_list", jifa_list, me);
                                for (i = 0; i < sizeof(jifa_list);i ++)
                                {
                                        addn_temp("apply/" + jifa_list[i], 600, me);
                                }
                        }

                        me->start_call_out((: call_other,  __FILE__, "remove_effect", me, HIM "���֮��" NOR, 600 :), 900);
                        return 1;
                }
                if( arg == "juedai" )
                {
                        if( query_temp("suit_defense", me) )
                                return notify_fail("�������������ô����ؼ��У����Ժ���ʹ�á�\n");

                        if( query("neili", me) < query("max_neili", me))
                                return notify_fail("���ʣ���������㣬�޷�ʩչ��װ������\n");
                        set("neili", 0, me);

                        message_combatd(HIM "$NĿ����棬�����Ϲ����֣����˲���������ʵ��һ����򜣬���뿹�⣡\n" NOR, me);
                        set_temp("suit_defense", 1, me);
                        addn_temp("apply/damage", 15000, me);
                        addn_temp("apply/armor", 10000, me);

                        me->start_call_out((: call_other,  __FILE__, "remove_effect2", me, HIM "�^�����" NOR, 15000, 10000 :), 600);
                        return 1;
                }
        }

        if( slvl == 5 )
        {
                if( arg != "zhanws1" && arg != "zhanws2" )
                        return notify_fail("��Ŀǰ���߱�����װ�ؼ���\n");

                if( arg == "zhanws2" )
                {
                        if( query_temp("suit_special", me) )
                                return notify_fail("�������������ô����ؼ��У����Ժ���ʹ�á�\n");

                        if( query("neili", me) < query("max_neili", me))
                                return notify_fail("���ʣ���������㣬�޷�ʩչ��װ������\n");
                        set("neili", 0, me);

                        message_combatd(HIR "$N��ͷ���죬��Ȼһ����̾����Ȼȫ������ɫ�Ĺ�â������ɱ�����֣�\n" NOR, me);
                        set_temp("suit_special", 300, me);

                        jifa_list = ({});
                        jifa_map = me->query_skill_map();
                        if( sizeof(jifa_map) )
                        {
                                jifa_list = keys(jifa_map);

                                set_temp("suit/jifa_list", jifa_list, me);
                                for (i = 0; i < sizeof(jifa_list);i ++)
                                {
                                        addn_temp("apply/" + jifa_list[i], 1000, me);
                                }
                        }
                        me->start_call_out((: call_other,  __FILE__, "remove_effect", me, HIR "�ء��o�p" NOR, 1000 :), 1800);
                        return 1;
                }
                if( arg == "zhanws1" )
                {
                        if( query_temp("suit_defense", me) )
                                return notify_fail("�������������ô����ؼ��У����Ժ���ʹ�á�\n");

                        if( query("neili", me) < query("max_neili", me))
                                return notify_fail("���ʣ���������㣬�޷�ʩչ��װ������\n");
                        set("neili", 0, me);

                        message_combatd(HIR "$N��Хһ�����ḧȫ��װ��������֮��������ɫѪâ�������ĵ��ߺ���\n" NOR, me);
                        set_temp("suit_defense", 1, me);
                        addn_temp("apply/damage", 20000, me);
                        addn_temp("apply/armor", 10000, me);

                        me->start_call_out((: call_other,  __FILE__, "remove_effect2", me, HIR "�𡤟o�p" NOR, 20000, 10000 :), 1200);
                        return 1;
                }
        }

        return 1;
}