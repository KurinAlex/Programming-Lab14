#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "input-helpers.h"
#include "output-helpers.h"

#define FILE_NAME "flights.dat"
#define TMP_FILE_NAME "tmp.dat"

FILE* file_pointer;

int file_exists()
{
	fopen_s(&file_pointer, FILE_NAME, "rb");
	if (!file_pointer)
	{
		printf("Database does not exist!\n");
		return 0;
	}
	return 1;
}

int edit_action(flight flight_data, const char* flight_number, FILE** file_pointer)
{
	int is_changed = 0;
	if (!strcmp(flight_data.direction.number, flight_number))
	{
		enter_flight(&flight_data);
		is_changed = 1;
		printf("Flight data successfully edited\n");
	}
	fwrite(&flight_data, sizeof(flight), 1, *file_pointer);
	return is_changed;
}

int delete_action(flight flight_data, const char* flight_number, FILE** file_pointer)
{
	if (strcmp(flight_data.direction.number, flight_number))
	{
		fwrite(&flight_data, sizeof(flight), 1, *file_pointer);
		return 0;
	}
	printf("Flight data successfully deleted\n");
	return 1;
}

void file_change_action(int(*change_action)(flight, const char*, FILE**), const char* comparer_name)
{
	if (!file_exists())
	{
		return;
	}

	FILE* tmp_file_pointer;
	fopen_s(&tmp_file_pointer, TMP_FILE_NAME, "wb");

	char comparer[MAX_SIZE];
	enter_string(&comparer, comparer_name, 0);

	int is_changed = 0;

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);
	while (!feof(file_pointer))
	{
		if (change_action(flight_data, comparer, &tmp_file_pointer))
		{
			is_changed = 1;
		}
		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}

	if (!is_changed)
	{
		printf("This flight is not in the database\n");
	}

	fclose(file_pointer);
	fclose(tmp_file_pointer);

	remove(FILE_NAME);
	rename(TMP_FILE_NAME, FILE_NAME);
}

void output()
{
	if (!file_exists())
	{
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
	if (!file_exists())
	{
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

void task2()
{
	if (!file_exists())
	{
		return;
	}

	short days;
	enter_days(&days, 0);

	char plane_types[MAX_FLIGHT_DAYS][MAX_SIZE][MAX_SIZE];
	int types_count[MAX_FLIGHT_DAYS];
	int seats_count[MAX_FLIGHT_DAYS];
	for (int i = 0; i < MAX_FLIGHT_DAYS; i++)
	{
		types_count[i] = 0;
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

				int in_list = 0;
				for (int i = 0; i < types_count[day]; i++)
				{
					if (!strcmp(flight_data.plane.type, plane_types[day][i]))
					{
						in_list = 1;
						break;
					}
				}
				if (!in_list)
				{
					strcpy_s(plane_types[day][types_count[day]++], MAX_SIZE, flight_data.plane.type);
				}
			}
		}

		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}
	fclose(file_pointer);

	printf("\nTotal seats and plane types by flight days:\n");
	for (int day = 0; day < MAX_FLIGHT_DAYS && days; day++, days >>= 1)
	{
		if (days & 1)
		{
			printf("%s:\n", week_days[day]);
			if (types_count[day])
			{
				printf("- seats count: %d\n", seats_count[day]);
				printf("- plane types: %s", plane_types[day][0]);
				for (int i = 1; i < types_count[day]; i++)
				{
					printf(", %s", plane_types[day][i]);
				}
				printf("\n");
			}
			else
			{
				printf("- no flights available\n");
			}
		}
	}
}

void task3()
{
	if (!file_exists())
	{
		return;
	}

	FILE* tmp_file_pointer;
	fopen_s(&tmp_file_pointer, TMP_FILE_NAME, "wb");

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
			fwrite(&flight_data, sizeof(flight), 1, tmp_file_pointer);
		}
		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}

	printf("Task successfully completed\n");

	fclose(file_pointer);
	fclose(tmp_file_pointer);

	remove(FILE_NAME);
	rename(TMP_FILE_NAME, FILE_NAME);
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
			printf("Try one more time...");
			break;
		}

		printf("\nPress any key...");
		getchar();
	}
}