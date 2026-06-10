// Customer Story: Login to Teamcenter, Create Item, and Create BOM
// Purpose: Complete workflow from authentication to BOM structure creation
// Author: Generated based on INDEX.md patterns
// Dependencies: tcinit, tccore/item, bom/bom

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<bom/bom.h>

int Report_Error(int iFail) {
    char* cError = NULL;
    if (iFail != ITK_ok) {
        EMH_ask_error_text(iFail, &cError);
        printf("\n\n Error is : %s", cError);
        MEM_free(cError);
        exit(0);
    }
    return 0;
}

int ITK_user_main(int argc, char* argv[]) {
    
    int iFail = 0;
    char* cUserID = NULL;
    char* cPassword = NULL;
    char* cGroup = NULL;
    char* cItemID = NULL;
    char* cItemName = NULL;
    
    tag_t tItem = NULLTAG;
    tag_t tRev = NULLTAG;
    tag_t tWindow = NULLTAG;
    tag_t tBomLine = NULLTAG;

    if (argc == 6) {
        cUserID = ITK_ask_cli_argument("-u=");
        cPassword = ITK_ask_cli_argument("-p=");
        cGroup = ITK_ask_cli_argument("-g=");
        cItemID = ITK_ask_cli_argument("-id=");
        cItemName = ITK_ask_cli_argument("-name=");

        // Step 1: Login (from 1_login_logout.cpp pattern)
        Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
        printf("\n\n Login Success");

        // Step 2: Create Item (from 2_Create_Item.cpp pattern)
        Report_Error(iFail = ITEM_create_item(cItemID, cItemName, "Item", "", &tItem, &tRev));
        printf("\n\n Item created");

        Report_Error(iFail = ITEM_save_item(tItem));
        printf("\n\n Item Saved");

        // Step 3: Create BOM Window (from 16_print_BOM_in_CSVfile.cpp pattern)
        Report_Error(iFail = BOM_create_window(&tWindow));
        printf("\n\n BOM Window Created");

        Report_Error(iFail = BOM_set_window_top_line(tWindow, tItem, tRev, NULLTAG, &tBomLine));
        printf("\n\n Top BOM Line Created - BOM Object Ready");

        // Cleanup
        Report_Error(iFail = BOM_close_window(tWindow));
        printf("\n\n BOM Window Closed");

        // Step 4: Logout
        Report_Error(iFail = ITK_exit_module(TRUE));
        printf("\n\n Logout Success");
    }
    else {
        printf("\n\n Usage: create_item_with_bom.exe -u=user -p=pass -g=group -id=item_id -name=item_name");
    }
    return 0;
}
