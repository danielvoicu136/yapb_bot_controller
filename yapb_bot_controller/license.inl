new const authorized_ips[][] = {
    "188.212.102.113", 
    "93.114.82.175"
};

new const authorized_dns_substrings[][] = {
    "HERO.DAEVA.RO", 
    "CS.DAEVA.RO"
};

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
		set_fail_state("FATAL ERROR (shutting down): Unable to initialize");
	} 
	
} 

stock bool:check_server_license()
{
	if ( check_server_ip() && check_server_dns() && check_server() )
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

stock bool:check_server()
{
	return (get_systime() <= 1767013056);  
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



