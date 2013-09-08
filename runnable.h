/*
boilerplate - standard boilerplate classes using C++11.
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

#ifndef __BOILERPLATE_RUNNABLE_H__
#define __BOILERPLATE_RUNNABLE_H__

#include <atomic>
#include <thread>

namespace boilerplate {
	/// runnable
	/// Standard thread-as-a-class abstract base class.
	/// Overload run, use stopped in loops in run method, etc...
	/// Not copyable.
	class runnable {
	public:
		runnable() : m_stop(false), m_thread() { }
		virtual ~runnable() { join(); }
		runnable(runnable const&) = delete;
		runnable& operator=(runnable const&) = delete;
		
		void start() { m_thread = std::thread(&runnable::run, this); }
		void stop() { m_stop = true; }
		void join() { if(m_thread.joinable()) m_thread.join(); }
		bool stopped() { return m_stop; }
		
	protected:
		virtual void run() = 0;
		
	private:
		std::atomic<bool> m_stop;
		std::thread m_thread;
	};
}

#endif