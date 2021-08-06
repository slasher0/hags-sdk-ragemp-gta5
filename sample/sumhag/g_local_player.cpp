#include "game.h"




LONG CGame::Validate_LocalPlayer()//CPed *pObj)
{
	m_Frame++;

	//m_Local.m_car = NULL;


	//if(m_ForceUpdate || !m_Local.m_Data)
	{
	}

	


#if 0
#ifdef _DEV_
	if(GetAsyncKeyState(VK_END)&1)
	{
		g_Console->Printf("ped   %p", pObj);
		g_Console->Printf("car   %p", m_Local.m_car);
	}
#endif
#endif



	//m_Local.World = &pObj->PT.Pos_View.x;






	BOOL tp = m_Local.m_TP;
	if(tp)
	{
		m_Local.m_TP = 0;

		if(tp == 3)
		{
			PPlayer pTarget = Bot.target ? FindPlayer(Bot.target) : 0;
			if(!pTarget)
			{
				goto_fail();
				return 0;
			}

			m_Local.m_TPos = pTarget->World;
		}
		else
		if(tp == 2)
		{
			//if(!TeleportBlip())
			//{
			//	goto_fail();
			//	return 0;
			//}
		}

		//Pos2world(&m_Local.m_TPos.x, &world.x);
	}


	return 0;
}


