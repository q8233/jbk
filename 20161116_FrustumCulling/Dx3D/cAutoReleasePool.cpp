#include "StdAfx.h"
#include "cAutoReleasePool.h"


cAutoReleasePool::cAutoReleasePool(void)
{
}


cAutoReleasePool::~cAutoReleasePool(void)
{
}

void cAutoReleasePool::AddObject( cObject* pObject )
{
	m_setObjectPool.insert(pObject);
}

void cAutoReleasePool::Drain()
{
	for each(auto p in m_setObjectPool)
	{
		SAFE_RELEASE(p);
	}
	m_setObjectPool.clear();
}
