#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "input-helpers.h" 

#define FILE_NAME "flights.dat"

FILE* file_pointer;

void edit_action(flight flight_data, int flight_number, FILE** file_pointer)
{
	if (flight_data.direction.number == flight_number)
	{
		enter_flight(&flight_data);
	}
	fwrite(&flight_data, sizeof(flight), 1, *file_pointer);
}

void delete_action(flight flight_data, int flight_number, FILE** file_pointer)
{
	if (flight_data.direction.number != flight_number)
	{
		fwrite(&flight_data, sizeof(flight), 1, *file_pointer);
	}
}

void remove_infrequent_flights_action(flight flight_data, int min_flight_days_count, FILE** file_pointer)
{
	if (flight_data.direction.days_count >= min_flight_days_count)
	{
		fwrite(&flight_data, sizeof(flight), 1, *file_pointer);
	}
}

void file_change_action(void(*change_action)(flight, int, FILE**), const char* comparer_name)
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

	int comparer;
	enter_int(&comparer, comparer_name);

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

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);
	while (!feof(file_pointer))
	{
		printf("Flight number %d:\n", flight_data.direction.number);
		printf(">> Flying days: %s", flight_data.direction.days[0]);
		for (int i = 1; i < flight_data.direction.days_count; i++)
		{
			printf(", %s", flight_data.direction.days[i]);
		}
		printf("\n>> Plane type: %s\n", flight_data.plane.type);
		printf(">> Seats number: %d\n", flight_data.plane.seats);
		printf(">> Price: %d\n\n", flight_data.plane.price);

		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}
	fclose(file_pointer);
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

	char days[MAX_FLIGHT_DAYS][MAX_SIZE];
	int cheapest_flights[MAX_FLIGHT_DAYS];
	int min_prices[MAX_FLIGHT_DAYS];
	int days_count = 0;
	char answer;
	do
	{
		enter_string(&days[days_count], "flight day");
		cheapest_flights[days_count] = -1;
		min_prices[days_count] = INT_MAX;
		days_count++;

		if (days_count == MAX_FLIGHT_DAYS)
		{
			break;
		}

		printf("Enter 'y' if you want to add one more flight day, else 'n':\n");
		answer = getchar();
		getchar();

	} while (answer != 'n');

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);

	while (!feof(file_pointer))
	{
		for (int i = 0; i < flight_data.direction.days_count; i++)
		{
			for (int j = 0; j < days_count; j++)
			{
				if (!strcmp(days[j], flight_data.direction.days[i]) && flight_data.plane.price < min_prices[j])
				{
					cheapest_flights[j] = flight_data.direction.number;
					min_prices[j] = flight_data.plane.price;
				}
			}
		}
		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}
	fclose(file_pointer);

	printf("\nCheapest flights by days:\n");
	for (int i = 0; i < days_count; i++)
	{
		if (cheapest_flights[i] == -1)
		{
			printf("- %s: no flights available\n", days[i]);
		}
		else
		{
			printf("- %s: %d\n", days[i], cheapest_flights[i]);
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

	char days[MAX_FLIGHT_DAYS][MAX_SIZE];
	int seats_count[MAX_FLIGHT_DAYS];
	int days_count = 0;
	char answer;
	do
	{
		enter_string(&days[days_count], "flight day");
		seats_count[days_count] = 0;
		days_count++;

		if (days_count == MAX_FLIGHT_DAYS)
		{
			break;
		}

		printf("Enter 'y' if you want to add one more flight day, else 'n':\n");
		answer = getchar();
		getchar();

	} while (answer != 'n');

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);

	while (!feof(file_pointer))
	{
		for (int i = 0; i < flight_data.direction.days_count; i++)
		{
			for (int j = 0; j < days_count; j++)
			{
				if (!strcmp(days[j], flight_data.direction.days[i]))
				{
					seats_count[j] += flight_data.plane.seats;
				}
			}
		}
		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}
	fclose(file_pointer);

	printf("\nTotal seats by flight days:\n");
	for (int i = 0; i < days_count; i++)
	{
		printf("- %s: %d\n", days[i], seats_count[i]);
	}
}

void task3()
{
	file_change_action(remove_infrequent_flights_action, "minimum number of flight days");
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
		enter_int(&key, "menu item");
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