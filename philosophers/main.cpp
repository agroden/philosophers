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

#include <stdio.h>
#include <string.h>
#include <mutex>
#include <list>
#include <string>
#include <iostream>

#include "philosopher.h"

using namespace std;
using namespace dining_philosophers;

const unsigned int PHILOSOPHERS = 5;
const unsigned int ITERATIONS = 5;
const unsigned int MAX_THINK_TIME = 5000;
const unsigned int MAX_EAT_TIME = 5000;

void print_usage() {
	cout << "dining_philosophers [OPTIONS]" << endl;
	cout << "Options:" << endl;
	cout << "-h, --help:\t\tprint this message" << endl;
	cout << "-p, --philosophers:\tthe number of philosophers (default: 5)" 
		<< endl;
	cout << "-i, --iterations:\tthe number of times each philosopher will \
		think and eat (default: 5)" << endl;
	cout << "-t, --think:\t\tthe max time each philosopher will spend thinking \
		in seconds (default: 5 seconds)" << endl;
	cout << "-e, --eat:\t\tthe max time each philosopher will spend eating \
		in seconds (default: 5 seconds)" << endl;
}

int main(int argc, const char* argv[]) {
	unsigned int num_philosophers = PHILOSOPHERS;
	unsigned int iterations = ITERATIONS;
	unsigned int think_time = MAX_THINK_TIME;
	unsigned int eat_time = MAX_EAT_TIME;
	
	for (int i = 0; i < argc; ++i) {
		if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0) 
			print_usage();
			
		else if (strcmp("-p", argv[i]) == 0 || 
			strcmp("--philosophers", argv[i]) == 0) {
			num_philosophers = stoi(argv[i + 1]);
			++i;
			
		} else if (strcmp("-i", argv[i]) == 0 || 
			strcmp("-iterations", argv[i]) == 0) {
			iterations = stoi(argv[i + 1]);
			++i;
			
		} else if (strcmp("-t", argv[i]) == 0 || 
			strcmp("--think", argv[i]) == 0) {
			think_time = stoi(argv[i + 1]) * 1000;
			++i;
			
		} else if (strcmp("-e", argv[i]) == 0 || 
			strcmp("--eat", argv[i]) == 0) {
			eat_time = stoi(argv[i + 1]) * 1000;
			++i;
		}
	}

	// make one chopstick mutex for each philosopher
	vector<mutex*> table;
	for (unsigned int i = 0; i < num_philosophers; ++i) {
		mutex* c = new mutex();
		table.push_back(c);
	}
	
	// create the philosophers and get them started
	list<philosopher*> philosophers;
	for (unsigned int i = 0; i < num_philosophers; ++i) {
		philosophers.push_back(new philosopher(i, table, eat_time, think_time, 
			iterations));
		philosophers.back()->start();
	}
	
	// wait for all the philosophers to stop thinking and eating
	for (auto iter = philosophers.begin(); iter != philosophers.end(); ++iter) {
		(*iter)->join();
	}
	
	return 0;
}