#include "UIManager.h"
#include "../Application.h"


namespace Beginning {
	

	void UIManager::ClearObjects()
	{
		for (auto object : m_Elements) {
			object->CleanUIObject();
			delete object;  // Free memory
		}
		m_Elements.clear();
	}

	void UIManager::UpdateObjects()
	{
		for (auto object : m_Elements) {
			//if (object->isPendingKill == false)
			{
				object->Update(1.0f / 60.0f);  // Call update for each object

			}
		}
	}


	void UIManager::RemoveObject(UIObject* object)
	{
		auto it = std::find(m_Elements.begin(), m_Elements.end(), object);
		if (it != m_Elements.end()) {
			object->CleanUIObject();
			delete* it;  // Free memory
			m_Elements.erase(it);
		}
	}

}