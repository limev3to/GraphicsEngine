#include "pch.h"
#include "Core/GameTime.h"

namespace nu {

	nu::Time::Time()
	{
		Reset();
	}

	void nu::Time::Reset()
	{

		m_startTicks = SDL_GetTicksNS();
		m_frameTicks = SDL_GetTicksNS();
		m_currentTicks = SDL_GetTicksNS();
	}

	void nu::Time::Tick()
	{
		m_currentTicks = SDL_GetTicksNS(); // get current ticks
		m_deltaTicks = m_currentTicks - m_frameTicks; // delta = current ticks - prev ticks 
		m_frameTicks = m_currentTicks;
	}


}

