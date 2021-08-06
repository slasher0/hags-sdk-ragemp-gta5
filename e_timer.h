#pragma once


class cTimer
{
public:
	ULONG _End;

	__forceinline static ULONG sysTickCount()
	{
//#if defined(_GUITEST_)
//		return GetTickCount();
//#endif

#if defined(_BASEDLL) || defined(_VMPROT_)
		extern ULONG g_TickCount;
		return g_TickCount;
#else
		return g_Interface->m_TickCount;
#endif
	}

	inline bool Check() const
	{
		if(!_End)
			return 0;
		ULONG Tick = sysTickCount();
		return _End > Tick;
	}

	inline VOID End()
	{
		_End = 0;
	}

	inline bool EndCheck() // null timer if finished
	{
		if(_End && _End <= sysTickCount())
		{
			_End = 0;
			return 1;
		}
		return 0;
	}

	inline VOID ms(ULONG t, ULONG Tick = sysTickCount() )
	{
		_End = Tick + t;
	}

	inline bool Check_Restart(ULONG t, ULONG Tick = sysTickCount() ) 
	{
		if(_End > Tick)
			return 1;
		_End = Tick + t;
		return 0;
	}

	inline VOID Start(ULONG t)
	{
		ms(t);
	}

};

