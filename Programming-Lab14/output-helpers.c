#include <stdio.h>

#include "output-helpers.h"

const char week_days[MAX_FLIGHT_DAYS][MAX_SIZE] = { "monday","tuesday","wednesday","thursday","friday","saturday","sunday" };

void print_tabs(int tab_count)
{
	for (int i = 0; i < tab_count; i++)
	{
		putchar('\t');
	}
}

void print_days(short days)
{
	int is_first = 1;
	for (int i = 0; i < MAX_FLIGHT_DAYS; i++, days >>= 1)
	{
		if (days & 1)
		{
			if (!is_first)
			{
				printf(", ");
			}
			printf("%s", week_days[i]);
			is_first = 0;
		}
	}
	putchar('\n');
}

void print_flight(flight flight)
{
	printf("Flight number %s:\n", flight.direction.number);
	printf(">> Flight days: ");
	print_days(flight.direction.days);
	printf(">> Plane type: %s\n", flight.plane.type);
	printf(">> Seats number: %d\n", flight.plane.seats);
	printf(">> Price: %d\n\n", flight.plane.price);
}