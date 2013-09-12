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

#include <thread>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "philosopher.h"

using namespace std;
using namespace dining_philosophers;

std::mutex philosopher::s_mtx;
std::mt19937 philosopher::s_rng;

philosopher::philosopher(int id, vector<mutex*> table, unsigned int max_eat, 
	unsigned int max_think, unsigned int iters) : 
	m_id(id), m_table(table), m_eat_dist(1, max_eat), 
	m_think_dist(1, max_think), m_max_iter(iters) {
	
	// seed the random number generator
	uint_least32_t seed[mt19937::state_size];
	random_device rand;
	generate_n(seed, mt19937::state_size, std::ref(rand));
	seed_seq seq(begin(seed), end(seed));
	s_rng.seed(seq);

	// assign unique locks to both chopsticks
	m_left = unique_lock<mutex>(*(m_table[m_id]), defer_lock);
	m_right = unique_lock<mutex>(*(m_table[(m_id + 1) % m_table.size()]), 
		defer_lock);
}

philosopher::~philosopher() { }

void philosopher::get_resources() {
	auto start = std::chrono::high_resolution_clock::now();
	lock(m_left, m_right);
	auto stop = std::chrono::high_resolution_clock::now();
	auto elapsed = stop - start;
	auto duration = chrono::duration<double, std::milli>(elapsed);
	std::stringstream ss;
	ss << "\t(" << m_curr_iter << ") Philosopher " << m_id << " waited " << 
		std::fixed << std::setprecision(2) << duration.count() << 
		" ms to pick up chopsticks " << m_id << " and " << (m_id + 1);
	log(ss);
}

void philosopher::release_resources() {
	m_left.unlock();
	m_right.unlock();
	std::stringstream ss;
	ss << "\t(" << m_curr_iter << ") Philosopher " << m_id << 
		" put down chopsticks " << m_id << " and " << (m_id + 1);
	log(ss);
}

void philosopher::run() {
	std::stringstream ss;
	for (m_curr_iter = 0; m_curr_iter < m_max_iter; ++m_curr_iter) {
		if (stopped()) // respect the stop command
			break;
		
		// decide the amount of time to eat and think every iteration
		chrono::milliseconds eat_time(m_eat_dist(s_rng));
		chrono::milliseconds think_time(m_think_dist(s_rng));
		
		// think
		ss << "(" << m_curr_iter << ") Philosopher " << m_id << 
			" is thinking...";
		log(ss);
		this_thread::sleep_for(think_time);
		ss << "(" << m_curr_iter << ") Philosopher " << m_id << 
			" thought for " << think_time.count() << " ms";
		log(ss);
		
		// grab some chopsticks
		get_resources();
		
		// eat
		ss << "(" << m_curr_iter << ") Philosopher " << m_id << " is eating...";
		log(ss);
		this_thread::sleep_for(eat_time);
		ss << "(" << m_curr_iter << ") Philosopher " << m_id << " ate for " << 
			eat_time.count() << " ms";
		log(ss);
		
		// put down chopsticks
		release_resources();
	}
}

void philosopher::log(std::stringstream& ss) {
	std::lock_guard<std::mutex> lock(s_mtx);
	cout << ss.str() << endl;
	ss.str("");
}