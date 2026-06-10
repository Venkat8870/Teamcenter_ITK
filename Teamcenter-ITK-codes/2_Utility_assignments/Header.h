// Header.h - Common header file for Teamcenter ITK applications
// This file can be used for common function declarations and includes

#ifndef HEADER_H
#define HEADER_H

// Common includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Teamcenter ITK includes
#include <tc/tc_startup.h>
#include <tcinit/tcinit.h>
#include <tc/emh.h>
#include <tccore/item.h>
#include <bom/bom.h>
#include <tccore/aom_prop.h>
#include <tc/tc_util.h>

// Common function declarations
int Report_Error(int iFail);

#endif // HEADER_H