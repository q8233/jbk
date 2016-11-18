#pragma once

#include "cAction.h"

class cActionSeq
	: public cAction
	, public iActionDelegate
{
protected:
	std::vector<cAction*> m_vecAction;
	int m_nCurrentActionIndex;

public:
	cActionSeq(void);
	virtual ~cActionSeq(void);
	
	virtual void AddAction(cAction* pAction);
	static cAction* Create();

	// cAction override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish(cAction* pSender) override;
};

