#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "input-helpers.h"
#include "output-helpers.h"

#define FILE_NAME "flights.dat"

FILE* file_pointer;

void edit_action(flight flight_data, const char* flight_number, FILE** file_pointer)
{
	if (!strcmp(flight_data.direction.number, flight_number))
	{
		enter_flight(&flight_data);
	}
	fwrite(&flight_data, sizeof(flight), 1, *file_pointer);
}

void delete_action(flight flight_data, const char* flight_number, FILE** file_pointer)
{
	if (strcmp(flight_data.direction.number, flight_number))
	{
		fwrite(&flight_data, sizeof(flight), 1, *file_pointer);
	}
}

void file_change_action(void(*change_action)(flight, const char*, FILE**), const char* comparer_name)
{
	fopen_s(&file_pointer, FILE_NAME, "r+b");
	if (!file_pointer)
	{
		printf("Database do not exist!\n");
		return;
	}

	const char* tmp_file_name = "temp.dat";
	FILE* tmp_file_pointer;
	fopen_s(&tmp_file_pointer, tmp_file_name, "wb");

	char comparer[MAX_SIZE];
	enter_string(&comparer, comparer_name, 0);

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);
	while (!feof(file_pointer))
	{
		change_action(flight_data, comparer, &tmp_file_pointer);
		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}

	fclose(file_pointer);
	fclose(tmp_file_pointer);

	remove(FILE_NAME);
	rename(tmp_file_name, FILE_NAME);
}

void output()
{
	fopen_s(&file_pointer, FILE_NAME, "rb");
	if (!file_pointer)
	{
		printf("Database do not exist!\n");
		return;
	}

	int is_empty = 1;
	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);
	while (!feof(file_pointer))
	{
		is_empty = 0;
		print_flight(flight_data);
		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}
	fclose(file_pointer);

	if (is_empty)
	{
		printf("Database is empty\n");
	}
}

void add()
{
	fopen_s(&file_pointer, FILE_NAME, "ab");

	flight flight_data;
	enter_flight(&flight_data);

	fwrite(&flight_data, sizeof(flight), 1, file_pointer);
	fclose(file_pointer);
}

void edit()
{
	file_change_action(edit_action, "number of flight to edit");
}

void delete()
{
	file_change_action(delete_action, "number of flight to delete");
}

void task1()
{
	fopen_s(&file_pointer, FILE_NAME, "rb");
	if (!file_pointer)
	{
		printf("Database do not exist!\n");
		return;
	}

	short days;
	enter_days(&days, 0);

	char cheapest_flights[MAX_FLIGHT_DAYS][MAX_SIZE];
	int min_prices[MAX_FLIGHT_DAYS];
	for (int i = 0; i < MAX_FLIGHT_DAYS; i++)
	{
		min_prices[i] = INT_MAX;
	}

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);
	while (!feof(file_pointer))
	{
		short common_days = flight_data.direction.days & days;
		for (int day = 0; day < MAX_FLIGHT_DAYS && common_days; day++, common_days >>= 1)
		{
			if ((common_days & 1) && flight_data.plane.price < min_prices[day])
			{
				strcpy_s(cheapest_flights[day], MAX_SIZE, flight_data.direction.number);
				min_prices[day] = flight_data.plane.price;
			}
		}

		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}
	fclose(file_pointer);

	printf("\nCheapest flights by days:\n");
	for (int day = 0; day < MAX_FLIGHT_DAYS && days; day++, days >>= 1)
	{
		if (days & 1)
		{
			if (min_prices[day] == INT_MAX)
			{
				printf("- %s: no flights available\n", week_days[day]);
			}
			else
			{
				printf("- %s: %s with price %d\n", week_days[day], cheapest_flights[day], min_prices[day]);
			}
		}
	}
}

void task2() //I need some help with storage of plane types
{
	fopen_s(&file_pointer, FILE_NAME, "rb");
	if (!file_pointer)
	{
		printf("Database do not exist!\n");
		return;
	}

	short days;
	enter_days(&days, 0);

	int seats_count[MAX_FLIGHT_DAYS];
	for (int i = 0; i < MAX_FLIGHT_DAYS; i++)
	{
		seats_count[i] = 0;
	}

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);

	while (!feof(file_pointer))
	{
		short common_days = flight_data.direction.days & days;
		for (int day = 0; day < MAX_FLIGHT_DAYS && common_days; day++, common_days >>= 1)
		{
			if (common_days & 1)
			{
				seats_count[day] += flight_data.plane.seats;
			}
		}

		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}
	fclose(file_pointer);

	printf("\nTotal seats by flight days:\n");
	for (int day = 0; day < MAX_FLIGHT_DAYS && days; day++, days >>= 1)
	{
		if (days & 1)
		{
			printf("- %s: %d\n", week_days[day], seats_count[day]);
		}
	}
}

void task3()
{
	fopen_s(&file_pointer, FILE_NAME, "r+b");
	if (!file_pointer)
	{
		printf("Database do not exist!\n");
		return;
	}

	const char* tmp_file_name = "temp.dat";
	FILE* tmp_file_pointer;
	fopen_s(&tmp_file_pointer, tmp_file_name, "wb");

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);
	while (!feof(file_pointer))
	{
		int days = flight_data.direction.days;
		int days_count = 0;
		for (int i = 0; i < MAX_FLIGHT_DAYS && days; i++, days >>= 1)
		{
			if (days & 1)
			{
				days_count++;
			}
		}

		if (days_count >= 2)
		{
			fwrite(&tmp_file_pointer, sizeof(flight), 1, file_pointer);
		}
		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}

	fclose(file_pointer);
	fclose(tmp_file_pointer);

	remove(FILE_NAME);
	rename(tmp_file_name, FILE_NAME);
}

void main()
{
	while (1)
	{
		system("cls");

		printf("   MENU\n");
		printf("1. Output flights database\n");
		printf("2. Add data\n");
		printf("3. Edit data\n");
		printf("4. Delete data\n");
		printf("5. Task 1\n");
		printf("6. Task 2\n");
		printf("7. Task 3\n");
		printf("8. Exit\n\n");

		int key;
		enter_int(&key, "menu item", 0);
		system("cls");

		switch (key)
		{
		case 1:
			output();
			break;
		case 2:
			add();
			break;
		case 3:
			edit();
			break;
		case 4:
			delete();
			break;
		case 5:
			task1();
			break;
		case 6:
			task2();
			break;
		case 7:
			task3();
			break;
		case 8:
			exit(0);
			break;
		default:
			printf("Try one more time...\n");
			break;
		}

		printf("Press any key...\n");
		getchar();
	}
}