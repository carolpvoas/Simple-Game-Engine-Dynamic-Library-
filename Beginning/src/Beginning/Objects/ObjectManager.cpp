#include "ObjectManager.h"

namespace Beginning {

	void ObjectManager::RemoveObject(Object* object)
	{
		auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
		if (it != m_Objects.end()) {
			object->Clean();
			delete* it;  // Free memory
			m_Objects.erase(it);
		}
	}

	void ObjectManager::DestroyObject(Object* object)
	{
		for (auto m_object : m_Objects) {
			if (m_object == object && !m_object->isPendingKill)
			{
				object->isPendingKill = true;
			}
		}
	}

	void ObjectManager::ClearObjects()
	{
		for (auto object : m_Objects) {
			object->m_Components.clear();  // Clean up components
			object->m_RenderComponents.clear();
			object->m_RigidbodyComponent = nullptr;
			delete object;  // Free memory
		}
		m_Objects.clear();
	}

	void ObjectManager::UpdateObjects()
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			if (m_Objects[i]->m_canUpdate == true)
			{
				if (m_Objects[i]->isPendingKill == false)
				{
					m_Objects[i]->OnUpdate(1.0f / 60.0f);  // Call update for each object
					m_Objects[i]->UpdateComponents(1.0f / 60.0f);
				}
			}
		}

	}
}
