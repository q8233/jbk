#pragma once

class cGameObject;
class cAction;

class iActionDelegate
{
public:
	virtual void OnActionFinish(cAction* pSender) = 0;
};

class cAction : public cObject
{
protected:
	float m_fPassedTime;
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(cGameObject*, m_pTarget, Target);
	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);

public:
	cAction(void);
	virtual ~cAction(void);

	virtual void Start();
	virtual void Update();

	static cAction* Create();
};

