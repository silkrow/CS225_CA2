#include "MeTsystem.h"
#include "InjPos.h"
#include "RegPos.h"
#include "MeTPerson.h"
#include "UpAge.h"
#include "PriLet.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <fstream>

using namespace std;

/* 
Construction function: initialize the whole system
including: information about registration and injection positions
sortby of weekly reports
also create the structures such as Fib-heap and common heap for age update and priority letters
*/

MeTsystem::MeTsystem(/*int nOper*/){
	char s[100];
    //this->nOp = nOper;
    printf("Begin Initialization.\n");
    printf("Enter a positive number of Vaccination sites\n");
    int nvc;
	fgets(s, 100, stdin);
	nvc = atoi(s);
    //cin>>nvc;
    this->numInj=nvc;
	this->totcap = 0;
	this->totass = 0;
	//this->sortby = 0;
	printf("Enter a positive number of Registration sites\n");
    int nrg;
    fgets(s, 100, stdin);
	nrg = atoi(s);
    this->numReg = nrg;
    for(int i = 1; i <= nvc; i++){
        printf("Please enter the %d-th injection site's capacity\n",i);
        int cap;
		fgets(s, 100, stdin);
		cap = atoi(s);
    	double lo, la;
    	printf("Please enter its positon -- longitude\n");
		fgets(s, 100, stdin);
		lo = atof(s);
        printf("Please enter its positon -- latitue\n");
		fgets(s, 100, stdin);
		la = atof(s);
        this->dbInj[i] = new InjPos(i, cap, lo,la);
		this->totcap += cap;
    }
    for(int i = 1; i <= nrg; i++){
        this->dbReg[i] = new RegPos(i);
    }
	printf("By which property shall weekly report be sorted by?\n");
	printf("0: name\n");
    printf("1: profession category\n");
    printf("2: age group\n");
	fgets(s, 100, stdin);
	this->sortby = atoi(s);

    //this->PL = new PriLet();
    this->FH = new FibHeap();
    this->PL = new PriLet();
    this->UA = new UpAge();
    printf("Initialization completed. Type 'setup' to start testing.\n");
}

/*
2 compare functions for injection positions' arrangement
*/
bool cmpx(InjPos* x1, InjPos* x2){
    return x1->longitude < x2->longitude;
}
bool cmpy(InjPos* x1, InjPos* x2){
    return x1->latitude < x2->latitude;
}

/*
3 different compare functions for different sortby in weekly reports
*/

bool cmp0(MeTPerson* a, MeTPerson* b){
    int a1 = a->tmpSt;
    int a2 = b->tmpSt;
    string b1 = a->name;
    string b2 = b->name;
    int p1, p2;
    if(a1 == 4) p1 = 1;
    else if(a1 == 2) p1 = 2;
    else if(a1 == 1) p1 = 3;
    else p1 = 4;
    if(a2 == 4) p2 = 1;
    else if(a2 == 2) p2 = 2;
    else if(a2 == 1) p2 = 3;
    else p2 = 4;
    
    if(p1 < p2) return 1;
    if(p1 > p2) return 0;
    return (b1<b2);
}

bool cmp1(MeTPerson* a, MeTPerson* b){
	int a1 = a->tmpSt;
    int a2 = b->tmpSt;
    int b1 = a->profession;
    int b2 = b->profession;
    int p1, p2;
    if(a1 == 4) p1 = 1;
    else if(a1 == 2) p1 = 2;
    else if(a1 == 1) p1 = 3;
    else p1 = 4;
    if(a2 == 4) p2 = 1;
    else if(a2 == 2) p2 = 2;
    else if(a2 == 1) p2 = 3;
    else p2 = 4;
    
    if(p1 < p2) return 1;
    if(p1 > p2) return 0;
    return (b1<b2);
}

bool cmp2(MeTPerson* a, MeTPerson* b){
    int a1 = a->tmpSt;
    int a2 = b->tmpSt;
    int b1 = a->ageGroup;
    int b2 = b->ageGroup;
    int p1, p2;
    if(a1 == 4) p1 = 1;
    else if(a1 == 2) p1 = 2;
    else if(a1 == 1) p1 = 3;
    else p1 = 4;
    if(a2 == 4) p2 = 1;
    else if(a2 == 2) p2 = 2;
    else if(a2 == 1) p2 = 3;
    else p2 = 4;
    
    if(p1 < p2) return 1;
    if(p1 > p2) return 0;
    return (b1<b2);
}


/*
daily withdraw check:
The injection sites will give us daily feedback on the list of 
those who did not attend treatment on time that day. 
We use this list to confirm the WITHDRAW status.
*/
void MeTsystem::withdrawCheck(string tt){
    for(int i = 1; i <= this->totass; i++){
        if(this->dbPerson[this->assid[i]]->tmpSt != 3){
            this->dbPerson[this->assid[i]]->tmpSt = 4;
            this->dbPerson[this->assid[i]]->watime = this->dbPerson[this->assid[i]]->calwaitingtime(tt) - 1; // yesterday
        }
    }
    totass = 0; // clear the array
}


/*
Each person's age changes over time, 
so we build a general heap to determine which people's age groups need to be changed.
*/
void MeTsystem::dailyAgeCheck(string tt){
	if (UA->siz == 0) return;
    while(datecmp(tt, UA->top()->nxtUpdate)){
        MeTPerson* tmp = UA->top();
        UA->pop();
        tmp->updAge(tt, FH);
		if (tmp->ageGroup != 7) 
        	UA->insert(tmp);
		if (UA->siz == 0) return;
    }
}


/*
compare 2 date(YYYY-MM-DD):return 0 if s1 is ealier than s2
*/
bool MeTsystem::datecmp(string s1, string s2){
    int by1,bm1,bd1,by2,bm2,bd2;
    by1=(s1[0]-'0')*1000+(s1[1]-'0')*100+(s1[2]-'0')*10+(s1[3]-'0');
    bm1=(s1[5]-'0')*10+(s1[6]-'0');
    bd1=(s1[8]-'0')*10+(s1[9]-'0');

    by2=(s2[0]-'0')*1000+(s2[1]-'0')*100+(s2[2]-'0')*10+(s2[3]-'0');
    bm2=(s2[5]-'0')*10+(s2[6]-'0');
    bd2=(s2[8]-'0')*10+(s2[9]-'0');
    
    if (by1<by2) return 0;
    if (by1==by2&&(bm1<bm2)) return 0;
    if (by1==by2&&bm1==bm2&&bd1<bd2)  return 0;
    return 1; 
}

/*
call those who have priority letter:
If some person's priority letter deadline is forthcoming(in 5 days), 
handle specially 

*/
int MeTsystem::priorityAss(string tt){
    int tot = this->totcap;
    int ret = 0;
	if (PL->siz == 0) return 0;
    while(PL->top()->calpriddl(tt) <= 5 && ret < tot){
        MeTPerson* tmpp = PL->top();
        PL->pop();
        if(tmpp->tmpSt == 3) continue;
        ret++;
        this->assid[ret] = tmpp->pid;
		tmpp->delmark = 1;
        FH->delNode(tmpp->Fibnode);
		tmpp->tmpSt = 2;
        tmpp->weekUpdate = 1;
        tmpp->monthUpdate = 1;
		if(PL->siz == 0) break;
    }
    return ret;
}

/*
The common assignment method, 
extract front elements from centralized queue (Fib-Heap)
*/

int MeTsystem::commomAss(int t){
    int tot = this->totcap;
    int ret = t;
    /* CALL FIB-QUEUE and find the first(tot-t) ids*/
    for(int i = t + 1; i <= tot; i++){
        int tmpid = this->FH->removeM();
        if (tmpid == 0) break; // no valid 
		if (dbPerson[tmpid]->tmpSt == 3) continue;
        this->assid[i] = tmpid;
        ret++;
		dbPerson[tmpid]->tmpSt = 2;
        dbPerson[tmpid]->weekUpdate = 1;
        dbPerson[tmpid]->monthUpdate = 1;
    }
    return ret;
}

/*
Check for those who did not treat on time yesterday
Check if anyone's age group has changed
Generate a list of people who will be available for treatment tomorrow
Placing people at the right injection sites
*/
void MeTsystem::dailyRep_MeT(string tt){
    withdrawCheck(tt); // clear tmp array
    dailyAgeCheck(tt); // update age groups
     // first assign those with priority letter,
	int pri_a = priorityAss(tt);
    int nas = commomAss(pri_a); // then from fib queue
	this->totass = nas;
    AssignInj(); // arrange inj-pos for the selected ones

    FILE* file1;
    file1 = fopen("daily.txt", "w");
    fprintf(file1, "Daily Report\n");
	fprintf(file1, "%s\n", tt.c_str());
    fprintf(file1, "Total number of people assigned: %d\n", nas);
    for(int i = 1; i <= nas; i++){
        fprintf(file1, "PID = %d, Injection ID = %d\n", this->assid[i], this->dbPerson[this->assid[i]]->idInjPos);
    }
	fclose(file1);
}

/*
Generate a list of people who have activities last week
include: new registration/withdraw/finish treatment/in treatment list
*/
void MeTsystem::weeklyRep_MeT(string tt){	
    int printtot = 0;
	
	for(int i = 1; i <= this->nPer ; i++){
        if(dbPerson[i]->weekUpdate == 0) continue;
        printtot++;
		this->repper[printtot] = dbPerson[i];
        dbPerson[i]->weekUpdate = 0;
	}


    if(sortby == 0){
       	sort(repper+1, repper+printtot+1, cmp0);
    }
    else if(sortby == 1){
       	sort(repper+1, repper+printtot+1, cmp1);
	}	
   	else if(sortby == 2){
		sort(repper+1, repper+printtot+1, cmp2);
	}
    FILE* file2;
    file2 = fopen("weekly.txt", "w");

    fprintf(file2, "Weekly Report\n");
	fprintf(file2, "%s\n", tt.c_str());
    fprintf(file2, "The people who have been treated:\n");
    fprintf(file2, "PID/name/profession category/age group/waiting time/risk/assigned injection state\n");
    int j = 1;
	while(j <= printtot && this->repper[j]->tmpSt == 4){
        this->repper[j]->printPer(tt, file2);
        j++;
    }
    fprintf(file2, "The registered people with a set appointment:\n");
    fprintf(file2, "PID/name/profession category/age group/waiting time/risk/assigned injection state\n");
	while(j <= printtot && this->repper[j]->tmpSt == 2){
        this->repper[j]->printPer(tt, file2);
        j++;
    }
    fprintf(file2, "The queueing people without a set appointment:\n");
    fprintf(file2, "PID/name/profession category/age group/waiting time/risk/assigned injection state\n");
    while(j <= printtot && this->repper[j]->tmpSt == 1){
        this->repper[j]->printPer(tt, file2);
        j++;
    }
    fprintf(file2, "Withdraw people:\n");
    fprintf(file2, "PID/name/profession category/age group/waiting time/risk/original assigned inj-state\n");
    for(int i = j; i <= printtot; i++){
        this->repper[i]->printPer(tt, file2);
    }
	fclose(file2);
	return;
}

/*
Generate a list of people in summary of a month
*/

void MeTsystem::monthlyRep_MeT(string tt){
    FILE* file3;
    file3 = fopen("monthly.txt", "w");
    fprintf(file3, "Monthly Report\n");
	fprintf(file3, "%s\n", tt.c_str());
    fprintf(file3, "Number of registered people in total: %d\n", this->nPer);
    int newwait = 0;
    int totalwait = 0;
    int newappointments = 0;
    int totalappointments = 0;
    int newwithdraw = 0;
    int totalwithdraw = 0;
    int totwaitingtime = 0;
    for(int i = 1; i <= this->nPer; i++){
        //if(dbPerson[i]->monthUpdate == 0) continue;
        if(dbPerson[i]->tmpSt == 1){ // waiting
            totalwait++;
            totwaitingtime += dbPerson[i]->calwaitingtime(tt);
            if(dbPerson[i]->monthUpdate == 1) newwait++; // register in this month
        }
        else if(dbPerson[i]->tmpSt == 2 || dbPerson[i]->tmpSt == 4){ // finish or have an appointment
            totalappointments++;
            totwaitingtime += dbPerson[i]->watime; // stastic waiting time
            if(dbPerson[i]->monthUpdate == 1) newappointments++;
        }
        else{ // withdraw
            totalwithdraw++;
            if(dbPerson[i]->monthUpdate == 1) newwithdraw++;
        }
        dbPerson[i]->monthUpdate = 0;
    }
    double avtime;
	if(totalwait+totalappointments == 0) avtime = 0.00; 
	else avtime = (double)totwaitingtime / ((double)totalwait + (double)totalappointments);
    fprintf(file3, "Number of treatment appointment in total: %d\n", totalappointments);
    fprintf(file3, "Number of treatment appointment new this month: %d\n", newappointments);
    fprintf(file3, "Number of waiting in total: %d\n", totalwait);
    fprintf(file3, "Number of waiting new this month: %d\n", newwait);
    fprintf(file3, "Number of withdraw in total: %d\n", totalwithdraw);
    fprintf(file3, "Number of withdraw new this month: %d\n", newwithdraw);
    fprintf(file3, "Average waiting time: %.2lf days\n", avtime);
	fclose(file3);
}


/*
Uncomment this function if you need to test interactivity
*/
/*
void MeTsystem::run_MeT(string tt){ // tt in YYYY-MM-DD-sfm
    //int numOp = this->nOp;
    //for(int i = 1; i<= numOp; i++){
        int iOp;
        printf("Choose your identity:");
        printf("O: Staff in Registration Site\n");
        printf("1: Staff in Injection Site\n");
        printf("2: Ask for report\n");
        cin>>iOp;
        switch(iOp){
            case 0: {
                int idR;
                printf("Please enter the id of your site.\n");
                cin>>idR;
                if(idR > this->numReg){
                    printf("ERROR! Invalid site id!\n");
                    break;
                }
                printf("Choose an operation:\n");
                printf("0: New Registration\n");
                printf("1: Update personal information\n");
                printf("2: Withdraw\n");
                printf("3: Forward information\n"); // twice daily
                printf("4: Re-Register\n");
                int opr;
                cin>>opr;
                switch (opr){
                case 0:{
                    MeTPerson* newm;
                    if(0 != newm->iniMeTPerson(nPer + 1, idR, tt)){
                        // successfully
                        nPer++;
                        printf("Your unique PID is %d\n",nPer);
                        this->dbPerson[nPer] = newm;
                        this->dbPerson[nPer]->regDate = tt;
                        this->dbReg[idR]->cnt++;
                        this->dbReg[idR]->locList[this->dbReg[idR]->cnt] = nPer;
                        //this->dbReg[idR]->RegPid.push(nPer);
                        this->UA->insert(newm);
                        if(this->dbPerson[nPer]->priLetter == 1){
                            this->PL->insert(newm);
                        }
                    }
                    break;
                }
                case 1:{          
                    printf("Please enter your PID\n");
                    int tmpid, tmpop;
                    cin>>tmpid;
                    printf("Which information do you want to change?\n");
                    printf("0: Phone Number\n");
                    printf("1: WeChat\n");
                    printf("2: Email\n");
                    printf("3: Address\n");
                    printf("4: Profession\n");
                    printf("5: Risk Status\n");
                    cin>>tmpop;
                    switch (tmpop){
                    case 0:{
                        string newi;
                        printf("Please enter your new Phone Number\n");
                        getline(cin, newi);
                        this->dbPerson[tmpid]->updInf(1, newi);
                        break;
                    }
                    case 1:{
                        string newi;
                        printf("Please enter your new WeChat\n");
                        getline(cin, newi);
                        this->dbPerson[tmpid]->updInf(2, newi);
                        break;
                    }
                    case 2:{
                        string newi;
                        printf("Please enter your new Email\n");
                        getline(cin, newi);
                        this->dbPerson[tmpid]->updInf(3, newi);
                        break;
                    }
                    case 3:{
                        double nla, nlo;
                        printf("Please enter your new address, latitude and longitude\n");
                        cin>>nla>>nlo;
                        this->dbPerson[tmpid]->updAdd(nlo, nla);
                        break;
                    }
                    case 4:{
                        int newpr;
                        printf("Please enter your new profession\n");
                        cin>>newpr;
                        this->dbPerson[tmpid]->updPro(newpr, FH);
                        break;
                    }
                    case 5:{
                        int newri;
                        printf("Please enter your new risk status\n");
                        cin>>newri;
                        this->dbPerson[tmpid]->updRiskSt(newri, FH);
                        break;
                    }
                    default:
                        break;
                    }
                }
                case 2:{
                    printf("Please enter your PID\n");
                    int tmpid;
                    cin>>tmpid;
                    this->dbPerson[tmpid]->withdraw(FH);
                    break;
                }
                case 3:{
                    this->dbReg[idR]->forwardInf(FH, dbPerson);
                    break;
                }
                case 4:{
                    printf("Please enter your PID\n");
                    int tmpid;
                    cin>>tmpid;
                    this->dbPerson[tmpid]->reRegister();
                    this->dbPerson[tmpid]->regDate = tt;
                    this->dbReg[idR]->cnt++;
                    this->dbReg[idR]->locList[this->dbReg[idR]->cnt] = tmpid;
                    this->UA->insert(this->dbPerson[tmpid]);
                    if(this->dbPerson[nPer]->priLetter == 1){
                        this->PL->insert(this->dbPerson[tmpid]);
                    }
                    break;
                }
                default:
                    break;
                }
                break;           
            }
            case 1: {
                int idI;
                printf("Please enter the id of your site.\n");
                cin>>idI;
                if(idI > this->numInj){
                    printf("ERROR! Invalid site id!\n");
                    break;
                }
                printf("Choose an operation:\n");
                printf("0: Change capacity\n");
                printf("1: Update injection namelist\n");
                int tmpope;
                cin>>tmpope;
                if(tmpope == 0){
                    printf("Please enter new capacity\n");
                    int newc;
                    cin>>newc;
                    int oldc = this->dbInj[idI]->capacity;
                    this->dbInj[idI]->capacity = newc;
                    this->totcap = this->totcap - oldc + newc;
                }
                else{
                    printf("Please enter the number of people who did not come.\n");
                    int tota;
                    cin>>tota;
                    for(int i = 1; i <= tota; i++){
                        printf("Please enter the %d-th person's PID\n",i);
                        int tmppid;
                        cin>>tmppid;
                        this->dbPerson[tmppid]->withdraw(FH);
                    }
                }
                break;
            }
            case 2: {
                int nrep;
                printf("Choose an operation:\n");
                printf("0: Daily Report\n");
                printf("1: Weekly Report\n");
                printf("2: Monthly Report\n");
                cin>>nrep;
                switch (nrep)
                {
                case 0: {
                    dailyRep_MeT(tt);
                    break;
                }
                case 1: {
                    weeklyRep_MeT(tt);
                    break;
                }

                case 2: {
                    monthlyRep_MeT(tt);
                    break;
                }
                default:{
                    printf("ERROR! Invalid Operation!\n");
                    break;
                }
                }
                break;
            }
            default: {
                printf("ERROR! You do not have access to the system.\n");
            }
        }
    //}
}
*/

/*
Calculate the distance between a person and a injection position 
*/
double MeTsystem::Dis(InjPos* a,MeTPerson* b){
    return sqrt((a->longitude-b->addLon)*(a->longitude-b->addLon)+(a->latitude-b->addLat)*(a->latitude-b->addLat));
}


/*
Assign Injection position for each person waiting to be injected tomorrow
*/
void MeTsystem::AssignInj(){
    InjPos* a1[10005];
    InjPos* a2[10005];
    int temp[10005];

    for (int i=1;i<=numInj;i++){
        a1[i]=dbInj[i];
        a2[i]=dbInj[i];
        temp[dbInj[i]->InjID]=dbInj[i]->capacity;
    }
    sort(a1+1,a1+numInj+1,cmpx);
    sort(a2+1,a2+numInj+1,cmpy);

    int delta=5;

    for (int j = 1 ; j <= this->totass; j++){
        int i = this->assid[j];
        int l,r,x,res=-1;
        double Min=0;
        
        l=1; r=numInj;
        while (r-l>1){
            int mid=(l+r)/2;
            if (a1[mid]->longitude>dbPerson[i]->addLon) r=mid-1;
            else l=mid+1;
        }
        x=delta; 
        while (l>=1&&x>0){
            if (temp[a1[l]->InjID]==0){
                l--; continue;
            }
            x--;
            if (res==-1||Dis(a1[l],dbPerson[i])<Min){
                res=a1[l]->InjID; Min=Dis(a1[l],dbPerson[i]); 
            }
        }
        x=delta; 
        while (r<=numInj&&x>0){
            if (temp[a1[r]->InjID]==0){
                r++; continue;
            }
            x--;
            if (res==-1||Dis(a1[r],dbPerson[i])<Min){
                res=a1[r]->InjID; Min=Dis(a1[r],dbPerson[i]); 
            }
        }
        l=1; r=numInj;
        while (r-l>1){
            int mid=(l+r)/2;
            if (a2[mid]->latitude>dbPerson[i]->addLat) r=mid-1;
            else l=mid+1;
        }
        x=delta; 
        while (l>=1&&x>0){
            if (temp[a2[l]->InjID]==0){
                l--; continue;
            }
            x--;
            if (res==-1||Dis(a2[l],dbPerson[i])<Min){
                res=a2[l]->InjID; Min=Dis(a2[l],dbPerson[i]); 
            }
        }
        x=delta; 
        while (r<=numInj&&x>0){
            if (temp[a2[r]->InjID]==0){
                r++; continue;
            }
            x--;
            if (res==-1||Dis(a2[r],dbPerson[i])<Min){
                res=a2[r]->InjID; Min=Dis(a2[r],dbPerson[i]); 
            }
        }





        if (res==-1){
            dbPerson[i]->idInjPos=0;
            continue;
        }
        temp[res]--;
        dbPerson[i]->idInjPos=res;
    }
}
