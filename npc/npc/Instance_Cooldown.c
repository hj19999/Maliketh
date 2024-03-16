//===== rAthena Script ===========================================|
//= Instance @cd/@cooldown Command                               =|
//===== By: ======================================================|
//= crazyarashi                                                  =|
//===== Current Version: =========================================|
//= 1.0 Initial Version                                          =|
//================================================================|

function	script	Get_Time2Str	{
	.@Time_Left = getarg(0) - gettimetick(2);	
	.@Month = .@Time_Left / 2592000;
	.@Time_Left = .@Time_Left - (.@Month * 2592000);
	.@Days = .@Time_Left / 86400;
	.@Time_Left = .@Time_Left - (.@Days * 86400);
	.@Hours = .@Time_Left / 3600;
	.@Time_Left = .@Time_Left - (.@Hours * 3600);
	.@Minutes = .@Time_Left / 60;
	.@Time_Left = .@Time_Left - (.@Minutes * 60);
	
	.@Time$ = "";
	if( .@Month > 1 )
		.@Time$ = .@Time$ + .@Month + " Months, ";
	else if( .@Month > 0 )
		.@Time$ = .@Time$ + .@Month + " Month, ";
	else if( .@Month < 1 )
		.@Time$ = .@Time$ + "";
	
	if( .@Days > 1 )
		.@Time$ = .@Time$ + .@Days + " Days, ";
	else if( .@Days > 0 )
		.@Time$ = .@Time$ + .@Days + " Day, ";
	else if( .@Days < 1 )
		.@Time$ = .@Time$ + "";

	if( .@Hours > 1 )
		.@Time$ = .@Time$ + .@Hours + " Hours, ";
	else if( .@Hours > 0 )
		.@Time$ = .@Time$ + .@Hours + " Hour, ";

	if( .@Minutes > 1 )
		.@Time$ = .@Time$ + .@Minutes + " Minutes, ";
	else if( .@Minutes > 0 )
		.@Time$ = .@Time$ + .@Minutes + " Minute, ";

	if( .@Time_Left > 1 || .@Time_Left == 0 )
		.@Time$ = .@Time$ + .@Time_Left + " Seconds";
	else if( .@Time_Left == 1 )
		.@Time$ = .@Time$ + .@Time_Left + " Second";
	
	return .@Time$;
}

-	script	instance_setting	-1,{
	end;
	
OnInit:
	setarray .instances$,
	//= ID, "Name", Quest Cooldown ID or "Cooldown Variable", Minimum Level
	1,"Airship Assault",15050,120,
	2,"Bakonawa Lake",12278,140,
	3,"Bangungot Hospital",9223,100,
	4,"Buwaya Cave",4229,130,
	5,"Central Laboratory",12347,140,
	6,"Charleston Crisis",13185,130,
	7,"Cor Mission",16377,130,
	8,"Devil's Tower",7577,130,
	9,"Eclage Interior","ecl_interior_time",100,
	10,"Edda Somatology",16390,170,
	11,"Endless Tower",60200,50,
	12,"Faceworm Nest",12325,140,
	13,"Fall of Glastheim",12460,130,
	14,"Fall of Glastheim(H)",12461,170,
	15,"Geffen Magic Tournament","GeffenTournament",90,
	16,"Ghost Palace",1261,120,
	17,"Hazy Forest",7211,100,
	18,"Heart Hunter War Base",12375,100,
	19,"Horror Toy Factory",12331,140,
	20,"Isle of Bios",15005,160,
	21,"Last Room",11379,150,
	22,"Malangdo Culvert",12271,90,
	23,"Morse Cave",9319,160,
	24,"Nidhoggur's Nest",3135,90,
	25,"Nightmarish Jitterbug",13182,120,
	26,"Octopus Cave",4197,90,
	27,"Old Glastheim",12317,130,
	28,"Old Glastheim H",12334,160,
	29,"Orc's Memory",12059,75,
	30,"OS Mission",12455,130,
	31,"Room of Consciousness",7706,100,
	32,"Sarah and Fenrir",9336,145,
	33,"Sarah's Memory",15002,99,
	34,"Sealed Shrine",3040,75,
	35,"Sky Fortress",9419,145,
	36,"Wolfchev's Laboratory",5112,140,
	37,"Werner's Laboratory",7743,100;
	.size = getarraysize(.instances$);
	bindatcmd "cd",strnpcinfo(3)+"::OnInstanceView";
	bindatcmd "cooldown",strnpcinfo(3)+"::OnInstanceView";
	bindatcmd "instancesetting",strnpcinfo(3)+"::OnSetting";
end;
	
OnInstanceView:
	.@char_id = getcharid(0);
	explode(.@T$,cd_data$,",");
	.@size = getarraysize(.@T$);
	if(!.@size){
		mes "[ ^0000CDSetting^000000 ]";
		mes "Please use the ^0000CD@instancesetting^000000 for the initial set-up of the ^0000CD@cooldown/@cd^000000 command.";
		end;
	}
	dispbottom "|==================== Instance Cooldown ====================|",0xFFA500;
	for(.@i = 0; .@i < .@size; .@i++){
		explode(.@TT$,.@T$[.@i],":");
		.@id$ = .@TT$[0];
		.@display = atoi(.@TT$[1]);
		if(!.@display){
			.@index = inarray(.instances$,.@id$);
			if(.@index == -1){
				dispbottom "An unknown error has occurred.",0xFF3300;
				end;
			}
			.@name$ = .instances$[.@index+1];
			.@min_level = atoi(.instances$[.@index+3]);
			.@cd_id = atoi(.instances$[.@index+2]);
			if(BaseLevel < .@min_level){
				.@display$ = sprintf("| %s : Unavailable - Required Level : %d",.@name$,.@min_level);
				dispbottom .@display$,0xFF3300;
				continue;
			}
			if(.@cd_id == 0 || .@cd_id > 999999) // Unix Check
				.@cd_time = getd(.instances$[.@index+2]);
			else {
				switch(checkquest(.@cd_id,PLAYTIME)){
					case -1:
					case 2:
						.@display$ = sprintf("| %s : Available",.@name$);
						dispbottom .@display$,0x00FF55;
						continue;
						
					case 0:
					case 1:
						query_sql("SELECT `time` FROM `quest` WHERE `char_id` = '"+.@char_id+"' AND `quest_id` = '"+.@cd_id+"'",.@cd_time);
						break;
				}
			}
			if(.@cd_time > gettimetick(2)){
				.@display$ = sprintf("| %s : Cooldown : " + Get_Time2Str(.@cd_time),.@name$);
				dispbottom .@display$,0xFF6347;
				continue;
			}
		}
	}
	dispbottom "|===========================================================|",0xFFA500;
end;

OnSetting:
	function instance_cd_setting;
	instance_cd_setting();
end;
	
	
function	instance_cd_setting	{
	explode(.@T$,cd_data$,",");
	.@menu$ = "Quit:";
	for(.@i = 0; .@i < .size; .@i += 4){
		explode(.@TT$,.@T$[.@index],":");
		.@id = atoi(.@TT$[0]);
		.@display = atoi(.@TT$[1]);
		if(!.@display){
			.@menu$ += "^00CC00" +.instances$[.@i+1] + "^000000:";
			.@menu[.@index] = 0;
		} else {
			.@menu$ += "^CC0000" +.instances$[.@i+1] + "^000000:";
			.@menu[.@index] = 1;
		}
		.@name$[.@index] = .instances$[.@i+1];
		.@TT$[0] = atoi(.instances$[.@i]);
		.@TT$[1] = .@menu[.@index];
		.@T$[.@index] = implode(.@TT$,":");
		cd_data$ = implode(.@T$,",");
		.@index++;
	}
	mes "[ ^0000CDSetting^000000 ]";
	mes "Select an instance to modify.";
	mes "^00CC00Green^000000 colored names are currently being displayed in the ^0000CD@cooldown^000000 command.";
	next;
	.@s = select(.@menu$) - 1;
	switch(.@s){
		case 0:
			end;
			
		default:
			.@s -= 1;
			mes "[ ^0000CDSetting^000000 ]";
			mes "Name : ^0000CD" + .@name$[.@s] + "^000000";
			mes "Status : " + ((.@menu[.@s] > 0)?"^CC0000Hidden^000000":"^00CC00Visible^000000");
			next;
			if(select(((.@menu[.@s] > 0)?"Add Instance":"Remove Instance"),"Return to list") == 2)
				instance_cd_setting();
			mes "[ ^0000CDSetting^000000 ]";
			mes "The instance ^0000CD" + .@name$[.@s] + "^000000 has been " + ((.@menu[.@s] > 0)?"added":"removed") + " from the cooldown display.";
			if(.@menu[.@s] == 0)
				.@menu[.@s] = true;
			else
				.@menu[.@s] = false;
			explode(.@TT$,.@T$[.@s],":");
			.@TT$[1] = .@menu[.@s];
			.@T$[.@s] = implode(.@TT$,":");
			cd_data$ = implode(.@T$,",");
			next;
			instance_cd_setting();
	}
	end;
}

}