#pragma once

#include "cAction.h"

class cActionRepeat
	: public cAction
	, public iActionDelegate
{
protected:
	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);

public:
	cActionRepeat(void);
	virtual ~cActionRepeat(void);

	static cAction* Create();
	
	// cAction override
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish(cAction* pSender) override;
};

