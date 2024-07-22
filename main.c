#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_SCHEDULE_PER_DAY 6
#define CINEMA_COUNT 2

struct Seat
{
    char *seatNumber;
    int isReserved;
};

struct Schedule
{
    struct Seat *seats[5][10];
    char time[100];
};

struct Movie
{
    char title[100];
    char description[1024];
    int duration;
    struct Schedule schedule[MAX_SCHEDULE_PER_DAY];
};

struct Cinema
{
    char name[100];
    struct Movie movies[100];
};

struct Cinema cinemas[2];
struct Cinema empty[2];

void initializeCinemas();

void searchMovie();

void showSchedule();

void loadSchedule();

void loadScheduleFromFile();

int getMovieIndex(struct Cinema *cinema, const char *title);

void initializeSeats(struct Schedule *schedule);

void printSeats(struct Schedule *schedule);

void reserveSeat();

int countReservedSeats(struct Schedule *schedule);

void printTickets();

void onExit();

int verifySchedule();

void showMenu();

int main()
{
    initializeCinemas();
    loadSchedule();

    // Loop to show the menu
    while (1)
    {
        showMenu();
    }
}

void showMenu()
{
    printf("====================================\n");
    printf("1. Reserve Seat\n");
    printf("2. View Schedule\n");
    printf("3. Search Movie\n");
    printf("4. Print Tickets\n");
    printf("5. Load Schedule From File\n");
    printf("6. Exit\n");
    printf("====================================\n\n\n");

    int choice;

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {

        reserveSeat();
        break;
    }

    case 2:
    {
        showSchedule();
        break;
    }

    case 3:
    {
        searchMovie();
        break;
    }
    case 4:
    {

        printTickets();
        break;
    }

    case 5:
    {
        loadScheduleFromFile();
        break;
    }
    case 6:
    {
        onExit();
        exit(0);
    }
    }
}

void loadSchedule()
{
    FILE *file;

    char *filename = "MovieSched.txt";
    char line[1024];
    int cinemaIndex = 0;
    int movieIndex = 0;
    int scheduleIndex = 0;
    int lineCounter = 0;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s", filename);
    }
    while (fgets(line, sizeof(line), file))
    {
        // check if line is empty

        if (line[0] == '\n')
        {

            continue;
        }

        else if (isdigit(line[0]) && strstr(line, ":") == NULL && strstr(line, "min") == NULL)
        {
            if (cinemaIndex == atoi(line))
            {

                movieIndex++;
            }
            else
            {
                cinemaIndex = atoi(line);
                movieIndex = 0;
            }
            scheduleIndex = 0;
            lineCounter = 1;

            continue;
        }

        else if (lineCounter == 1)
        {

            line[strcspn(line, "\n")] = '\0';

            strcpy(cinemas[cinemaIndex - 1].movies[movieIndex].title, line);
            lineCounter++;
            continue;
        }
        else if (lineCounter == 2)
        {
            strcpy(cinemas[cinemaIndex - 1].movies[movieIndex].description, line);
            lineCounter++;
            continue;
        }
        else if (lineCounter == 3)
        {
            cinemas[cinemaIndex - 1].movies[movieIndex].duration = atoi(line);
            lineCounter++;
            continue;
        }

        else if (strstr(line, ":") != NULL)
        {
            line[strcspn(line, "\n")] = '\0';
            strcpy(cinemas[cinemaIndex - 1].movies[movieIndex].schedule[scheduleIndex].time, line);
            initializeSeats(&cinemas[cinemaIndex - 1].movies[movieIndex].schedule[scheduleIndex]);
            scheduleIndex++;
            lineCounter++;
            continue;
        }
    }
    fclose(file);

    if (verifySchedule())
    {
        printf("Schedule is valid.\n");
    }
    else
    {
        printf("Schedule is invalid. Modify the schedule file and run the program again.\n");
        exit(0);
    }
}

void loadScheduleFromFile()
{
    FILE *file;
    char filename[100];

    char line[1024];
    int cinemaIndex = 0;
    int movieIndex = 0;
    int scheduleIndex = 0;
    int lineCounter = 0;

    printf("Enter the filename: ");
    scanf(" %[^\n]", filename);

    // Clear cinemas
    memset(cinemas, 0, sizeof(cinemas));

    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s", filename);
    }
    while (fgets(line, sizeof(line), file))
    {
        // check if line is empty
        if (line[0] == '\n')
        {
            continue;
        }
        else if (isdigit(line[0]) && strstr(line, ":") == NULL && strstr(line, "min") == NULL)
        {
            if (cinemaIndex == atoi(line))
            {
                movieIndex++;
            }
            else
            {
                cinemaIndex = atoi(line);
                movieIndex = 0;
            }
            scheduleIndex = 0;
            lineCounter = 1;
            continue;
        }
        else if (lineCounter == 1)
        {
            line[strcspn(line, "\n")] = '\0';
            strcpy(cinemas[cinemaIndex - 1].movies[movieIndex].title, line);
            lineCounter++;
            continue;
        }
        else if (lineCounter == 2)
        {
            strcpy(cinemas[cinemaIndex - 1].movies[movieIndex].description, line);
            lineCounter++;
            continue;
        }
        else if (lineCounter == 3)
        {
            cinemas[cinemaIndex - 1].movies[movieIndex].duration = atoi(line);
            lineCounter++;
            continue;
        }
        else if (strstr(line, ":") != NULL)
        {
            line[strcspn(line, "\n")] = '\0';
            strcpy(cinemas[cinemaIndex - 1].movies[movieIndex].schedule[scheduleIndex].time, line);
            initializeSeats(&cinemas[cinemaIndex - 1].movies[movieIndex].schedule[scheduleIndex]);
            scheduleIndex++;
            lineCounter++;
            continue;
        }
    }
    fclose(file);

    if (verifySchedule())
    {
        printf("Schedule is valid.\n");
    }
    else
    {
        printf("Schedule is invalid. Modify the schedule file and run the program again.\n");
        exit(0);
    }
}

int getMovieIndex(struct Cinema *cinema, const char *title)
{
    for (int i = 0; i < sizeof(cinema->movies) / sizeof(cinema->movies[0]); i++)
    {
        if (strcmp(cinema->movies[i].title, title) == 0)
        {
            return i;
        }
    }
    return -1;
}

void showSchedule()
{
    for (int i = 0; i < CINEMA_COUNT; i++)
    {
        printf("Cinema: %s\n", cinemas[i].name);
        for (int j = 0; j < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); j++)
        {
            if (cinemas[i].movies[j].title[0] == '\0')
            {
                break;
            }
            printf("Movie: %s\n", cinemas[i].movies[j].title);
            printf("Description: %s\n", cinemas[i].movies[j].description);
            printf("Duration: %d minutes\n", cinemas[i].movies[j].duration);
            for (int k = 0; k < MAX_SCHEDULE_PER_DAY; k++)
            {
                if (cinemas[i].movies[j].schedule[k].time[0] == '\0')
                {
                    break;
                }
                printf("Schedule %d: %s\n", k + 1, cinemas[i].movies[j].schedule[k].time);
                printSeats(&cinemas[i].movies[j].schedule[k]);
            }
        }
    }
}

void initializeCinemas()
{
    strcpy(cinemas[0].name, "Cinema 1");
    strcpy(cinemas[1].name, "Cinema 2");
}

void searchMovie()
{
    int choice;
    printf("Search by:\n");
    printf("1. Title\n");
    printf("2. Time\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        char searchTitle[100];
        printf("Enter movie title: ");
        scanf(" %[^\n]", searchTitle);

        for (int i = 0; i < CINEMA_COUNT; i++)
        {
            for (int j = 0; j < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); j++)
            {
                if (strcmp(cinemas[i].movies[j].title, searchTitle) == 0)
                {
                    printf("Cinema: %s\n", cinemas[i].name);
                    printf("Movie: %s\n", cinemas[i].movies[j].title);
                    printf("Description: %s\n", cinemas[i].movies[j].description);
                    printf("Duration: %d minutes\n", cinemas[i].movies[j].duration);
                    for (int k = 0; k < MAX_SCHEDULE_PER_DAY; k++)
                    {
                        if (cinemas[i].movies[j].schedule[k].time[0] == '\0')
                        {
                            break;
                        }
                        printf("+-------------------------------------------------------------+\n");
                        printf("Schedule %d: %s\n", k + 1, cinemas[i].movies[j].schedule[k].time);
                        printSeats(&cinemas[i].movies[j].schedule[k]);
                        printf("+-------------------------------------------------------------+\n");
                    }
                }
            }
        }
        break;
    }

    case 2:
    {
        char searchTime[100];
        printf("Enter movie time (HH:MM): ");
        scanf(" %[^\n]", searchTime);

        for (int i = 0; i < CINEMA_COUNT; i++)
        {
            for (int j = 0; j < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); j++)
            {
                for (int k = 0; k < MAX_SCHEDULE_PER_DAY; k++)
                {
                    if (cinemas[i].movies[j].schedule[k].time[0] == '\0')
                    {
                        break;
                    }
                    if (strcmp(cinemas[i].movies[j].schedule[k].time, searchTime) == 0)
                    {
                        printf("Cinema: %s\n", cinemas[i].name);
                        printf("Movie: %s\n", cinemas[i].movies[j].title);
                        printf("Description: %s\n", cinemas[i].movies[j].description);
                        printf("Duration: %d minutes\n", cinemas[i].movies[j].duration);
                        printf("+-------------------------------------------------------------+\n");
                        printf("Schedule: %s\n", cinemas[i].movies[j].schedule[k].time);
                        printf("+-------------------------------------------------------------+\n");
                        printSeats(&cinemas[i].movies[j].schedule[k]);
                        printf("+-------------------------------------------------------------+\n");
                    }
                }
            }
        }
        break;
    }
    }
}

void initializeSeats(struct Schedule *schedule)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            struct Seat *seat = malloc(sizeof(struct Seat));
            seat->seatNumber = malloc(3);
            sprintf(seat->seatNumber, "%c%d", 'A' + i, j + 1);
            seat->isReserved = 0;
            schedule->seats[i][j] = seat;
        }
    }
}

void printSeats(struct Schedule *schedule)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (schedule->seats[i][j]->isReserved)
            {
                printf("X  ");
            }
            else
            {
                printf("%2s ", schedule->seats[i][j]->seatNumber);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void reserveSeat()
{
    char movieTitle[100];
    char movieTime[100];
    char seatNumber[3];
    int cinemaIndex = -1, movieIndex = -1, scheduleIndex = -1;
    char seatRow;
    int seatColumn;

    printf("Enter movie title: ");
    scanf(" %[^\n]", movieTitle);
    printf("Enter movie time (HH:MM AM/PM): ");
    scanf(" %[^\n]", movieTime);
    printf("Enter seat number (e.g. A1): ");
    scanf(" %[^\n]", seatNumber);

    for (int i = 0; i < CINEMA_COUNT; i++)
    {
        for (int j = 0; j < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); j++)
        {
            if (strcmp(cinemas[i].movies[j].title, movieTitle) == 0)
            {
                cinemaIndex = i;
                movieIndex = j;
                for (int k = 0; k < MAX_SCHEDULE_PER_DAY; k++)
                {

                    printf("%s\n", cinemas[i].movies[j].schedule[k].time);

                    if (strcmp(cinemas[i].movies[j].schedule[k].time, movieTime) == 0)
                    {
                        printf("Schedule found.\n");
                        printf("%s\n", cinemas[i].movies[j].schedule[k].time);
                        scheduleIndex = k;
                        break;
                    }
                }
                // If a matching movie is found but no schedule matches, break the loop
                if (scheduleIndex == -1)
                {
                    printf("No matching schedule found.\n");
                    break;
                }
            }
        }
        // If a matching movie is found (regardless of the schedule), break the loop
        if (movieIndex != -1)
        {

            printf("Movie found.\n");
            break;
        }
    }

    // Handle the case where no matching movie or schedule is found
    if (cinemaIndex == -1 || movieIndex == -1 || scheduleIndex == -1)
    {

        showMenu();
    }

    else
    {
        seatRow = toupper(seatNumber[0]) - 'A';
        seatColumn = atoi(seatNumber + 1) - 1;

        if (seatRow < 0 || seatRow >= 5 || seatColumn < 0 || seatColumn >= 10)
        {
            printf("Invalid seat number.\n");
            showMenu();
        }

        if (cinemas[cinemaIndex].movies[movieIndex].schedule[scheduleIndex].seats[seatRow][seatColumn]->isReserved)
        {
            printf("Seat %s is already reserved.\n", seatNumber);
        }
        else
        {
            cinemas[cinemaIndex].movies[movieIndex].schedule[scheduleIndex].seats[seatRow][seatColumn]->isReserved = 1;
            printf("Seat %s reserved successfully.\n", seatNumber);

            printSeats(&cinemas[cinemaIndex].movies[movieIndex].schedule[scheduleIndex]);
        }
    }
}

int countReservedSeats(struct Schedule *schedule)
{
    int count = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (schedule->seats[i][j]->isReserved)
            {
                count++;
            }
        }
    }
    return count;
}

int timeToMinutes(const char *timeStr)
{
    int hours, minutes;
    char period[3]; // To store "AM" or "PM"
    sscanf(timeStr, "%d:%d %2s", &hours, &minutes, period);

    if ((strcmp(period, "PM") == 0) && (hours < 12))
    {
        hours += 12;
    }
    else if ((strcmp(period, "AM") == 0) && (hours == 12))
    {
        hours = 0;
    }

    return hours * 60 + minutes;
}

void printTickets()
{
    printf("Printing tickets...\n\n");
    for (int i = 0; i < CINEMA_COUNT; i++)
    {
        for (int j = 0; j < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); j++)
        {
            for (int k = 0; k < MAX_SCHEDULE_PER_DAY; k++)
            {
                if (cinemas[i].movies[j].schedule[k].time[0] == '\0')
                {
                    break;
                }
                for (int l = 0; l < 5; l++)
                {
                    for (int m = 0; m < 10; m++)
                    {
                        if (cinemas[i].movies[j].schedule[k].seats[l][m]->isReserved)
                        {
                            printf("%s\n", cinemas[i].name);
                            printf("%s\n", cinemas[i].movies[j].title);
                            printf("%s\n", cinemas[i].movies[j].schedule[k].time);
                            printf("%s\n", cinemas[i].movies[j].schedule[k].seats[l][m]->seatNumber);
                            printf("\n");
                        }
                    }
                }
            }
        }
    }
}

void onExit()
{
    printf("Exiting...\n");

    // Save the reserved seats in a file in the format specified
    FILE *file;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char filename[100];
    sprintf(filename, "ReservedSeats - %d-%d-%d-%d-%d-%d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Could not open file %s", filename);
    }
    else
    {
        for (int i = 0; i < CINEMA_COUNT; i++)
        {
            for (int j = 0; j < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); j++)
            {
                for (int k = 0; k < MAX_SCHEDULE_PER_DAY; k++)
                {
                    if (cinemas[i].movies[j].schedule[k].time[0] == '\0')
                    {
                        break;
                    }
                    // Initialize a variable to store reserved seats as a string
                    char reservedSeats[500] = ""; // Adjust size as necessary
                    int hasReservedSeats = 0;     // Flag to check if there are any reserved seats

                    for (int l = 0; l < 5; l++)
                    {
                        for (int m = 0; m < 10; m++)
                        {
                            if (cinemas[i].movies[j].schedule[k].seats[l][m]->isReserved)
                            {
                                // Append seat number to reservedSeats string
                                char seat[10]; // Adjust size as necessary
                                sprintf(seat, "%s, ", cinemas[i].movies[j].schedule[k].seats[l][m]->seatNumber);
                                strcat(reservedSeats, seat);
                                hasReservedSeats = 1;
                            }
                        }
                    }

                    // Remove the last comma and space if there are reserved seats
                    if (hasReservedSeats)
                    {
                        reservedSeats[strlen(reservedSeats) - 2] = '\0'; // Overwrite the last comma and space
                        fprintf(file, "Cinema No: %s\nTitle: %s\nTime: %s\nTaken Seats:\n%s\n\n", cinemas[i].name, cinemas[i].movies[j].title, cinemas[i].movies[j].schedule[k].time, reservedSeats);
                    }
                }
            }
        }
        fclose(file); // Don't forget to close the file
    }
}

int verifySchedule()
{
    printf("Verifying schedule...\n");

    int startMinutes = timeToMinutes("10:30 AM");
    int endMinutes = timeToMinutes("9:00 PM");
    int buffer = 45; // There's a 45 minute buffer between each movie
    int isValid = 1; // Assume schedule is valid initially

    for (int i = 0; i < CINEMA_COUNT; i++)
    {
        for (int j = 0; j < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); j++)
        {
            for (int k = 0; k < MAX_SCHEDULE_PER_DAY; k++)
            {
                if (cinemas[i].movies[j].schedule[k].time[0] == '\0')
                {
                    break; // No more schedules
                }

                int currentMinutes = timeToMinutes(cinemas[i].movies[j].schedule[k].time);
                int movieDuration = cinemas[i].movies[j].duration; // Assuming duration is in minutes
                int currentMovieEnd = currentMinutes + movieDuration;

                // Check if the showing time is outside the allowed window
                if (currentMinutes < startMinutes || currentMovieEnd > endMinutes)
                {
                    printf("---------------------------------------------\n");
                    printf("Invalid showing time or duration: %s\n", cinemas[i].movies[j].schedule[k].time);
                    printf("---------------------------------------------\n\n\n");
                    isValid = 0; // Mark as invalid
                }

                // Check for overlapping schedules
                if (k > 0)
                {
                    int previousMovieEnd = timeToMinutes(cinemas[i].movies[j].schedule[k - 1].time) + cinemas[i].movies[j].duration + buffer;

                    if (currentMinutes < previousMovieEnd)
                    {
                        printf("---------------------------------------------\n");
                        printf("Overlapping Schedule\nCinema: %s\nMovie: %s\nSchedule: %s and %s\n",
                               cinemas[i].name, cinemas[i].movies[j].title,
                               cinemas[i].movies[j].schedule[k - 1].time, cinemas[i].movies[j].schedule[k].time);

                        int gap = currentMinutes - previousMovieEnd;

                        printf("Overlap between schedules: %d minutes\n", gap);
                        isValid = 0; // Mark as invalid
                        printf("---------------------------------------------\n\n\n");
                    }
                }
            }
        }
    }

    return isValid; // Return 1 if valid, 0 if not
}