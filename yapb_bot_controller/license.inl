// ========================================================================
// ************ License Configuration *************************************
// ========================================================================

// IP License (No Port)
new const authorized_ips[][] = {
    "188.212.102.113", 
    "188.212.101.5",
	"93.114.82.83",
	"193.84.64.49",
	"146.70.254.108"	
};

// DNS License (Strict) 
new const authorized_dns_substrings[][] = {
    "HERO.DAEVA.RO", 
    "CS.DAEVA.RO",
	"SUPERHERO.DAEVA.RO",
	"SH.DAEVA.RO",
	"CSGO.DAEVA.RO"	
};



// ========================================================================
// ************ License Construction **************************************
// ========================================================================


static TASK_VALUE;
public TASK_LICENSE_ALWAYS()
{
	TASK_VALUE = random_num(10340, 25011);
	set_task(180.0, "TASK_LICENSE_STARTUP", TASK_VALUE, _, _, "b");
}

public TASK_LICENSE_STARTUP() 
{ 
	if (!check_server_license())
	{
		remove_task(TASK_VALUE);
	
		log_amx("This Server is not licensed. Contact DAEVA.RO to renew the license.");
		server_print("This Server is not licensed. Contact DAEVA.RO to renew the license.");
		set_fail_state("This Server is not licensed. Contact DAEVA.RO to renew the license.");
			
	} 
	
} 

stock bool:check_server_license()
{
	if ( check_server_ip() && check_server_dns() && check_server_date() )
	{
		return true;
	}

	return false;
}

stock bool:check_server_dns()
{
    new server_dns[64];
    get_cvar_string("hostname", server_dns, charsmax(server_dns));

    for (new i = 0; i < sizeof authorized_dns_substrings; i++)
    {
        if (contain(server_dns, authorized_dns_substrings[i]) != -1)
        {
            return true;
        }
    }

    return false;
}


stock bool:check_server_ip()
{
    new netaddress[64], left[64], right[64];
    get_user_ip(0, netaddress, charsmax(netaddress), 0); 
    strtok(netaddress, left, charsmax(left), right, charsmax(right), ':', 0); 

    for (new i = 0; i < sizeof(authorized_ips); i++)
    {
        if (equali(left, authorized_ips[i], 0))
        {
            return true; 
        }
    }

    return false; 
}


stock bool:check_server_date()
{
	// TIME License (Unix)
	new const TimeStamp = 1742016625; 
	new iYear, iMonth, iDay, iHour, iMinute, iSecond;
	if (get_systime() <= TimeStamp)
	{
		UnixToTime(TimeStamp, iYear, iMonth, iDay, iHour, iMinute, iSecond);
		
		log_amx("This license is valid until %d.%d.%d", iDay, iMonth, iYear);
		server_print("This license is valid until %d.%d.%d", iDay, iMonth, iYear);
		
		return true;
	}
	else
	{
		UnixToTime(TimeStamp, iYear, iMonth, iDay, iHour, iMinute, iSecond);
		
		log_amx("This license has expired at %d.%d.%d. Contact DAEVA.RO to renew the license.", iDay, iMonth, iYear);
		server_print("This license has expired at %d.%d.%d. Contact DAEVA.RO to renew the license.", iDay, iMonth, iYear);
		
		return false;
	}
	
	return false;
}
