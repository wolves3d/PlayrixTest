#ifndef __GameTitle_h_included__
#define __GameTitle_h_included__


#include "BaseState.h"
#include "GetReady.h"


class CGameTitle : public CBaseState
{
public:
	
	virtual void OnDraw()
	{
		const int halfWidth = (Render::device.Width() / 2);
		const int halfHeight = (Render::device.Height() / 2);

		Render::BindFont("arial");
		Render::PrintString(halfWidth, halfHeight, "CLICK TO BEGIN", 1.f, CenterAlign);
	}

	virtual void OnMouse()
	{
		if (Core::mainInput.GetMouseLeftButton())
		{
			ExitState();

			CStateManager & mgr = g_Game->GetStateManager();
			mgr.PushState(new CGetReady());
		}
	}
};


#endif __GameTitle_h_included__