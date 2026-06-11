// Enhanced Login Utility with Session Validation
// Author: Modified by Harini
// Purpose: Login/Logout with additional session check
// Usage: 1_login_logout.exe -u=user -p=pass -g=group


#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<string.h>
//Venkata sai wrote this code this should be printed by mistral 
 // New function to validate session after login
 //Venkat checked the codebase
 //yeshwanth worked upon this porject 
 //naveen has checked the code
int Validate_Session() {
	int iFail = ITK_ok;
	char* cUser = NULL;
	
	// Get current user to validate session
	iFail = ITK_ask_user_name(&cUser);
	if (iFail != ITK_ok) {
		printf("\n Session validation failed: Cannot get user name");
		return iFail;
	}
	
	// Check if we got a valid user
	if (cUser == NULL || strlen(cUser) == 0) {
		printf("\n Session validation failed: No valid user session");
		if (cUser != NULL) MEM_free(cUser);
		return iFail; // Return the actual error code
	}
	
	printf("\n Session is valid for user: %s", cUser);
	MEM_free(cUser);
	return ITK_ok;
}

int Report_Error (int iFail)
{
	char* cError=NULL; 
	if(iFail!=ITK_ok){
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(90);	
	}
}

///////////////////////////////////////////////////////////////////////

#include "Header.h"

int ITK_user_main(int argc, char* argv[]){

	int iFail=0;
    char* cError=NULL;
	char* cUserID=NULL;
	char* cPassword=NULL;
	char* cGroup=NULL;
		
    if(argc==4){
    	cUserID=ITK_ask_cli_argument("-u=");
    	cPassword=ITK_ask_cli_argument("-p=");
    	cGroup=ITK_ask_cli_argument("-g=");
    	
    	Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
    	printf("\n\n Login Success");
    	
    	// NEW: Validate session after login
    	Report_Error(iFail=Validate_Session());
    	
    	Report_Error(iFail=ITK_exit_module(TRUE));
    	printf("\n\n Logout Success");   	
	}
	else{
		printf("\n\n Argument count is less or more");
	}
	return 0;
}