// This program is a part of NT MudLIB

#include <ansi.h>

// classp ?
// abilities

// 基本能力
string *bas_ability = ({
        "str",
        "int",
        "con",
        "dex",
        "kar",
        "qi",
        "jing",
        "neili",
        "jingli",
        "ref_neili",
        "ref_qi",
        "ref_jing",
        "attack",
        "dodge",
        "parry",
        "fatal",
        "full_self",
        "avoid_busy",
        "avoid_weak",
        "reduce_damage",
});

mapping max_ability = ([
        "str"   : 500,
        "int"   : 500,
        "con"   : 500,
        "dex"   : 500,
        "kar"   : 70,
        "qi"    : 10,
        "jing"  : 10,
        "neili" : 10,
        "jingli": 10,
        "ref_neili": 20,
        "ref_qi"   : 20,
        "ref_jing" : 20,
        "attack": 10,
        "dodge" : 10,
        "parry" : 10,
        "fatal" : 10,
        "full_self" : 10,
        "avoid_busy": 10,
        "avoid_weak": 10,
        "reduce_damage": 10,
]);

string *bas_ability_info = ({
        "角色力量值提高１点\t","角色才智值提高１点\t","角色体质值提高１点\t",
        "角色身法值提高１点\t","角色运气值提高１点\t","最大气血值提高１％\t",
        "最大精气值提高１％\t","最大内力值提高１％\t","最大精力值提高１％\t",
        "恢复气血提高５０点\t","恢复精气提高５０点\t","恢复内力提高５０点\t",
        "直接命中机会增１％\t","直接闪避机会增１％\t","直接招架机会增１％\t",
        "触发暴击机会增１％\t","战神附体机会增１％\t","免疫忙乱机会增１％\t",
        "免疫虚弱机会增１％\t","化解伤害比例增１％\t",
});

mapping bas_ability_pot = ([
        "attack":       2,
        "dodge":        2,
        "parry":        2,
        "fatal":        3,
        "full_self":    3,
        "avoid_busy":   3,
        "avoid_weak":   3,
        "reduce_damage":3,
]);

mapping fam_ability = ([
        "武当派":({"reduce_busy", "add_busy", "tjq_defense", "tu_damage", "luan_attack"}),
]);

mapping fam_ability_info = ([
        "少林派": ({
                        "当yijinjing金刚不坏体神功yun jin,且气血降低到20/25/30/35/40%出现怒目金刚效果",
                        "一拍两散 命中降低 80/70/60/50/40 %，伤害增加100/200/300/400/500%,同时内力为0，精气降低 90%",
                        "少林弟子持钝器或空手时，伤害额外增加 2/4/6/8/10%",
                        "激发招架为金刚不坏体时，增加1/2/3/4/5%的伤害反射",
                        "少林弟子每多修习10种parry skill，战斗保护力增加1/2/3/4/5%",
                }),
        "武当派": ({
                        "技能组合化解忙乱几率＋１％",
                        "技能组合忙乱对手几率＋１％",
                        "太极拳的特殊防御提高＋１％",
                        "绝招太极图伤害力提高＋１％",
                        "绝招乱环诀命中率提高＋１％",
                }),
        "关外胡家":({
                        "冷月寒气降低对手闪避  2/4/6/8/10%",            
                        "刀剑七重天倍击几率增加 6/12/18/24/30%",
                         "破定军山附带致死效果几率  2/4/6/8/10%",
                        "妙手驱毒效果增加 (加满则免疫毒素) 20/40/60/80/100%",
                        "当气血降低到20/25/30/35/40%出现热血效果",
                }),
                
]);

string *tf_ability = ({
        "gold_find",
        "double_day",
        "add_zq",
        
        "kingkong",
        "magic_find",
        "ability_delay",
        
        "anti_blow",
        "add_damage",
        "dealer",
        
        "indentily",
        "wushen",
        "qn_lose",
        
        "attack"
        "dodge",
        "add_exp",
        
        "avoid_cd",
        "add_damage",
        "avoid_dodge",
        
        "reduce_damage",
        "add_buff",
        "avoid_busy",
        
        "defense",
        "double_damage",
        "shashen",
        
        "add_blade",
        "add_sword",
        "add_unarmed"
});

string *tf_ability_info = ({
        /*
战斗回精：战斗时自动恢复精气。
     战斗回气：战斗时自动恢复气血。
        第一境界

财运（1）：可在完成任务后获得额外的金钱。
练气（1）：可增加每天双倍经验时间1小时。
神佑（1）：每级获得的先天真气值增加1点。
神佑
战斗时当HP=0时，20%的几率出现神佑复活效果，复活以后拥有少量HP
第二境界

金刚（2）：增加对boss级别怪物的绝对防御5%、10%点。
强运（2）：掉落装备机率增加5%、10%点。
心音（2）：辅助武功ability效果持续时间增加25%、50%。
必中
普通物理攻击命中100%
武器
急速就是增加出招的几率 暴击是双倍伤害几率 格挡是减免30%伤害几率。命中是无视闪避招架的几率 精通是perform伤害加成
第三境界

明镜（3）：使玩家有20%、40%、60%的几率防止怪物的必杀技。
伏魔（2）：增加对boss怪物的绝对伤害5%、10%点。
商才（2）：经商利润提高10%，买东西价格降低10%。
迷踪
逃跑几率增加
靴
第四境界

神眼（3）：鉴定时有33%、66%、100%几率能让宝石和装备的等级提高一级。
武神（3）：攻击和防御效果增加3%、6%、10%。
天赐（4）：死亡时有5%、10%、15%、20%的机率不掉潜能。
力量祝福
提升自身及队友的伤害力，消耗愤怒值40。
战神祝福
全体队友伤害力上升，消耗愤怒值70。
防御祝福
提升自身及队友的防御力，消耗愤怒值40。
守护祝福
全体队友物理防御力上升，消耗愤怒值80。
第五境界

鹰眼（3）：攻击中有50%几率触发，命中增加5%、10%、15%（BUFF类）。
轻灵（3）：攻击中有50%几率触发，躲闪增加5%、10%、15%（BUFF类）。
神蕴（3）：任务中获取的5%、10%、15%的额外经验。

第六境界

归真（2）：特殊武功，物品（天赋技能，饰品）的冷却时间降低10%、20% 。
通明（5）：每一次攻击有4%、5%、6%、7%、8%的几率进入通明状态，造成的伤害增加 (对方等级*4%、8%、12%、20%)点（BUFF类）。
强袭（5）：每一次攻击有5%、10%、15%、20%、25%的机率让敌方除凌波微步之外的所有轻功的特殊防御均无效。
破防提高：我的理解是在目前破防的技术上增加一个比例。比如破防提高10％，那么如果你破防10点，
       就变成了11点。
   命中加强：说实话，我看不出它有什么作用来。
   双倍打击：在你造成伤害的基础上，加2倍，重要的一个属性。比例越高越好。
   致命打击：非常重要的属性，造成比例打血效果。
致命打击：非常重要的属性，造成比例打血效果。
第七境界

铁布杉（3）：有3%、6%、10%的几率在受到攻击时抵抗一半的伤害。
极境（5）：当生命值低于40%时，获得基本属性（无装备时候的属性）增加4%、8%、12%、16%、20%的效果
逍遥（3）：战斗中有10%、20%、30%的机率消除忙乱。

第八境界

武炼（5）：受到暴击伤害后有20%、40%、60%、80%、100%几率增加防御（等级*5%）。
顿悟（3）：攻击时增加5%、10%、15%的双倍攻击机率。
杀神（3）：无视目标抗性，直接造成30%、40%、50%的二次攻击伤害。
杀戮-攻击时有几率对当前房间所有人进行一次攻击
清心咒
解除各种异常状态，如被封物理、魔法、中毒、混乱等，消耗愤怒值50。
me->query_craze() < 1000
消耗愤怒burning
me->cost_craze()
berserk增加愤怒
第九境界

刀皇（5）：使用刀法武功增加4%、8%、12%、16%、20%的命中及伤害效果。
剑圣（5）：使用剑法武功增加4%、8%、12%、16%、20%的命中及伤害效果。
拳王（5）：使用拳法武功增加4%、8%、12%、16%、20%的命中及伤害效果。
*/
});
        
mapping max_ability3 = ([
        "str"   : 1,
        "int"   : 1,
        "con"   : 2,
        "dex"   : 2,
        "kar"   : 2,
        "qi"    : 10,
        "jing"  : 10,
        "neili" : 10,
        "jingli": 10,
        "ref_neili": 20,
        "ref_qi"   : 20,
        "ref_jing" : 20,
        "attack": 10,
        "dodge" : 10,
        "parry" : 10,
        "fatal" : 10,
        "full_self" : 10,
        "avoid_busy": 10,
        "avoid_weak": 10,
        "reduce_damage": 10,
]);        
        

int do_ability_cost(object ob, string arg, int flag);
int check_ability(object me,string arg, int flag);
int improve_ability(object me,string arg, int flag);
int valid_ability_improve(object me, string arg, int flag);
string get_ability_idf(object me,string arg, int flag);
string get_ability_info(object ob, string arg, int flag);

int valid_ability_improve(object me, string arg, int flag)
{
        int level;
        int a_lvl;

        a_lvl = check_ability(me, arg, flag);
        level = max_ability[arg];
        if( a_lvl >= level ) 
                return 0;
 
        return 1;
}

varargs int check_ability(object me,string arg, int flag)
{
        int result;
        
        // Ability 门派
        result=query("ability2/"+query("family/family_name", me)+"/"+arg, me)+query("ability1/"+arg, me);
                
        return result;
}

int check_ability_cost(object me, string arg, int flag) 
{
        int cost;

        if( flag == 1 ) {
                cost = 2;
        } else {
                if( member_array(arg, bas_ability) == -1 ) {
                        return 0;
                }
                cost = bas_ability_pot[arg];
                if( !cost ) {
                        cost = 1;
                }
        }
        return cost;
}

// 每次耗费一点能力点
int do_ability_cost(object me, string arg, int flag)
{
        int cost;

        cost = check_ability_cost(me, arg, flag);
        
        if( flag == 1 || flag == 0 ) {
                if( cost>query("ability", me)-query("learned_ability", me)){
                        return 0;
                }
                addn("learned_ability", cost, me);
        }
        
        return 1;
}

int improve_ability(object me,string arg, int flag)
{
        int lv;
        string myclass;

        myclass = query("family/family_name", me);
        if( !flag ) {
                addn("ability1/"+arg, 1, me);
        } else if( flag == 1 ) {
                addn("ability2/"+myclass+"/"+arg, 1, me);
        } 
        return 1;
}

string get_ability_info(object ob, string arg, int flag)
{
        int lv, cost, n;
        string desc, msg, myclass;

        //lv = check_ability(ob, arg, flag);
        cost = check_ability_cost(ob, arg, flag);
        desc = get_ability_idf(ob,arg,flag);
        if( flag == 1 ) {
                // ability's 门派技能
                myclass=query("family/family_name", ob);
                if( arrayp(fam_ability[myclass]) ) {
                        n = member_array(arg,fam_ability[myclass]);
                        if( n != -1 ) {
                                msg = fam_ability_info[myclass][n];
                                msg = sprintf(msg+"\t%-12s"HIC"%5d\n"NOR,desc, cost);
                                return msg;
                        }
                }
                return "";
        }
        
        n = member_array(arg, bas_ability);
        if( n != -1 ) {
                msg = bas_ability_info[n];
                msg = sprintf(msg+"\t%-12s"HIC"%5d\n"NOR, desc, cost);
                return msg;
        }
        return "";
}


string get_ability_idf(object me,string arg, int flag)
{
        int level = max_ability[arg];
        int lev = check_ability(me,arg,flag);       
        // 1,2,3,4 Basic
        // 5,6,7 Advanced
        // 8,9: Master
        // 10: Grand Master
        string *tough_level_desc = ({
                YEL "一无所知" NOR,//0
                HIB "初窥门径" NOR,//1
                HIB "略知一二" NOR,//2
                HIB "粗通皮毛" NOR,//3
                HIB "马马虎虎" NOR,//4
                CYN "了然於胸" NOR,//5
                CYN "出类拔萃" NOR,//6
                CYN "登峰造极" NOR,//7
                HIC "一代宗师" NOR,//8
                HIC "举世无双" NOR,//9
                HIW "深不可测" NOR,//10
        });
        string desc = sprintf(HIM"%3d/%3d"NOR, lev, level);
        if( lev >= 10 ) lev = 10;
        return (tough_level_desc[lev]+"（"+ desc+"）");
}

