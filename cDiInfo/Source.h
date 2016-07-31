/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Source.h - Main source header file
*/
#pragma once

// Local Includes
#include "Core.h"
#include "Enumerations.h"
#include "Features.h"
#include "Registry.h"
#include "Source.h"
#include "Strings.h"

// Prints usage information
void printUsage(int argc, char** argv);

// Main entry point
int main(int argc, char** argv);