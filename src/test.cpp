#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"
//#include "MeTsystem.h"
//#include "MeTPerson.h"
//#include "InjPos.h"
//#include "RegPos.h"

int main(){	
	/* teminal */
	srand(1);
	FILE* pfile;
	pfile = fopen("generation.txt", "w");
	fclose(pfile);
    Test test0;
    test0.terminal_loop(); // Need the MeTsystem member as input.
    return 0;
}
