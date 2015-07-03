#ifndef __MainGame_h_included__
#define __MainGame_h_included__


class CMainGame : public CBaseState
{
public:

	virtual void OnEnter()
	{
		g_Game->CreateBubbles();
	}

	virtual void OnUpdate(float ft)
	{
	}

	virtual void OnDraw()
	{
		const int halfWidth = (Render::device.Width() / 2);
		const int height = (Render::device.Height() - 15);

		Render::BindFont("arial");
		Render::PrintString(halfWidth, height, string("TIME: ") + utils::lexical_cast(g_Game->GetStageTime()), 1.f, CenterAlign);
	}
};


#endif __MainGame_h_included__