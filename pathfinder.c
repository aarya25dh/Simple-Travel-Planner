#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_CITIES 8
#define INF 9999
#define MAX_TICKETS 10
#define MAX_ITINERARY 10

// Graph (Adjacency Matrix)
int graph[MAX_CITIES][MAX_CITIES] = {
    {0, 200, 150, 250, INF, INF, INF, INF}, // Kathmandu
    {200, 0, 80, 160, INF, INF, INF, INF},  // Pokhara
    {150, 80, 0, 140, INF, INF, INF, INF},  // Chitwan
    {250, 160, 140, 0, 210, 230, INF, INF}, // Butwal
    {INF, INF, INF, 210, 0, INF, 540, 450}, // Dharan
    {INF, INF, INF, 230, INF, 0, 550, INF}, // Nepalgunj
    {INF, INF, INF, INF, 540, 550, 0, 120}, // Ilam
    {INF, INF, INF, INF, 450, INF, 120, 0}  // Biratnagar
};

char *cities[MAX_CITIES] = {
    "Kathmandu", "Pokhara", "Chitwan", "Butwal",
    "Dharan", "Nepalgunj", "Ilam", "Biratnagar"};

// Print all cities with their indices
void printCities()
{
    for (int i = 0; i < MAX_CITIES; i++)
    {
        printf("%d-%s", i, cities[i]);
        if (i != MAX_CITIES - 1)
            printf(", ");
    }
    printf(": ");
}

// Dijkstra's Algorithm to find shortest path
void dijkstra(int src)
{
    int dist[MAX_CITIES], visited[MAX_CITIES];

    for (int i = 0; i < MAX_CITIES; i++)
    {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (int count = 0; count < MAX_CITIES - 1; count++)
    {
        int min = INF, minIndex = -1;
        for (int v = 0; v < MAX_CITIES; v++)
        {
            if (!visited[v] && dist[v] < min)
            {
                min = dist[v];
                minIndex = v;
            }
        }
        if (minIndex == -1)
            break; // No more reachable nodes
        visited[minIndex] = 1;

        for (int v = 0; v < MAX_CITIES; v++)
        {
            if (!visited[v] && graph[minIndex][v] && dist[minIndex] + graph[minIndex][v] < dist[v])
            {
                dist[v] = dist[minIndex] + graph[minIndex][v];
            }
        }
    }

    printf("\nShortest distances from %s:\n", cities[src]);
    for (int i = 0; i < MAX_CITIES; i++)
    {
        printf("To %s: %d km\n", cities[i], dist[i]);
    }
}

// Itinerary Management
typedef struct
{
    char places[MAX_ITINERARY][30];
    int top;
} Itinerary;

// adding place to itinerary
void addItinerary(Itinerary *it, char *place)
{
    if (it->top < MAX_ITINERARY - 1)
    {
        strcpy(it->places[++it->top], place);
        printf("%s added to itinerary.\n", place);
    }
    else
    {
        printf("Itinerary is full!\n");
    }
}

// removing place from itinerary
void removeItinerary(Itinerary *it)
{
    if (it->top >= 0)
    {
        printf("%s removed from itinerary.\n", it->places[it->top--]);
    }
    else
    {
        printf("Itinerary is empty!\n");
    }
}

// Ticket Booking System
typedef struct
{
    char name[30];
    int source, destination;
} Ticket;

typedef struct
{
    Ticket tickets[MAX_TICKETS];
    int front, rear;
} TicketQueue;

void bookTicket(TicketQueue *q)
{
    if (q->rear >= MAX_TICKETS - 1)
    {
        printf("Ticket booking full!\n");
        return;
    }

    Ticket t;
    printf("Enter passenger name: ");
    fgets(t.name, 30, stdin);
    t.name[strcspn(t.name, "\n")] = 0;

    printf("Enter source city: ");
    printCities();
    scanf("%d", &t.source);

    printf("Enter destination city: ");
    printCities();
    scanf("%d", &t.destination);
    getchar(); // Clear buffer

    if (t.source < 0 || t.source >= MAX_CITIES || t.destination < 0 || t.destination >= MAX_CITIES)
    {
        printf("Invalid city selection!\n");
        return;
    }

    q->tickets[++q->rear] = t;
    printf("Ticket booked for %s from %s to %s.\n", t.name, cities[t.source], cities[t.destination]);
}

// ticket confirmation
void confirmTicket(TicketQueue *q)
{
    if (q->front <= q->rear)
    {
        Ticket t = q->tickets[q->front++];
        printf("Ticket confirmed: %s from %s to %s.\n", t.name, cities[t.source], cities[t.destination]);
    }
    else
    {
        printf("No tickets to confirm.\n");
    }
}

// MAIN FUNCTION
int main()
{
    TicketQueue tickets = {.front = 0, .rear = -1};
    Itinerary itinerary = {.top = -1};
    int choice;

    printf("Welcome to the Travel Planner System!\n");
    do
    {
        printf("\n1. Find Shortest Travel Route\n2. Book Ticket\n3. Confirm Ticket\n4. Add to Itinerary\n5. Remove from Itinerary\n6. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
        {
            int city;
            printf("Enter source city: ");
            printCities();
            scanf("%d", &city);
            getchar();
            if (city < 0 || city >= MAX_CITIES)
            {
                printf("Invalid city selection!\n");
                break;
            }
            dijkstra(city);
            break;
        }
        case 2:
            bookTicket(&tickets);
            break;
        case 3:
            confirmTicket(&tickets);
            break;
        case 4:
        {
            char place[30];
            printf("Enter place to add to itinerary: ");
            fgets(place, 30, stdin);
            place[strcspn(place, "\n")] = 0;
            addItinerary(&itinerary, place);
            break;
        }
        case 5:
            removeItinerary(&itinerary);
            break;
        case 6:
            printf("Exiting Travel Planner System.\n");
            break;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 6);

    return 0;
}
