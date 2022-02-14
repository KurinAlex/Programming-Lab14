#pragma once

#define MAX_SIZE 100
#define MAX_FLIGHT_DAYS 7

typedef struct
{
	char days[MAX_FLIGHT_DAYS][MAX_SIZE];
	int number;
	int days_count;
} direction;

typedef struct
{
	char type[MAX_SIZE];
	int seats;
	int price;
} plane;

typedef struct
{
	direction direction;
	plane plane;
} flight;