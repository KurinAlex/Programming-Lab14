#include <stdio.h>

#include "input-helpers.h"
#include "output-helpers.h"
#include "math-helpers.h"

void enter_short(short* number, const char* number_name, int tabs_count)
{
	print_tabs(tabs_count);
	printf("Enter %s: ", number_name);
	scanf_s("%hd", number);
	getchar();
}

void enter_int(int* number, const char* number_name, int tabs_count)
{
	print_tabs(tabs_count);
	printf("Enter %s: ", number_name);
	scanf_s("%d", number);
	getchar();
}

void enter_string(char* s, const char* s_name, int tabs_count)
{
	print_tabs(tabs_count);
	printf("Enter %s: ", s_name);
	gets_s(s, MAX_SIZE);
}

void enter_days(short* days, int tabs_count)
{
	print_tabs(tabs_count);
	printf("Enter flight days:\n");

	char answer = 'y';
	short input;
	*days = 0;
	do
	{
		enter_short(&input, "number from 1 to 7 (1 for monday, 7 for sunday)", tabs_count + 1);

		if (input < 1 || input > 7)
		{
			print_tabs(tabs_count + 1);
			printf("Wrong input\n");
		}
		else
		{
			*days |= pow2(input - 1);

			do
			{
				print_tabs(tabs_count + 1);
				printf("Enter 'y' if you want to add one more flight day, else 'n': ");
				answer = getchar();
				getchar();
			} while (answer != 'n' && answer != 'y');
		}
	} while (answer != 'n');
}

void enter_flight(flight* flight)
{
	printf("Enter flight data:\n");
	enter_string(&flight->direction.number, "flight number", 1);
	enter_days(&flight->direction.days, 1);
	enter_string(&flight->plane.type, "plane type", 1);

	do
	{
		enter_int(&flight->plane.seats, "seats number", 1);
	} while (flight->plane.seats < 0);

	do
	{
		enter_int(&flight->plane.price, "price", 1);
	} while (flight->plane.price < 0);
}