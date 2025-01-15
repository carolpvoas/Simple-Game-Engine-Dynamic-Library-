
#include "Input.h"

namespace Beginning
{
	bool Input::KeyPressed(SDL_Scancode key)
	{
		if (m_CurrentPlayer != nullptr)
		{
			if (!m_CurrentPlayer->isPendingKill)
			{
				const Uint8* state = SDL_GetKeyboardState(NULL);
				return state[key] == 1;
			}
		}
		
		return false;
	}
	void Input::BindPlayer(Object* object)
	{
		m_CurrentPlayer = object;
	}
}

