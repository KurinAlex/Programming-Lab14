#include<stdio.h>
#include<stdlib.h>

#define FILE_NAME "flights.dat"
#define MAX_SIZE 100

typedef struct
{
	char days[7][MAX_SIZE];
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

FILE* file_pointer;

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
	char answer;
	int days_count = 0;
	do
	{
		enter_string(&flight->direction.days[days_count++], "flight day");

		if (days_count == 7)
		{
			break;
		}

		printf("Enter 'y' if you want to add ont more flight day, else 'n':\n");
		answer = getchar();
		getchar();
	} while (answer != 'n');
	flight->direction.days_count = days_count;
	enter_int(&flight->direction.number, "flight number");

	enter_string(&flight->plane.type, "plane type");
	enter_int(&flight->plane.seats, "seats number");
	enter_int(&flight->plane.price, "price");
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
		if (flight_data.direction.number == flight_number)
		{
			enter_flight(&flight_data);
		}
		fwrite(&flight_data, sizeof(flight), 1, tmp_file_pointer);
		fread(&flight_data, sizeof(flight), 1, file_pointer);
	}

	fclose(file_pointer);
	fclose(tmp_file_pointer);

	remove(FILE_NAME);
	rename(tmp_file_name, FILE_NAME);
}

void delete()
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
	enter_int(&flight_number, "number of flight to delete");

	flight flight_data;
	fread(&flight_data, sizeof(flight), 1, file_pointer);
	while (!feof(file_pointer))
	{
		if (flight_data.direction.number != flight_number)
		{
			fwrite(&flight_data, sizeof(flight), 1, tmp_file_pointer);
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
		printf(" MENU \n");
		printf("1. Output flights database\n");
		printf("2. Add data\n");
		printf("3. Edit data\n");
		printf("4. Delete data\n");
		printf("5. Exit\n");

		int key;
		printf("Choosen menu item:");
		scanf_s("%d", &key);
		getchar();
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
			add();
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