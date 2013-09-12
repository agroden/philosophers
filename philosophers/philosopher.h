/*
philosophers - a C++11 example of the dining philosopher's problem.
Copyright (C) 2013  Alexander Groden

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PHILOSOPHERS_H__
#define __PHILOSOPHERS_H__

#include <string>
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
		philosopher(int id, std::vector<std::mutex*> table, 
			unsigned int max_eat, unsigned int max_think, unsigned int iters);
		virtual ~philosopher();
		
	protected:
		virtual void get_resources();
		virtual void release_resources();
		virtual void run();
		virtual void log(std::stringstream& str);
	
	private:
		int m_id;
		std::vector<std::mutex*> m_table;
		std::uniform_int_distribution<unsigned int> m_eat_dist;
		std::uniform_int_distribution<unsigned int> m_think_dist;
		const unsigned int m_max_iter;
		unsigned int m_curr_iter;
		std::unique_lock<std::mutex> m_left;
		std::unique_lock<std::mutex> m_right;
		static std::mt19937 s_rng;
		static std::mutex s_mtx;
	};
};

#endif