// print bom structure in PDF file

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<bom/bom.h>
#include<tccore/aom_prop.h>
#include<tc/tc_util.h>

FILE* file;

int Report_Error(int iFail)
{
	char* cError = NULL;
	if (iFail != ITK_ok) {
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
}

//////////////////////////////////////////////////////////////////////////
#include "Header.h"

// Function to generate HTML content for PDF conversion
void generate_html_bom_report(const char* filename, const char* html_content) {
    FILE* html_file = TC_fopen(filename, "w+");
    if (html_file != NULL) {
        TC_fprintf(html_file, "%s", html_content);
        fflush(html_file);
        fclose(html_file);
    }
}

int ITK_user_main(int argc, char* argv[])
{

	int iFail = 0;
	int iNumberOfchild = 0;
	int iRevCount = 0;
	int i = 0, j = 0, k = 0;
	char* cError = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGrp = NULL;
	char* cItem_id = NULL;
	char* cObject_Name = NULL;
	char* cItemID = NULL;
	char* cRevision_ID = NULL;
	char* cAssembly_Level = NULL;
	char* cParent_Assembly = NULL;
	char* cOwner = NULL;
	char* cGroup = NULL;

	tag_t tItem = NULLTAG;
	tag_t* tRevList = NULLTAG;
	tag_t tWindow = NULLTAG;
	tag_t tBomLine = NULLTAG;
	tag_t* tBomChildren = NULLTAG;
	tag_t tChild = NULLTAG;
	tag_t* tSubChilds = NULLTAG;

	if (argc == 5)
	{
		cUserID = ITK_ask_cli_argument("-u=");
		cPassword = ITK_ask_cli_argument("-p=");
		cGrp = ITK_ask_cli_argument("-g=");
		cItem_id = ITK_ask_cli_argument("-id=");

		if (cItem_id != NULL)
		{
			Report_Error(iFail = ITK_init_module(cUserID, cPassword, cGroup));
			printf("\n\n Login Success");

			// Start building HTML content for PDF
			char html_content[65535] = "";
			strcat(html_content, "<html><head><title>BOM Report</title></head><body>");
			strcat(html_content, "<h1 style='text-align:center;'>Bill of Materials Report</h1>");
			strcat(html_content, "<table border='1' cellpadding='5' cellspacing='0' style='width:100%; border-collapse:collapse;'>");
			strcat(html_content, "<tr style='background-color:#f2f2f2;'><th>Object Name</th><th>Item ID</th><th>Revision ID</th><th>Assembly Level</th><th>Parent Assembly</th><th>Owner</th><th>Group</th></tr>");

			Report_Error(ITEM_find_item(cItem_id, &tItem));
			if (tItem != NULLTAG)
			{
				printf("\n\n Item found success");
				Report_Error(ITEM_list_all_revs(tItem, &iRevCount, &tRevList));
				if (tRevList != NULL)
				{
					printf("\n\n Item Revisons found success");
					for (i = 0; i < iRevCount; i++)
					{
						Report_Error(BOM_create_window(&tWindow));
						Report_Error(BOM_set_window_top_line(tWindow, tItem, tRevList[i], NULLTAG, &tBomLine));

						Report_Error(AOM_UIF_ask_value(tBomLine,"bl_rev_object_name",&cObject_Name));
						Report_Error(AOM_UIF_ask_value(tBomLine,"bl_item_id", &cItemID));
						Report_Error(AOM_UIF_ask_value(tBomLine,"bl_item_revision_id",&cRevision_ID));
						Report_Error(AOM_UIF_ask_value(tBomLine,"bl_level_starting_0", &cAssembly_Level));
						Report_Error(AOM_UIF_ask_value(tBomLine,"bl_formatted_parent_name",&cParent_Assembly));
						Report_Error(AOM_UIF_ask_value(tBomLine,"bl_rev_owning_user",&cOwner));
						Report_Error(AOM_UIF_ask_value(tBomLine,"bl_rev_ownning_group",&cGroup));

						// Add row to HTML table
						char row[2048];
						sprintf(row, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>",
							cObject_Name, cItemID, cRevision_ID, cAssembly_Level, cParent_Assembly, cOwner, cGroup);
						strcat(html_content, row);

						Report_Error(BOM_line_ask_all_child_lines(tBomLine, &iNumberOfchild, &tBomChildren));
						if (iNumberOfchild > 0)
						{
							for(j=0; j< iNumberOfchild; j++)
							{
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_object_name", &cObject_Name));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_item_id", &cItemID));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_item_revision_id", &cRevision_ID));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_level_starting_0", &cAssembly_Level));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_formatted_parent_name", &cParent_Assembly));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_owning_user", &cOwner));
								Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_ownning_group", &cGroup));

								// Add child row to HTML table
								sprintf(row, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>",
									cObject_Name, cItemID, cRevision_ID, cAssembly_Level, cParent_Assembly, cOwner, cGroup);
								strcat(html_content, row);
							}
						}

					}

				}
				if (tRevList)
				{
					MEM_free(tRevList);
				}
				if (tBomChildren)
				{
					MEM_free(tBomChildren);
				}
				BOM_close_window(tWindow);

				// Close HTML table and body
				strcat(html_content, "</table>");
				strcat(html_content, "</body></html>");

				// Generate HTML file that can be converted to PDF
				generate_html_bom_report("BOM_REPORT.html", html_content);
				printf("\n\n HTML BOM report generated: BOM_REPORT.html");
				printf("\n\n To convert to PDF, use a tool like wkhtmltopdf: wkhtmltopdf BOM_REPORT.html BOM_REPORT.pdf");

				Report_Error(iFail = ITK_exit_module(TRUE));
				printf("\n\n Logout Success");
			}
		}
		else
		{
			printf("\n\n Argument count is less or more");
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//to print Bom with children

// print bom structure in PDF file

#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<stdlib.h>
#include<tccore/item.h>
#include<bom/bom.h>
#include<tccore/aom_prop.h>
#include<tc/tc_util.h>

FILE* file;
FILE* fp;

////////////////////////////////////////////////////////////////////////////////////////////

#include "Header.h"

// Recursive function to process BOM child lines and add to HTML
void bom_sub_child_html(tag_t* tBomChildren, int iNumOfChild, char* html_content) {
    int j = 0;
    int iSubChildCount = 0;
    char* cObjectName = NULL;
    char* cItemId = NULL;
    char* cRevisionId = NULL;
    char* cAssemblyLevel = NULL;
    char* cParentAssembly = NULL;
    char* cOwner = NULL;
    char* cGroup = NULL;
    tag_t* tSubChilds = NULLTAG;
    tag_t tChildWindow = NULLTAG;
    tag_t tSubBomLine = NULLTAG;
    char row[2048];

    for (j = 0; j < iNumOfChild; j++) {
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_object_name", &cObjectName));
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_item_item_id", &cItemId));
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_item_revision_id", &cRevisionId));
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_level_starting_0", &cAssemblyLevel));
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_formatted_parent_name", &cParentAssembly));
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_owning_user", &cOwner));
        Report_Error(AOM_UIF_ask_value(tBomChildren[j], "bl_rev_ownning_group", &cGroup));

        // Add row to HTML table
        sprintf(row, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>",
               cObjectName, cItemId, cRevisionId,
               cAssemblyLevel, cParentAssembly, cOwner, cGroup);
        strcat(html_content, row);

        // Recursively process children of this child
        Report_Error(BOM_create_window(&tChildWindow));
        Report_Error(BOM_set_window_top_line(tChildWindow, NULLTAG, tBomChildren[j], NULLTAG, &tSubBomLine));
        Report_Error(BOM_line_ask_all_child_lines(tSubBomLine, &iSubChildCount, &tSubChilds));

        if (iSubChildCount > 0) {
            bom_sub_child_html(tSubChilds, iSubChildCount, html_content);
        }

        if (tSubChilds) MEM_free(tSubChilds);
        BOM_close_window(tChildWindow);

        // Free allocated strings
        if (cObjectName) { MEM_free(cObjectName); cObjectName = NULL; }
        if (cItemId) { MEM_free(cItemId); cItemId = NULL; }
        if (cRevisionId) { MEM_free(cRevisionId); cRevisionId = NULL; }
        if (cAssemblyLevel) { MEM_free(cAssemblyLevel); cAssemblyLevel = NULL; }
        if (cParentAssembly) { MEM_free(cParentAssembly); cParentAssembly = NULL; }
        if (cOwner) { MEM_free(cOwner); cOwner = NULL; }
        if (cGroup) { MEM_free(cGroup); cGroup = NULL; }
    }
}

int ITK_user_main(int argc, char* argv[]) {
    char* cUser = ITK_ask_cli_argument("-u=");
    char* cPass = ITK_ask_cli_argument("-p=");
    char* cGroup = ITK_ask_cli_argument("-g=");
    char* cItem = ITK_ask_cli_argument("-item=");

    int iRevCount = 0;
    int iNumOfChild = 0;
    char* cObjectName = NULL;
    char* cItemId = NULL;
    tag_t tItem = NULLTAG;
    tag_t* tRevList = NULLTAG;
    tag_t tWindow = NULLTAG;
    tag_t tBomLine = NULLTAG;
    tag_t* tBomChildren = NULLTAG;

    char html_content[65535] = "";
    strcat(html_content, "<html><head><title>Detailed BOM Report</title></head><body>");
    strcat(html_content, "<h1 style='text-align:center;'>Detailed Bill of Materials Report</h1>");
    strcat(html_content, "<table border='1' cellpadding='5' cellspacing='0' style='width:100%; border-collapse:collapse;'>");
    strcat(html_content, "<tr style='background-color:#f2f2f2;'><th>Object Name</th><th>Item Id</th><th>Revision ID</th><th>Assembly Level</th><th>Parent Assembly</th><th>Owner</th><th>Group</th></tr>");

    if (cUser && cPass && cGroup) {
        if (cItem != NULL) {
            Report_Error(ITK_init_module(cUser, cPass, cGroup));
            printf("\n Login successful");

            Report_Error(ITEM_find_item(cItem, &tItem));
            if (tItem != NULLTAG) {
                printf("\n Item found successful");

                Report_Error(ITEM_list_all_revs(tItem, &iRevCount, &tRevList));
                if (tRevList != NULLTAG) {
                    printf("\n Revision list found successful");

                    for (int i = 0; i < iRevCount; i++) {
                        Report_Error(BOM_create_window(&tWindow));
                        Report_Error(BOM_set_window_top_line(tWindow, tItem, tRevList[i], NULLTAG, &tBomLine));

                        Report_Error(AOM_UIF_ask_value(tBomLine, "bl_rev_object_name", &cObjectName));
                        Report_Error(AOM_UIF_ask_value(tBomLine, "bl_item_item_id", &cItemId));

                        char row[2048];
                        sprintf(row, "<tr><td>%s</td><td>%s</td><td>-</td><td>0</td><td>-</td><td>-</td><td>-</td></tr>",
                                cObjectName, cItemId);
                        strcat(html_content, row);

                        printf("\n %s,%s", cObjectName, cItemId);
                        Report_Error(BOM_line_ask_all_child_lines(tBomLine, &iNumOfChild, &tBomChildren));
                        if (iNumOfChild > 0) {
                            bom_sub_child_html(tBomChildren, iNumOfChild, html_content);
                        }
                        BOM_close_window(tWindow);
                    }
                    if (tRevList) {
                        MEM_free(tRevList);
                    }
                }
            }
            ITK_exit_module(ITK_ok);
            printf("\n Logout successfull");
        }
        else {
            printf("\n File not created");
            exit(0);
        }
    }
    else {
        printf("\n Enter the credentials properly");
    }

    // Close HTML table and body
    strcat(html_content, "</table>");
    strcat(html_content, "</body></html>");

    // Generate HTML file that can be converted to PDF
    generate_html_bom_report("DETAILED_BOM_REPORT.html", html_content);
    printf("\n\n HTML BOM report generated: DETAILED_BOM_REPORT.html");
    printf("\n\n To convert to PDF, use: wkhtmltopdf DETAILED_BOM_REPORT.html DETAILED_BOM_REPORT.pdf");

    return 0;
}