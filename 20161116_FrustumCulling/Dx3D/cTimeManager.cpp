#include "StdAfx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager(void)
	: m_dwFPS(0)
	, m_dwFrameCount(0)
	, m_dwPassedFPSTime(0)
{
	m_dwLastTimeStamp = GetTickCount();
}


cTimeManager::~cTimeManager(void)
{
}

void cTimeManager::Update()
{
	DWORD dwCurrTime = GetTickCount();
	m_dwDeltaTime = dwCurrTime - m_dwLastTimeStamp;
	m_dwLastTimeStamp = dwCurrTime;

	m_dwPassedFPSTime += m_dwDeltaTime;
	if(m_dwPassedFPSTime >= 1000)
	{
		m_dwPassedFPSTime -= 1000;
		m_dwFPS = m_dwFrameCount;
		m_dwFrameCount = 0;
	}

	while(m_dwPassedFPSTime >= 1000)
	{
		m_dwPassedFPSTime -= 1000;
	}

	m_dwFrameCount++;
}

float cTimeManager::GetDeltaTime()
{
	return m_dwDeltaTime / 1000.f;
}

DWORD cTimeManager::GetFPS()
{
	return m_dwFPS;
}
