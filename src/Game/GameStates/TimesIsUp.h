#ifndef __TimeIsUp_h_included__
#define __TimeIsUp_h_included__


class CTimeIsUp : public CBaseState
{
public:

	CTimeIsUp()
		: m_time(0)
	{}

	virtual void OnEnter()
	{
		m_exitTime = (m_time + 1.0f);
		AudioWrapper::Play("TimeIsUp");
	}

	virtual void OnDraw()
	{
		const int halfWidth = (Render::device.Width() / 2);
		const int halfHeight = (Render::device.Height() / 2);

		Render::BindFont("arial");
		Render::PrintString(halfWidth, halfHeight, "TIME IS UP!", 1.f, CenterAlign);
	}

	virtual void OnUpdate(float dt)
	{
		m_time += dt;

		if (m_time > m_exitTime)
		{
			ExitState();
			g_Game->GetStateManager().PushState(new CGameTitle());
		}
	}

	float m_time;
	float m_exitTime;
};


#endif __TimeIsUp_h_included__