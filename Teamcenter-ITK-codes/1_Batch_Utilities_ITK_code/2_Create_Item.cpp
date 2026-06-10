#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>

/**
 * Reports errors and exits the program if an error occurs.
 * 
 * @param iFail Error code to check
 * @return void
 */
int Report_Error(int iFail){
	char* cError=NULL;
	if(iFail!=ITK_ok){
		EMH_ask_error_text(iFail,&cError);
		printf("\n\n Error is : %s",cError);
		MEM_free(cError);
		exit(0);
	}
}

////////////////////////////////////////////////////////////
#include "header.h"

/**
 * Main function to create an item in Teamcenter using ITK.
 * 
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return 0 on success
 * 
 * Usage: 2_Create_Item -u=<username> -p=<password> -g=<group> -id=<item_id> -name=<item_name>
 */
int ITK_user_main(int argc, char* argv[]){
	
	int iFail=0;            /**< Error code */
	char* cError=NULL;      /**< Error message buffer */
	char* cUserID=NULL;     /**< Teamcenter username */
	char* cPassword=NULL;   /**< Teamcenter password */
	char* cGroup=NULL;      /**< Teamcenter group */
	char* cItemID=NULL;     /**< ID for the new item */
	char* cItemName=NULL;   /**< Name for the new item */
	tag_t tItem=NULLTAG;    /**< Tag for the created item */
	tag_t tRev=NULLTAG;     /**< Tag for the item revision */
	
	/** Check if correct number of arguments are provided */
	if(argc==6){
		/** Extract command-line arguments */
		cUserID=ITK_ask_cli_argument("-u=");
		cPassword=ITK_ask_cli_argument("-p=");
		cGroup=ITK_ask_cli_argument("-g=");
		cItemID=ITK_ask_cli_argument("-id=");
		cItemName=ITK_ask_cli_argument("-name=");
		
		/** Initialize Teamcenter session */
		Report_Error(iFail=ITK_init_module(cUserID,cPassword,cGroup));
		printf("\n\n login Success");
		
		/** Create new item in Teamcenter */
		Report_Error(iFail=ITEM_create_item(cItemID,cItemName,"Item","",&tItem,&tRev));
		printf("\n\n Item created ");
		
		/** Save the created item */
		Report_Error(iFail=ITEM_save_item(tItem));
		printf("\n\n Item Saved ");
		
		/** Exit Teamcenter session */
		Report_Error(iFail=ITK_exit_module(TRUE));
		printf("\n\n logout Success");
		
	//venkata sai commited this code	
	}else{
		/** Error message for incorrect argument count */
		printf("\n\n Argument count is less or more");
	}
	return 0;
}