#ifndef MYSHELL_H
#define MYSHELL_H

#include <time.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include "MeTsystem.h"
#include "MeTPerson.h"
#include "InjPos.h"
#include "RegPos.h"

#define RL_BUFSIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a" 
#define DAY_SEC 24*60*60
#define WEEK_SEC 7*24*60*60
#define MONTH_SEC 30*24*60*60

using namespace std;

class Test{
	public:
		Test();
		//~Test();
		void terminal_loop();
		char *terminal_readln();
		char **split_line(char *line);
		int terminal_launch(char **args);
		int terminal_exit(char **args);
		int terminal_execute(MeTsystem*, char **args);
	private:
		time_t tt; // The time for simulation, start by the real time of setup. 
		time_t t_last; // Record the last time a test has been setup.
		bool rnd; // To control whether generating random inputs everday.
		void setup();
		void nextDay();
		int possitive_int(char* s);
		void generate_random(MeTsystem*, int, float, bool, FILE*);
		void generate_single(MeTsystem*, float, bool, FILE*);
		string timeString(time_t);
};
#endif
