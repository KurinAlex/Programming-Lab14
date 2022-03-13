#pragma once

#include "structs.h"

void enter_short(short* number, const char* number_name, int tabs_count);
void enter_int(int* number, const char* number_name, int tabs_count);
void enter_string(char* s, const char* s_name, int tabs_count);
void enter_days(short* days, int tabs_count);
void enter_flight(flight* flight);