#pragma once

class cObject
{
private:
	ULONG m_nRefCount;

public:
	cObject(void);
	virtual ~cObject(void);

	void AddRef();
	void Release();
	cObject* AutoRelease();
};

