
#include "nspch.h"
#include <TrashEngine/core/Clock.h>

namespace TrashEngine {

	Clock::Clock() : m_CurrentTime(Time::GetTime())
	{
	}

	void Clock::start()
	{
		this->m_CurrentTime = Time::GetTime();
	}

	Time Clock::getElapsedTime()
	{
		float t = Time::GetTime();
		return t - this->m_CurrentTime;
	}

	Time Clock::restart()
	{
		float t = Time::GetTime();
		float elapsedTime = t - this->m_CurrentTime;
		this->m_CurrentTime = t;
		return elapsedTime;
	}

}