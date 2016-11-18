#pragma once

#define g_pObjectPool cObjectPool::GetInstance()

class cObjectPool
{
private:
	std::set<cObject*> m_setObject;

	SINGLETONE(cObjectPool);
public:
	void AddObject(cObject* pObject)
	{
		m_setObject.insert(pObject);
	}
	void RemoveObject(cObject* pObject)
	{
		m_setObject.erase(pObject);
	}
	void Destroy()
	{
		assert(m_setObject.empty() && "cObject를 상속받은 객체 중 삭제되지 않은 객체가 있습니다.");
	}
};

