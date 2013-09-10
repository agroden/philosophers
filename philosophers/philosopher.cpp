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
#include <iostream>

#include "philosopher.h"

using namespace std;
using namespace dining_philosophers;

philosopher::philosopher(int id, unsigned int total_philosophers, 
	vector<mutex*> table, unsigned int max_eat_time, 
	unsigned int max_think_time, unsigned int iterations) : 
	m_id(id), m_total_philosophers(total_philosophers),
	m_table(table), m_eat_dist(1, max_eat_time), 
	m_think_dist(1, max_think_time), m_iterations(iterations) {
	
	// seed the random number generator
	uint_least32_t seed[mt19937::state_size];
	random_device rand;
	generate_n(seed, mt19937::state_size, std::ref(rand));
	seed_seq seq(begin(seed), end(seed));
	m_rng.seed(seq);
}

philosopher::~philosopher() { }

void philosopher::get_resources() {
	mutex* left = m_table[m_id];
	mutex* right = m_table[(m_id + 1) % m_total_philosophers];
	lock(*left, *right);
	lock_guard<mutex> l(*left, adopt_lock);
	cout << "\tPhilosopher " << m_id << " picked up chopstick " << m_id << endl;
	lock_guard<mutex> r(*right, adopt_lock);
	cout << "\tPhilosopher " << m_id << " picked up chopstick " << (m_id + 1) 
		<< endl;
}

void philosopher::release_resources() {
	mutex* left = m_table[m_id];
	mutex* right = m_table[(m_id + 1) % m_total_philosophers];
	left->unlock();
	cout << "\tPhilosopher " << m_id << " put down chopstick " << m_id << endl;
	right->unlock();
	cout << "\tPhilosopher " << m_id << " put down chopstick " << (m_id + 1) 
		<< endl;
}

void philosopher::run() {
	for (unsigned int i = 0; i < m_iterations; ++i) {
		if (stopped()) // respect the stop command
			break;
		
		// decide the amount of time to eat and think every iteration
		chrono::milliseconds eat_time(m_eat_dist(m_rng));
		chrono::milliseconds think_time(m_think_dist(m_rng));
		
		// think
		cout << "(" << i << ") Philosopher " << m_id << " is thinking..." 
			<< endl;
		this_thread::sleep_for(think_time);
		cout << "(" << i << ") Philosopher " << m_id << " thought for " 
			<< think_time.count() << " milliseconds" << endl;
		
		// grab some chopsticks
		get_resources();
		
		// eat
		cout << "(" << i << ") Philosopher " << m_id << " is eating..." 
			<< endl;
		this_thread::sleep_for(eat_time);
		cout << "(" << i << ") Philosopher " << m_id << " ate for " << 
			eat_time.count() << " milliseconds" << endl;
		
		// put down chopsticks
		release_resources();
	}
}