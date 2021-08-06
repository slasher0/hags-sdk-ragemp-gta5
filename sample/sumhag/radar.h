#ifndef _cRadar_H_
#define _cRadar_H_


class cRadar
{
public:
	void Radar_init();
	void Radar_release();

	SDK_RADAR *Radar;

public:
	VOID Radar_Draw();
};


#endif