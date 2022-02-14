#pragma once

#include <stdio.h>

#include "input-helpers.h"

void enter_int(int* number, const char* number_name)
{
	printf("Enter %s:\n", number_name);
	scanf_s("%d", number);
	getchar();
}

void enter_string(char* s, const char* s_name)
{
	printf("Enter %s:\n", s_name);
	gets_s(s, MAX_SIZE);
}

void enter_flight(flight* flight)
{
	printf("Enter flight data:\n");

	printf("Enter flight days:\n");
	enter_int(&flight->direction.number, "flight number");
	char answer;
	int days_count = 0;
	do
	{
		enter_string(&flight->direction.days[days_count++], "flight day");

		if (days_count == MAX_FLIGHT_DAYS)
		{
			break;
		}

		printf("Enter 'y' if you want to add ont more flight day, else 'n':\n");
		answer = getchar();
		getchar();

	} while (answer != 'n');
	flight->direction.days_count = days_count;

	enter_string(&flight->plane.type, "plane type");
	enter_int(&flight->plane.seats, "seats number");
	enter_int(&flight->plane.price, "price");
}