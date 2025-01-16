new const authorized_ips[][] = {
    "188.212.102.113", 
    "188.212.101.5"   
};

new const authorized_dns_substrings[][] = {
    "HERO.DAEVA.RO", 
    "CS.DAEVA.RO"   
};

static TASK_VALUE;

public TASK_LICENSE()
{
	TASK_VALUE = random_num(10340, 25011);
	set_task(10.0, "TASK_CHECK_LICENSE", TASK_VALUE, _, _, "b");
}


public TASK_CHECK_LICENSE() { 
	if (!check_server_license())
	{
			remove_task(TASK_VALUE);
			server_print("FATAL ERROR - Server Shutdown - Server Files Invalid");
			set_fail_state("Server Shutdown - Server Files Invalid");
			
	} 
} 

stock bool:check_server_license()
{
    if (check_server_ip())
    {
        return true;
    }
	
    if (check_server_dns())
    {
        return true;
    }
    return false;
}

stock bool:check_server_ip()
{
    new server_ip[32];
    get_cvar_string("net_address", server_ip, charsmax(server_ip));

    for (new i = 0; i < sizeof authorized_ips; i++)
    {
        if (equal(server_ip, authorized_ips[i]))
        {
            return true;
        }
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