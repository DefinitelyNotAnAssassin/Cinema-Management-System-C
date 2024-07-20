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

void initializeCinemas();

void searchMovie();

void showSchedule();

void loadSchedule();

int getMovieIndex(struct Cinema *cinema, const char *title);

void initializeSeats(struct Schedule *schedule);

void printSeats(struct Schedule *schedule);

void reserveSeat();

void printTickets();

void onExit();

void verifySchedule();

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

    printf("1. Reserve Seat\n");
    printf("2. View Schedule\n");
    printf("3. Search Movie\n");
    printf("4. Print Tickets\n");
    printf("5. Exit\n");

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
    int cinemaIndex, movieIndex, scheduleIndex, seatRow, seatColumn;

    printf("Enter movie title: ");
    scanf(" %[^\n]", movieTitle);
    printf("Enter movie time (HH:MM): ");
    scanf(" %[^\n]", movieTime);
    printf("Enter seat number (e.g. A1): ");
    scanf(" %[^\n]", seatNumber);

    for (int i = 0; i < CINEMA_COUNT; i++)
    {
        for (int j = 0; j < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); j++)
        {
            if (strcmp(cinemas[i].movies[j].title, movieTitle) == 0)
            {
                for (int k = 0; k < MAX_SCHEDULE_PER_DAY; k++)
                {
                    if (strcmp(cinemas[i].movies[j].schedule[k].time, movieTime) == 0)
                    {
                        cinemaIndex = i;
                        movieIndex = j;
                        scheduleIndex = k;
                        break;
                    }
                }
            }
        }
    }

    seatRow = toupper(seatNumber[0]) - 'A';
    seatColumn = atoi(seatNumber + 1) - 1;

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

int timeToMinutes(const char *timeStr)
{
    int hours, minutes;
    sscanf(timeStr, "%d:%d", &hours, &minutes);
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

    // Save the reserved seats in a file in the format of cinema, movie, time, seat

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
                    for (int l = 0; l < 5; l++)
                    {
                        for (int m = 0; m < 10; m++)
                        {
                            if (cinemas[i].movies[j].schedule[k].seats[l][m]->isReserved)
                            {
                                fprintf(file, "%s\n%s\n%s\n%s\n\n", cinemas[i].name, cinemas[i].movies[j].title, cinemas[i].movies[j].schedule[k].time, cinemas[i].movies[j].schedule[k].seats[l][m]->seatNumber);
                            }
                        }
                    }
                }
            }
        }
    }
}

void verifySchedule()
{

    for (int i = 0; i < CINEMA_COUNT; i++)
    {
        printf("Verifying Cinema: %s\n\n", cinemas[i].name);
        // Assuming 'cinemas', 'i', and 'movieIndex' are already defined in your context
        for (int currentMovieIndex = 0; currentMovieIndex < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); currentMovieIndex++)
        {
            for (int currentScheduleIndex = 0; currentScheduleIndex < MAX_SCHEDULE_PER_DAY; currentScheduleIndex++)
            {
                if (cinemas[i].movies[currentMovieIndex].schedule[currentScheduleIndex].time[0] == '\0')
                {
                    break; // No more schedules for this movie
                }
                int currentEndTime = timeToMinutes(cinemas[i].movies[currentMovieIndex].schedule[currentScheduleIndex].time) + cinemas[i].movies[currentMovieIndex].duration;

                for (int nextMovieIndex = currentMovieIndex; nextMovieIndex < sizeof(cinemas[i].movies) / sizeof(cinemas[i].movies[0]); nextMovieIndex++)
                {
                    for (int nextScheduleIndex = (currentMovieIndex == nextMovieIndex) ? currentScheduleIndex + 1 : 0; nextScheduleIndex < MAX_SCHEDULE_PER_DAY; nextScheduleIndex++)
                    {
                        if (cinemas[i].movies[nextMovieIndex].schedule[nextScheduleIndex].time[0] == '\0')
                        {
                            break; // No more schedules for this movie or moving to the next movie
                        }
                        int nextStartTime = timeToMinutes(cinemas[i].movies[nextMovieIndex].schedule[nextScheduleIndex].time);

                        // Check if there is enough time for the 45-minute gap between the current end time and the next start time
                        if (currentEndTime + 45 > nextStartTime)
                        {
                            printf("Error: Not enough time for cleaning between %s and %s.\n", cinemas[i].movies[currentMovieIndex].title, cinemas[i].movies[nextMovieIndex].title);
                        }
                    }
                }
            }
        }
    }
}