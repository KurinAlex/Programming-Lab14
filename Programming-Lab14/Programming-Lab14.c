#include<stdio.h>
#include<stdlib.h>

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

void file_change_action(void(*change_action)(flight, int, FILE**))
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

	int flight_number;
	enter_int(&flight_number, "number of flight to edit");

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);
	while (!feof(file_pointer))
	{
		change_action(flight_data, flight_number, &tmp_file_pointer);
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
	file_change_action(edit_action);
}

void delete()
{
	file_change_action(delete_action);
}

void main()
{
	while (1)
	{
		system("cls");
		printf(" MENU \n");
		printf("1. Output flights database\n");
		printf("2. Add data\n");
		printf("3. Edit data\n");
		printf("4. Delete data\n");
		printf("5. Exit\n");

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