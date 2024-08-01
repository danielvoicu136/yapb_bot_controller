public TASK_CheckCamping() {

	if(get_cvar_float("mp_timelimit") && !get_timeleft() || ChangeLevel)
		return;
	
	new PrevMeter;
	new Players[32], Num;

	get_players(Players, Num);
	for(new index = 0; index < Num; ++index) 
	{
		if(is_user_alive(Players[index]) && is_user_connected(Players[index])) 
		{

			new id = Players[index];
		
			if(fm_get_user_maxspeed(id) > 2.0) 
			{
					coords_insert(id, 0);
					CheckCampingTime[id] ++
					
					if(CheckCampingTime[id] > 2) 
					{
						StandardDeviation[id] = coords_standard_deviation(id);
						CheckCampingTime[id] = 0
					}
					
					PrevMeter = Meter[id];
					
					Meter[id] += (100 - StandardDeviation[id]) / 60;
					
					Meter[id] = clamp(Meter[id], 0, 100);
					
					if(Meter[id] < PrevMeter && Meter[id] < 80)
						Meter[id] -= (PrevMeter - Meter[id]) / 3;
	
			}
			
			if(Meter[id] >= 100) 
			{
				g_bPlayerInactive[id] = true;
			}
			else 
			{ 
				g_bPlayerInactive[id] = false;
			} 
						
			new r = 0, g = 0, b = 0;	
	
			
			if(Meter[id] > 90) {
				r = 200;
				g = 100;
				b = 0;
			}
			else if(Meter[id] > 80) {
				r = 255; 
				g = 100;
				b = 0;
			}
			else if(Meter[id] > 60) {
				r = 255;
				g = 255;
				b = 0;
			}
			else if(Meter[id] > 50) {
				r = 0;
				g = 255; 
				b = 0;
			}
			else {
				r = 0;
				g = 255;
				b = 0;
			}
				
			new Message[256];
			
			if(Meter[id] >= 99) 
			{
					formatex(Message,sizeof(Message)-1,"| Spectators can replace you |");
				
					 set_hudmessage(r, g, b, -1.0, 0.75, 0, 0.1, 1.0, 0.1, 0.1, -1)
					 ShowSyncHudMsg(id, g_SyncObj, Message);
			} 
			else if(Meter[id] > 0) 
			{ 
					formatex(Message,sizeof(Message)-1,"| inactivity: %d |", Meter[id]);
					
					set_hudmessage(r, g, b, -1.0, 0.75, 0, 0.1, 1.0, 0.1, 0.1, -1)
					ShowSyncHudMsg(id, g_SyncObj, Message);
			}
			
		}

		else if(!is_user_alive(Players[index]) && is_user_connected(Players[index])) 
		{
				
			new Spectator = Players[index];

			new iTarget = entity_get_int( Spectator, EV_INT_iuser2 );
			
			
			if ( SHARED_ValidPlayer( iTarget ) && iTarget != Spectator )
			{
			
				new Message[256];
				if(is_user_bot(iTarget) && get_user_team(iTarget) == get_user_team(Spectator) ) 
				{
					formatex(Message,sizeof(Message)-1,"| Press R to replace this LAG player |");
				}
				else if(g_bPlayerInactive[iTarget] && get_user_team(iTarget) == get_user_team(Spectator)) 
				{ 
					formatex(Message,sizeof(Message)-1,"| Press R to replace this AFK player |");
				}
				else if(Meter[iTarget] > 0) 
				{
					formatex(Message,sizeof(Message)-1,"| inactivity: %d |", Meter[iTarget]);
				}
				else 
				{
					formatex(Message,sizeof(Message)-1,"%s",ANNOUNCE_IN_HUD);
				}
			
				set_hudmessage(0, 255, 0, -1.0, 0.75, 0, 0.1, 1.0, 0.1, 0.1, -1)
				ShowSyncHudMsg(Spectator, g_SyncObj, Message);

			}
		
			else
			{ 
				new Message[256];
				formatex(Message,sizeof(Message)-1,"| Good Luck and Have Fun |");
				set_hudmessage(0, 255, 0, -1.0, 0.75, 0, 0.1, 1.0, 0.1, 0.1, -1)
				ShowSyncHudMsg(Spectator, g_SyncObj, Message);
			}
			
		} 		
	}
}

stock coords_standard_deviation(id) {
	new Sum, Avg, Variance, VarianceTot;
	new CoordID, VectorID;
	
	for(CoordID = 0; CoordID < 3; ++CoordID) {
		Sum = 0;
		Variance = 0;
		
		for(VectorID = 0; VectorID < 4; ++VectorID)
			Sum += CoordsBody[id][VectorID][CoordID];
		
		Avg = Sum / 4;
		
		for(VectorID = 0; VectorID < 4; ++VectorID)
			Variance += power(CoordsBody[id][VectorID][CoordID] - Avg, 2);
		
		Variance = Variance /(4- 1);
		
		VarianceTot += Variance;
	}
	
	return sqroot(VarianceTot);
}

stock coords_insert(id, CoordType) {
	for(new VectorID = 4 - 1; VectorID > 0;--VectorID) {	
		for(new CoordID = 0; CoordID < 3; ++CoordID) {
			if(CoordType == 0)
				CoordsBody[id][VectorID][CoordID] = CoordsBody[id][VectorID - 1][CoordID];
			else
				CoordsEyes[id][VectorID][CoordID] = CoordsEyes[id][VectorID - 1][CoordID];
		}
	}
	
	if(is_user_connected(id)) {
		if(CoordType == 0)
			get_user_origin(id, CoordsBody[id][0], 0);
		else
			get_user_origin(id, CoordsEyes[id][0], 3);
	}
}

bool:SHARED_ValidPlayer( id )
{
	if ( id < 1 || id > MAX_PLAYERS )
	{
		return false;
	}

	return true;
}