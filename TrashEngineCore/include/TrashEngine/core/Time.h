#pragma once

#include "Base.h"

namespace TrashEngine {

	class Time
	{
	public:
		// for each platform
		NS_API static Time GetTime();

		NS_API Time(float time = 0.0f) : m_Time(time) {}

		NS_API float asSecond() const { return this->m_Time; }
		NS_API float asMilliseconds() const { return this->m_Time * 1000.0f; }

		NS_API operator float() const { return this->m_Time; }

		NS_API Time& operator += (const Time& other)
		{
			this->m_Time += other.m_Time;
			return *this;
		}

	private:
		float m_Time;
	};

}
