/*
philosophers - a C++11 example of the dining philosopher's problem.
Copyright (C) 2013  Alexander Groden

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __PHILOSOPHERS_H__
#define __PHILOSOPHERS_H__

#include <vector>
#include <mutex>
#include <memory>
#include <random>

#include "runnable.h"

namespace dining_philosophers {
	/**
	 * philosopher
	 * A default philosopher uses C++ std::lock to avoid deadlocks.
	 */
	class philosopher : public boilerplate::runnable {
	public:
		philosopher(int id, unsigned int total_philosophers, 
			std::vector<std::mutex*> table, unsigned int max_eat_time, 
			unsigned int max_think_time, unsigned int iterations);
		virtual ~philosopher();
		
	protected:
		virtual void get_resources();
		virtual void release_resources();
		virtual void run();
	
	private:
		int m_id;
		unsigned int m_total_philosophers;
		std::vector<std::mutex*> m_table;
		std::uniform_int_distribution<unsigned int> m_eat_dist;
		std::uniform_int_distribution<unsigned int> m_think_dist;
		unsigned int m_iterations;
		std::mt19937 m_rng;
	};
};

#endif