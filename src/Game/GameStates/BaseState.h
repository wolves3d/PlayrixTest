#ifndef __BaseState_h_included__
#define __BaseState_h_included__


class CBaseState
{
public:
	CBaseState()
		: m_exitFlag(false)
	{}

	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual void OnUpdate(float dt) {};
	virtual void OnDraw() = 0;
	virtual void OnMouse() {};

	void ExitState() { m_exitFlag = true; }
	bool IsEnded() const { return m_exitFlag;  }

private:
	bool m_exitFlag;
};


class CStateManager
{
public:

	void Update()
	{
		while (true)
		{
			CBaseState * state = GetState();

			if (NULL == state)
				break;
		
			if (false == state->IsEnded())
				break;
			
			PopState();
		}
	}

	void PushState(CBaseState * state)
	{
		Update(); // Remove any finished states first

		CBaseState * currentState = GetState();
		if (NULL != currentState)
		{
			currentState->OnExit();
		}

		m_states.push_back(state);
		state->OnEnter();
	}

	void PopState()
	{
		CBaseState * state = GetState();

		if (NULL != state)
		{
			state->OnExit();

			const uint stateID = (m_states.size() - 1);
			m_states.erase(m_states.begin() + stateID);

			delete state;
		}

		state = GetState();
		if (NULL != state)
		{
			state->OnEnter();
		}
	}

	CBaseState * GetState()
	{
		const uint stateCount = m_states.size();
		if (stateCount > 0)
		{
			return m_states[stateCount - 1];
		}
		
		return NULL;
	}

private:
	vector <CBaseState *> m_states;
};

#endif __BaseState_h_included__