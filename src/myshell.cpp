#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <sys/types.h> // For wait(), waitpid().
#include <sys/wait.h>
#include "myshell.h"
#include "MeTsystem.h"
#include "MeTPerson.h"
#include "InjPos.h"
#include "RegPos.h"

#include <ctime>

using namespace std;

Test::Test(){
	tt = 0;
	t_last = 0;
	rnd = true;
}

void Test::terminal_loop(){
	char *line;
	char **args;
    int status = 1;

	/* Initialization */

	MeTsystem* mySys = new MeTsystem(); 
	/* Main loop of the command line. */
	do{
		char line2[1024];
		printf("CS225_CA1:> ");
		line = terminal_readln();
		strcpy(line2, line);
		args = split_line(line);

		status = terminal_execute(mySys, args);

		free(line);
		free(args);
	} while (status);
}

char *Test::terminal_readln(){
	int bufsize = RL_BUFSIZE;
	//int position = 0;
	char *buffer = new char[bufsize];

	if (!buffer){
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}

	if (fgets(buffer, bufsize, stdin)){
		char *c;
		c = strchr(buffer, '\n');
		if (c) *c = 0;
		else while(getchar() != EOF && getchar() != '\n');
	}

	return buffer;
}

char **Test::split_line(char *line)
{
	int bufsize = TOK_BUFSIZE, position = 0;
	char **tokens = new char*[bufsize];
  	char *token;

  	if (!tokens) {
   		fprintf(stderr, "Allocation error\n");
    	exit(EXIT_FAILURE);
  	}

  	token = strtok(line, TOK_DELIM);
  	while (token != NULL) {
    	tokens[position] = token;
    	position++;

    	if (position >= bufsize) {
      		bufsize += TOK_BUFSIZE;
      		tokens = (char **)realloc(tokens, bufsize * sizeof(char*));
      		if (!tokens) {
        		fprintf(stderr, "Allocation error\n");
        		exit(EXIT_FAILURE);
      		}
    	}

   		token = strtok(NULL, TOK_DELIM);
  	}
  
	tokens[position] = NULL;
  	return tokens;
}

int Test::terminal_launch(char **args)
{
	pid_t pid;
	int status;

  	pid = fork();
  	if (pid == 0) {
    	// Child process
    	if (execvp(args[0], args) == -1) {
      		perror("error");
    	}
    	exit(EXIT_FAILURE);
  	} else if (pid < 0) {
    	// Error forking
    	perror("error");
  	} else {
    
		// Parent process
    	do {
      		waitpid(pid, &status, WUNTRACED);
    	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

int Test::possitive_int(char* s){ // Convert possitive argument into integer.
	int n = 0;
	while (s != NULL && *s != '\0'){
		if (*s < '0' || *s > '9')
			return -1;
		else n = n * 10 + *s - '0';
		s++;
	}

	return n;
}

int Test::terminal_execute(MeTsystem* mySys, char **args)
{
	if (args[0] == NULL) {
		return 1;
	}

	if (strcmp(args[0], "exit") == 0 && args[1] == NULL)
		return 0;

	if (strcmp(args[0], "setup") == 0 && args[1] == NULL) {
		if (t_last == 0)
			setup();	
		else {
			char s[100];
			printf("There's a setup at %s, type \'yes\' if you want to replace it with a new test: ", ctime(&t_last));
			fgets(s, 100, stdin);
			if (strcmp(s, "yes\n") == 0)
				setup();
		}

		printf("Test time setup at %s", ctime(&tt));
		return 1;
	}

	if (strcmp(args[0], "pass") == 0 || strcmp(args[0], "p") == 0) { // After passing, a few rounds of random tests will be done.
		
		int unit;
		
		/* Following are some parsing process. */
		if (args[2] == NULL || args[3] != NULL){
			printf("Error: invalid input syntax for pass command.\n");
			return 1;
		}
		
		int n; // n is the number of the time units. 
		int day = 0; // dayis the actual days to be passed.  
		n = possitive_int(args[1]);
		if (n < 0){
			printf("Error: invalid input syntax for pass command.\n");
			return 1;
		}
		
		if (strcmp(args[2], "d") == 0)
			unit = 0;
		else if (strcmp(args[2], "w") == 0)
			unit = 1;
		else if (strcmp(args[2], "m") == 0)
			unit = 2;
		else {
			printf("Error: invalid input syntax for pass command.\n");
			return 1;
		}

		/* Parsing finished, with unit representing the time unit, and n contains the number. */

		time_t count = tt;
		struct tm* time_info = localtime(&count);
		int m = time_info->tm_mon; // Record month to check for difference.

		switch (unit){
			case 0:
				day = n;
				break;
			case 1:
				while(n > 0){
					day++;
					count += DAY_SEC;
					time_info = localtime(&count);
					if (time_info->tm_wday == 0) n--;
				}
				//printf("%d\n", day);
				day++;
				break;
			case 2:
				while(n > 0){
					day++;
					count += DAY_SEC;
					time_info = localtime(&count);
					if (time_info->tm_mon != m) {
						m = time_info->tm_mon;
						n--;
					}
				}
				//day--;
				//printf("%d\n", day);
				break;
			default: 
				return 1; // This shall never happen.
		}

		/* Now day contains the number of days to be passed. */
		FILE * pfile;
		pfile = fopen("generation.txt", "a");
		time_info = localtime(&tt);
		for (int i = 0; i < day; i++){ //PASS
			time_info = localtime(&tt);
			if (rnd) generate_random(mySys, 100, 0.95, true, pfile);
			nextDay(); // perform the calculation today. 

			for (int j = 0; j < mySys->numReg; j++){ // Here forwarding information is performed once a day, but you can do it twice if you'd like to.
				mySys->dbReg[j + 1]->forwardInf(mySys->FH, mySys->dbPerson);
			}

			if (mySys->totass != 0){ // Generate some total withdraw.
				for (int j = 1; j <= mySys->totass; j++){
					if (rand()%20 == 0){
						mySys->dbPerson[mySys->assid[j]]->withdraw(mySys->FH);
						printf("pid %d didn't come!\n", mySys->assid[j]);
					} 
				}
			}

			if (tt != t_last){
				mySys->dailyRep_MeT(timeString(tt));
				if (time_info->tm_wday == 0){ mySys->weeklyRep_MeT(timeString(tt));}
				if (time_info->tm_mday == 1){ mySys->monthlyRep_MeT(timeString(tt));}
			}
		}

		fclose(pfile);
		return 1;
	}
	
	if (strcmp(args[0], "disrand") == 0 && args[1] == NULL) {
		rnd = false;
		printf("Random inputs disabled.\n");
		return 1;
	}

	if (strcmp(args[0], "enrand") == 0 && args[1] == NULL){
		rnd = true;
		printf("Random inputs enabled.\n");
		return 1;
	}

	if ((strcmp(args[0], "when") == 0 || strcmp(args[0], "w") == 0) && args[1] == NULL){
		printf("The time now is %s", ctime(&tt));
		return 1;
	}

	if ((strcmp(args[0], "generate") == 0 || strcmp(args[0], "g") == 0) && args[1] == NULL) { 
		if (rnd) {
			FILE * pfile;
			pfile = fopen("generation.txt", "w");
			generate_random(mySys, 10, 0.7, true, pfile);
			fclose(pfile);
		}
		else printf("Please use enrand to enable random generation!\n");
		return 1;
	}
	if((strcmp(args[0], "view") == 0 || strcmp(args[0], "v") == 0) && args[1] == NULL){
		char s[100];
		printf("Enter the pid you want to check.\n");
		fgets(s, 100, stdin);
		int pid = atoi(s);
		if(pid <= 0 || pid > mySys->nPer){
			printf("No such pid!\n");
			return 1;
		}
		else{
			printf("======================================================\n");
			printf("pid: %d timestamp: %s ID: %d name: %s wechat: %s phone: %s\n",
				mySys->dbPerson[pid]->pid, 
				mySys->dbPerson[pid]->regDate.c_str(), 
				mySys->dbPerson[pid]->idNumber, 
				mySys->dbPerson[pid]->name.c_str(), 
				mySys->dbPerson[pid]->WeChatn.c_str(), mySys->dbPerson[pid]->phonen.c_str()); 
			printf("lo: %f la: %f profession: %d risk: %d\n",
				mySys->dbPerson[pid]->addLon, 
				mySys->dbPerson[pid]->addLat, 
				mySys->dbPerson[pid]->profession, 
				mySys->dbPerson[pid]->riskSt);
			printf("birthday: %s ageGroup: %d nextUpdate: %s\n",
				mySys->dbPerson[pid]->dateBirth.c_str(), 
				mySys->dbPerson[pid]->ageGroup, 
				mySys->dbPerson[pid]->nxtUpdate.c_str());
			printf("priority: %d pri_ddl: %s tmpSt:%d\n", 
				mySys->dbPerson[pid]->priLetter, 
				mySys->dbPerson[pid]->priDate.c_str(),
				mySys->dbPerson[pid]->tmpSt);
		}
		return 1;
	}
	if((strcmp(args[0], "Fibonacci") == 0 || strcmp(args[0], "F") == 0) && args[1] == NULL){
		if (mySys->FH->totNum == 0){
			printf("Null Fibonacci Heap\n");
			return 1;
		}
		else {
			printf("Fibonacci of pid:\n");
			mySys->FH->printFib();
			return 1;
		}
	}

	if ((strcmp(args[0], "set") == 0 || strcmp(args[0], "s") == 0) && args[1] == NULL) { 
		printf("1: withdraw, 2: edit, 3: create, 4: call local update, 5: injection station info\n");
		int choice, pid, reg, choice2, num, inj;
		bool modified = false;
		FILE* temp = NULL;
		char s[100];

		fgets(s, 100, stdin);
		choice = atoi(s);
		switch(choice){
			case 1: // Withdraw.
				printf("Input PID.\n");
				fgets(s, 100, stdin);
				pid = atoi(s);
				if (pid <= 0 || pid > mySys->nPer){
					printf("Invalid pid!\n");
					return 1;
				}
				mySys->dbPerson[pid]->withdraw(mySys->FH); 
				printf("Withdraw performed.\n");
				return 1;
			case 2: // Edit
				printf("Input PID.\n");
				fgets(s, 100, stdin);
				pid = atoi(s);
				if (pid <= 0 || pid > mySys->nPer || mySys->dbPerson[pid]->tmpSt == 2 || mySys->dbPerson[pid]->tmpSt == 4){
					printf("Invalid pid!\n");
					return 1;
				}
				while(!modified){
					cout<<"Modify\n";
					cout<<"0: modification done!\n";
					cout<<"1: name\n";
					cout<<"2: wechat\n";
					cout<<"3: email\n";
					cout<<"4: phone number\n";
					cout<<"5: address\n";
					cout<<"6: profession\n";
					cout<<"7: risk state\n";
					cout<<"8: priority\n";
					int pick;
					string new_infomation = ""; // Contain the new string to be stored.
					int cindex; // To help convert Cstring -> string.
					float f1, f2;
					int i1;
					fgets(s, 100, stdin);
					pick = atoi(s);
					switch(pick){
						case 0:
							modified = true;
							printf("Modification for pid %d accomplished.\n", pid);
							break;
						case 1: // name modification.
				 			printf("Input new name: ");
							fgets(s, 100, stdin);
							cindex = 0;
							while(s[cindex] != '\n'){
								new_infomation += s[cindex];
								cindex++;
							}
							mySys->dbPerson[pid]->name = new_infomation;
							break;
						case 2: // wechat modification.
							printf("Input new wechat: ");
							fgets(s, 100, stdin);
							cindex = 0;
							while(s[cindex] != '\n'){
								new_infomation += s[cindex];
								cindex++;
							}
							mySys->dbPerson[pid]->updInf(2, new_infomation);
							break;
						case 3: // email modification.
							printf("Input new email: ");
							fgets(s, 100, stdin);
							cindex = 0;
							while(s[cindex] != '\n'){
			 				new_infomation += s[cindex];
								cindex++;
							}
							mySys->dbPerson[pid]->updInf(3, new_infomation);
							break;
						case 4: // phone modification.
							printf("Input new phone number: ");
							fgets(s, 100, stdin);
							cindex = 0;
							while(s[cindex] != '\n'){
								new_infomation += s[cindex];
								cindex++;
							}
							mySys->dbPerson[pid]->updInf(1, new_infomation);
							break;
						case 5: // address modification.
							printf("Input new lo: \n");
							fgets(s, 100, stdin);
							f1 = atof(s);
							printf("Input new la: \n");
							fgets(s, 100, stdin);
							f2 = atof(s);
						
							mySys->dbPerson[pid]->updAdd(f1, f2);
							break;
						case 6: // profession modification.
							printf("Input new profession(0~8): ");
							fgets(s, 100, stdin);
							i1 = atoi(s);
							mySys->dbPerson[pid]->updPro(i1, mySys->FH);
							break;
						case 7: // risk modification.
							printf("Input new risk(0~2): ");
							fgets(s, 100, stdin);
							i1 = atoi(s);
							mySys->dbPerson[pid]->updRiskSt(i1, mySys->FH);
							break;
						case 8: //priority letter.
							mySys->dbPerson[pid]->priLetter = 1;
							printf("How many days since %s", ctime(&tt));
							fgets(s, 100, stdin);
							i1 = atoi(s);
							if (i1 > 0){
								mySys->dbPerson[pid]->priDate = timeString(tt + DAY_SEC*i1);
								mySys->PL->insert(mySys->dbPerson[pid]);
							}
							else {
								mySys->dbPerson[pid]->priLetter = 0;
								printf("Invalid amount of days!\n");
							}
							break;
						default:
							printf("Invalid choice!\n");
							break;
					}
				}
				return 1;
			case 3:
				printf("1: new patient(with random initiation), 2: re-register\n");
				fgets(s, 100, stdin);
				choice2 = atoi(s);
				if (choice2 == 1){
					generate_single(mySys, 0.7, false, temp);
					printf("A new random patient has been generated.\n");
					printf("The pid is %d\n", mySys->nPer); // Risky, here we assume the last one to be the person just generated.
					printf("profession: %d\n", mySys->dbPerson[mySys->nPer]->profession);
					printf("priority: %d\n", mySys->dbPerson[mySys->nPer]->priLetter);
					if (mySys->dbPerson[mySys->nPer]->priLetter != 0){
						printf("ddl: %s\n", mySys->dbPerson[mySys->nPer]->priDate.c_str());
					}
					return 1;
				}
				else if (choice2 == 2){
					printf("Input original pid: ");
					fgets(s, 100, stdin);
					pid = atoi(s);
		
					if (pid <= 0 || pid > mySys->nPer || mySys->dbPerson[pid]->tmpSt == 2 ||
							mySys->dbPerson[pid]->tmpSt == 4){
						printf("Invalid pid!\n");
						return 1;
					}

					printf("Which registration station? ");
					fgets(s, 100, stdin);
					reg = atoi(s);
		
					if (reg < 1 || reg > mySys->numReg){
						printf("Invalid registration station!\n");
						return 1;
					}

					mySys->dbPerson[pid]->reRegister();
					mySys->dbPerson[pid]->regDate = timeString(tt);
					mySys->dbReg[reg]->cnt++;
					mySys->dbReg[reg]->locList.push_back(pid);
					//mySys->dbReg[reg]->locList[mySys->dbReg[reg]->cnt] = pid;
					mySys->UA->insert(mySys->dbPerson[pid]);
					if(mySys->dbPerson[pid]->priLetter == 1){
						mySys->PL->insert(mySys->dbPerson[pid]);
					}
					return 1;
				}
				printf("Invalid choice!\n");	
				return 1;
			case 4: // Forward infomation.
				printf("Input #registration station: ");
				fgets(s, 100, stdin);
				reg = atoi(s);
				if (reg < 1 || reg > mySys->numReg){
					printf("Invalid #registration station.\n");
					return 1;
				}
				mySys->dbReg[reg]->forwardInf(mySys->FH, mySys->dbPerson);
				return 1;	
			case 5: // For update of injection station.
				printf("Which injection station?\n");
				fgets(s, 100, stdin);
				inj = atoi(s);
				if (inj < 1 || inj > mySys->numInj){
					printf("Invalid #injection station.\n");
					return 1;
				}
				printf("1: update capacity, 2: update injection information\n");
				fgets(s, 100, stdin);
				choice2 = atoi(s);
				if (choice2 == 1){ // update capacity.
					printf("New capacity: ");
					fgets(s, 100, stdin);
					num = atoi(s);
					mySys->totcap += num - mySys->dbInj[inj]->capacity;
					mySys->dbInj[inj]->capacity = num;
					return 1;
				}
				else if (choice2 == 2){ // update injection infomation.
					printf("Input number of patients that didn't show up: ");
					fgets(s, 100, stdin);
					num = atoi(s);
					for (int k = 0; k < num; k++){
						printf("#%d of them has pid: ", k+1);
						fgets(s, 100, stdin);
						pid = atoi(s);
						mySys->dbPerson[pid]->withdraw(mySys->FH);
					}
					return 1;
				}
				printf("Invalid choice!\n");	
				return 1;

			default:
				printf("Invalid choice!\n");
				return 1;
		}
		return 1;
	}



    return terminal_launch(args);
}

/* ================= The following are functions for testing ================ */

void Test::setup(){
	time(&tt); // Setup the time right now, and update tt for simulation.
	t_last = tt; // Record the starting time.
	return;
}

void Test::nextDay(){
	tt += DAY_SEC;
	struct tm* temp = localtime(&tt); 
	temp->tm_hour = 0;
	temp->tm_min = 0;
	temp->tm_sec = 0;
	tt = mktime(temp);
	return; 
}

string Test::timeString(time_t t){
	string t_str = "";
	struct tm* t_ptr = localtime(&t);
	t_str += to_string(t_ptr->tm_year + 1900);
	t_str += "-";
	if (t_ptr->tm_mon + 1 < 10) t_str += "0";
	t_str += to_string(t_ptr->tm_mon + 1);
	t_str += "-";
	if (t_ptr->tm_mday < 10) t_str += "0";
	t_str += to_string(t_ptr->tm_mday);
	t_str += "-";
	if (t_ptr->tm_hour < 10) t_str += "0";
	t_str += to_string(t_ptr->tm_hour);
	t_str += "-";
	if (t_ptr->tm_min < 10) t_str += "0";
	t_str += to_string(t_ptr->tm_min);
	t_str += "-";
	if (t_ptr->tm_sec < 10) t_str += "0";
	t_str += to_string(t_ptr->tm_sec);

	return t_str;
}

/* 
 * This function generates a set of testing input for one single day. 
 * num  indicates the total number of patients that register today. 
 * r_I_patient is the rough ratio of I type(risk) patients in total patients.
 *
 * The randomed value are:
 * identification number (this will be the number of person in the whole list up to now and hence being unique)
 * name
 * contact details
 * profession
 * date of birth
 * risk status
 * 
 * The time stamp will be the exact time of simulation, we assume each registration takes exactly one second.
 *
 * */
void Test::generate_random(MeTsystem* mySys, int num, float r_I_patient, bool write, FILE * pfile) {
	fprintf(pfile, "%s", ctime(&tt));
	for (int i = 0; i < num; i++){ 
		generate_single(mySys, r_I_patient, write, pfile);
	}
	return;
}

void Test::generate_single(MeTsystem* mySys, float r_I_patient, bool write, FILE * pfile){
	MeTPerson* newm = new MeTPerson();
	newm->weekUpdate = 1;
    newm->monthUpdate = 1;
	int reg = rand()%mySys->numReg + 1; // #reg.
	mySys->nPer++; //
	tt += 1;// This line controls the timestamp!! Disable it if input is regarded as imediately done.
	string gen_time_stamp = timeString(tt);

	/* From iniMeTPerson*/
	newm->pid = mySys->nPer;									// pid
	newm->idRegpos = reg;										// registration station 
	int gen_id = (rand()%4 + 1)*100000 + rand()%100000;
	newm->idNumber = gen_id;								 	// id

	/* Name generation */
	string gen_name = "";
	for (int k = 0; k < 10; k++){
		if (rand()%2 == 0){
			gen_name += rand()%26 + 'a';
		}
		else{
			gen_name += rand()%26 + 'A';
		}
	}
	newm->name = gen_name;										// name

	/* Contact details generation, wechat, email, phone, address. */
	string gen_wechat = "";
	for (int k = 0; k < 15; k++){
		if (rand()%3 == 0){
			gen_wechat += rand()%26 + 'a';
		}
		else if (rand()%2 == 0){
			gen_wechat += rand()%26 + 'A';
		}
		else {
			gen_wechat += rand()%10 + '0';
		}
	}
	newm->WeChatn = gen_wechat;									// wechat
		
	string gen_email = "";
	for (int k = 0; k < 15; k++){
		if (rand()%3 == 0){
			gen_email += rand()%26 + 'a';
		}
		else if (rand()%2 == 0){
			gen_email += rand()%26 + 'A';
		}
		else {
			gen_email += rand()%10 + '0';
		}
	}
	newm->eMailn = gen_email;									// email
			
	string gen_phone = "1";
	for (int k = 0; k < 10; k++){
		gen_phone += rand()%10 + '0';
	}
	newm->phonen = gen_phone;									// phone

	float gen_lo = (float)(rand()%100) * 0.01 + (float)(rand()%1000);
	float gen_la = (float)(rand()%100) * 0.01 + (float)(rand()%1000);
	newm->addLon = gen_lo;
	newm->addLat = gen_la;										// address
		
	int gen_year = rand()%70 + 1;
	gen_year = 2022 - gen_year;
	int gen_mon = rand()%12 + 1;
	int gen_mday = rand()%28 + 1;
	newm->dateBirth = to_string(gen_year) + "-";
	if (gen_mon < 10) newm->dateBirth += "0";
	newm->dateBirth += to_string(gen_mon);
	newm->dateBirth += "-";
	if (gen_mday < 10) newm->dateBirth += "0";
	newm->dateBirth += to_string(gen_mday);						// birthday

	newm->ageGroup = newm->calAge(gen_time_stamp);				// age

	int gen_pro = rand()%8;
 	newm->profession = gen_pro; 								// profession

	int gen_risk;
	if (rand()%100 + 1 < 100*r_I_patient)
		gen_risk = 0;
	else if (rand()%2 == 0)
		gen_risk = 1;
	else gen_risk = 2;
	newm->riskSt = gen_risk;									// risk

	if(newm->riskSt == 1) newm->punishTime = 4;

	int extension, gen_pri;
	//if(rand()% 100 == 0){
	if(false){
		gen_pri = 1;
		extension = 20 + rand()% 15  + 1;
		newm->priLetter = 1;
		time_t temp = tt + DAY_SEC*extension;
		struct tm* temp_tm = localtime(&temp);
		int pri_year = 1900 + temp_tm->tm_year;
		int pri_mon = temp_tm->tm_mon + 1;
		int pri_mday = temp_tm->tm_mday;
		newm->priDate = to_string(pri_year) + "-";
		if (pri_mon < 10) newm->priDate += "0";
		newm->priDate += to_string(pri_mon);
		newm->priDate += "-";
		if (pri_mday < 10) newm->priDate += "0";
		newm->priDate += to_string(pri_mday);				
	}
	else{
		newm->priLetter = 0;									// priority
		newm->priDate = "";
		gen_pri = 0;
	}
	/* At last, put this person into the database of system. */
	mySys->dbPerson[mySys->nPer] = newm;  
	mySys->dbPerson[mySys->nPer]->regDate = gen_time_stamp;			// time stamp
	mySys->dbReg[reg]->cnt++;
	mySys->dbReg[reg]->locList.push_back(mySys->nPer);
	//mySys->dbReg[reg]->locList[mySys->dbReg[reg]->cnt] = mySys->nPer; 
	//mySys->dbReg[reg]->RegPid.push(mySys->nPer);
	if (mySys->dbPerson[mySys->nPer]->ageGroup != 7)
		mySys->UA->insert(newm);
	if(newm->priLetter == 1)	mySys->PL->insert(newm);

	/* Output generation to txt file. */
	if (write){
		fprintf(pfile, "======================================================\n");
		fprintf(pfile, "timestamp: %s ID: %d name: %s wechat: %s phone: %s\n",
				gen_time_stamp.c_str(), gen_id, gen_name.c_str(), 
				gen_wechat.c_str(), gen_phone.c_str()); 
		fprintf(pfile, "lo: %f la: %f profession: %d risk: %d ageGroup: %d nextUpdate: %s\n",
				gen_lo, gen_la, gen_pro, gen_risk, newm->ageGroup, 
				newm->nxtUpdate.c_str());
		fprintf(pfile, "priority: %d pid: %d pri_ddl: %s\n", 
				gen_pri, newm->pid, newm->priDate.c_str());
	}

}
