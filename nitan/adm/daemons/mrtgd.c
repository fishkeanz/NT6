// This program is a part of NT mudlib

#include <mudlib.h>

#define MUD_IP          "113.31.21.157"
#define LIBRARY_PATH    "/home/mud/nitan"
#define DATA_PATH      	"/data/daemon/mrtg.o"

//MRTG �趨��(����������ִ�д��趨��)
#define MRTGCONF 	"/data/daemon/mrtg/mrtg.conf"

mapping mrtglist = allocate_mapping(0);

 
/*
#  to get bits instead of bytes and graphs growing to the right
# Options[_]: growright, bits
IconDir: /mrtg/icons/
Language: gb2312
WorkDir: /var/www/html/mrtg/

#########################################################################
# Memory
#
#########################################################################


Directory[memory]: memory

Options[memory]: gauge, integer, noinfo
Target[memory]:  `vmstat | awk '{printf("%d",$4*1024)}';echo "";vmstat | awk '{printf("%d",$5*1024)}'`
#MaxBytes[memory]: 134217728
MaxBytes[memory]: 671023104
YLegend[memory]: memory usage(kb)

ShortLegend[memory]: &nbsp;&nbsp;
Unscaled[memory]: ymwd
LegendI[memory]: &nbsp; Memory Actv: &nbsp;
LegendO[memory]: &nbsp; Memory Free: &nbsp;
Legend1[memory]: Memory  ״̬&nbsp&nbsp---&nbsp&nbsp��ʽʹ����
Legend2[memory]: Memory  ״̬&nbsp&nbsp---&nbsp&nbsp��δʹ��
Title[memory]: Lonely' server Memory USAGE Monitor
PageTop[memory]: <H1>���� Memory ʹ���ʼ��</H1>
 */
 
//
// ���� MRTG �趨��(����������ִ�д��趨��)
//
int generate_mrtg_conf()
{
	string finaloutput="";
	
	finaloutput+="IconDir: /icons/\n";
        finaloutput+="Language: gb2312\n";
        finaloutput+="WorkDir: "LIBRARY_PATH"/www/muds\n";
	
	if( !sizeof(mrtglist) ) return 0 ;
	
	foreach(string name, mapping data in mrtglist)
	{
		string output="\n\n#########################################################################\n";
		output+= "#" + name + "\n";
		output+= "#########################################################################\n";
		
		name=replace_string(name," ","_");
		
		// ��׼��ʽ
		output+= "Directory[" + name +"]: "+ data["Directory"] + "\n";
		
		if( undefinedp(data["Options"]) )
			output+= "Options["+name+"]: gauge, integer, noinfo, nopercent\n";
		else 	output+= "Options[" + name +"]: "+ data["Options"] + "\n";
		
		output+= "Target[" + name +"]: "+ data["Target"] + "\n";
		
		if( !undefinedp(data["MaxBytes"]) )
			output+= "MaxBytes[" + name +"]: "+ data["MaxBytes"] + "\n";
		else 	output+= "MaxBytes[" + name +"]: 100\n";
		
		if( !undefinedp(data["AbsMax"]) )
			output+= "AbsMax[" + name +"]: "+ data["AbsMax"] + "\n";
		else 	output+= "AbsMax[" + name +"]: 1500\n";
			
		output+= "YLegend[" + name +"]: "+ data["YLegend"] + "\n";
		output+= "LegendI[" + name +"]: "+ data["LegendI"] + "\n";
		output+= "LegendO[" + name +"]: "+ data["LegendO"] + "\n";
		output+= "Legend1[" + name +"]: "+ data["Legend1"] + "\n";
		output+= "Legend2[" + name +"]: "+ data["Legend2"] + "\n";
		
		/*output+= "YLegend[" + name +"]: "+ data["YLegend"] + "\n";
		
		if( !undefinedp(data["ShortLegend"]) )
			output+= "ShortLegend[" + name +"]: "+ data["ShortLegend"] + "\n";
		*/	
		if( !undefinedp(data["SetEnv"]) )
			output+= "SetEnv[" + name +"]: "+data["SetEnv"]+ "\n";
			
		if( !undefinedp(data["ShortLegend"]) )
			output+= "ShortLegend[" + name +"]: "+ data["ShortLegend"] + "\n";
			
		if( !undefinedp(data["Unscaled"]) )
			output+= "Unscaled[" + name +"]: "+ data["Unscaled"] + "\n";
		else 	output+= "Unscaled[" + name +"]: ymwd\n";
		
		output += "Title[" + name +"]: "+ data["Title"] + "\n";
		output += "PageTop[" + name +"]: "+ data["PageTop"] + "\n\n";
		
		finaloutput += output;
	}

	write_file(MRTGCONF, finaloutput, 1);
}

 
int save()
{
        return save_object(DATA_PATH);  
}

// ���� MRTG ����
int addmrtg(string name, mapping info)
{
	if( !undefinedp(info) )
	{
		if( undefinedp(info["Directory"]))
			return 0;
		if( undefinedp(info["Target"]))
			return 0;
		if( undefinedp(info["YLegend"]))
			return 0;
		if( undefinedp(info["LegendI"]))
			return 0;
		if( undefinedp(info["LegendO"]))
			return 0;
		if( undefinedp(info["Legend1"]))
			return 0;
		if( undefinedp(info["Legend2"]))
			return 0;
		if( undefinedp(info["Title"]))
			return 0;
		if( undefinedp(info["PageTop"]))
			return 0;
				
	} else return 0;

	mrtglist[name] = info;
	
	generate_mrtg_conf();
	
	save();

	return 1;
}

// �Ƴ� MRTG
int removemrtg(string name)
{
        if( !mapp(mrtglist[name]) ) 
        	return 0;
	
	map_delete(mrtglist, name);

	generate_mrtg_conf();

	save();
	
	return 1;
} 

//
// �Ƴ�Ԥ��� MRTG
//
void remove_default_mrtg()
{
	removemrtg("0mudos_cpumem");
	removemrtg("0mudos_mem");
	removemrtg("0system_mem");
	removemrtg("0system_cpu");
	removemrtg("0system_netflow");
}

//
// ����Ԥ��� MRTG
//
void add_default_mrtg()
{
	// Mud Memory
	addmrtg("0system_netflow",([
		"Directory":	"default/0system_netflow",
		"Options":	" ",
		"Target":	"1:public@"MUD_IP":",
		"MaxBytes":	"125000",
		"AbsMax":	"1250000",
		"LegendI": 	"&nbsp; netflow in &nbsp;",
		"LegendO":	"&nbsp; netflow out &nbsp;",
		"Legend1":	"��������&nbsp&nbsp---&nbsp&nbsp",
		"Legend2":	"��������&nbsp&nbsp---&nbsp&nbsp",
                "Title":        "NT �������",
		"PageTop":	"<H1>NT �������</H1>",
                "YLegend":      "NT netflow monitor",
		"SetEnv":	"MRTG_INT_IP=\""MUD_IP"\" MRTG_INT_DESCR=\"xl0\"",
		//"ShortLegend": 	" %",
	]));
        /*
	// Mud Memory
	addmrtg("0mudos_cpumem",([
		"Directory":	"default/0mudos_cpumem",
		"Options":	"noinfo,gauge,integer",
                "Target":       "`ps -uU mud | grep config.xajh | grep driver | awk '{printf(\"%f\\n%f\\n\",$3*1,$4*1);}'`",
		"MaxBytes":	"100",
		"AbsMax":	"5000",
		"LegendI": 	"&nbsp; CPU load &nbsp;",
		"LegendO":	"&nbsp; MEM load &nbsp;",
		"Legend1":	"CPU ״̬&nbsp&nbsp---&nbsp&nbsp",
		"Legend2":	"MEM ״̬&nbsp&nbsp---&nbsp&nbsp",
                "Title":        "NT MUD ����",
		"PageTop":	"<H1>NT MUD ���� %</H1>",
		"YLegend":	"MudOS CPU/MEM load percentage graph",
		"ShortLegend": 	" %",
	]));
	// Mud MEM USAGE
	addmrtg("0mudos_mem",([
		"Directory":	"default/0mudos_mem",
		"Options":	"gauge,noinfo,integer",
                "Target":       "`ps -uU mud | grep config.xajh | grep driver | awk '{printf(\"%d\\n%d\\n\",$6*1024,($5+$6)*1024)}'`",
		"MaxBytes":	"100000000",
		"AbsMax":	"1073676288",
		"LegendI": 	"&nbsp; Data Usage: &nbsp;",
		"LegendO":	"&nbsp; Total Usage:  &nbsp;",
		"Legend1":	"Memory ״̬&nbsp&nbsp---&nbsp&nbspһ����ƽ��ֵ",
		"Legend2":	"Memory ״̬&nbsp&nbsp---&nbsp&nbsp�����ƽ��ֵ",
                "Title":        "NT Mud Memory USAGE Monitor",
		"PageTop":	"<H1>Memory Monitor For Mudos (NT) </H1>",
		"YLegend":	"MudOS MEM(kb)",
		"ShortLegend": 	"&nbsp;&nbsp;",
		
	]));
        */
	// MEMORY USAGE
	addmrtg("0system_mem",([
		"Directory":	"default/0system_mem",
		"Options":	"gauge, integer, noinfo",
		//"Target":	"`vmstat | awk '{printf(\"%d\",$4*1024)}';echo \"\";vmstat | awk '{printf(\"%d\",1073676288-($5*1024))}'`",
		"Target":	"`vmstat | awk '{count++; if(count == 3) printf(\"%d\\n%d\\n\",$4*1024,($5*1024) )}'`",
		"MaxBytes":	"1073676288",
                "AbsMax":       "2073676288",
		"LegendI": 	"&nbsp; Memory Actv: &nbsp;",
		"LegendO":	"&nbsp; Memory Free: &nbsp;",
		"Legend1":	"Memory ״̬&nbsp&nbsp---&nbsp&nbspһ����ƽ��ֵ",
		"Legend2":	"Memory ״̬&nbsp&nbsp---&nbsp&nbsp�����ƽ��ֵ",
		"Title":	"NT server Memory USAGE Monitor",
		"PageTop":	"<H1>.. Memory .....</H1>",
		"YLegend":	"memory usage(kb)",
		"ShortLegend": 	"&nbsp;&nbsp;",
	]));
	// CPU USAGE
	addmrtg("0system_cpu",([
		"Directory":	"default/0system_cpu",
		"Options":	"noinfo,gauge,nopercent,integer",
		"Target":	"`uptime | awk '{print $(NF-2)*100\"\\n\",$(NF-1)*100\"\\n\",$NF*100}'`",
		"MaxBytes":	"100",
		"AbsMax":	"5000",
		"LegendI": 	"&nbsp; CPU load &nbsp;",
		"LegendO":	"&nbsp; CPU load &nbsp;",
		"Legend1":	"CPU ״̬&nbsp&nbsp---&nbsp&nbspһ����ƽ��ֵ",
		"Legend2":	"CPU ״̬&nbsp&nbsp---&nbsp&nbsp�����ƽ��ֵ",
		"Title":	"���� CPU ����",
		"PageTop":	"<H1>���� CPU ���� (%)</H1>",
		"YLegend":	"CPU Utilization",
		"ShortLegend": 	" %",
		
	]));
}

//
// �Ƴ�
//
int remove()
{       
        return save();
}

void create()
{
        if( !restore_object(DATA_PATH) )
	{
		// ����Ԥ����Ŀ
		add_default_mrtg();
		
		// ȡ�� MNLMUDLIST_D Ŀǰ���� 
                MUDLIST_D->update_mrtg_data();
		
		// �����趨��
		generate_mrtg_conf();

                save();       
	}
}
string query_name()
{
	return "MRTG ϵͳ(MRTG_D)";
}