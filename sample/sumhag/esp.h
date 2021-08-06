#ifndef __ESP_H__
#define __ESP_H__




class giW2S
{
public:
	INT X, Y;
	inline BOOL Project(const Vector &Pos) { return sdk.GI_ScreenProjectPoint(&Pos.x, &X); }
};

class niW2S
{
public:
	INT X, Y;
	inline BOOL Project(const Vector &Pos) { return sdk.NI_ScreenProjectPoint(&Pos.x, &X); }
};






class cEsp
{
public:
	SDK_ESP *Esp;

	cTexture *Beam;

	static INT		esp_bar_max_dist;
	static FLOAT	esp_dist_pickup;
	static FLOAT	esp_dist_mine;
	static FLOAT	esp_dist_nade;

public:
	void Esp_init();
	void Esp_release();

	VOID Esp_Draw1(PPlayer);
	VOID Esp_Draw2(PPlayer);
	VOID Esp_Draw();
	VOID Esp_DrawBeam(PPlayer);
};



#endif