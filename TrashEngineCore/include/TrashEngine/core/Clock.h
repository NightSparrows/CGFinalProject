#pragma once

#include "Time.h"

namespace TrashEngine {

	class NS_API Clock
	{
	public:
		Clock();

		// start the time recording
		void start();

		/// Getting the elapsed time not resetting the last time.
		Time getElapsedTime();

		/// \return
		/// 	the elapsed time
		Time restart();

	private:
		float m_CurrentTime;
	};

}
