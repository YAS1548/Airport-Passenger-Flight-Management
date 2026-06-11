#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Passenger {
    int id;
    char name[50];
    struct Passenger* next;
} Passenger;

typedef struct PassengerQueue {
    Passenger* head;
    Passenger* tail;
} PassengerQueue;

typedef struct PassengerStack {
    Passenger* first;
} PassengerStack;

typedef struct Flight {
    int id;
    char destination[50];
    int capacity;
    int passenger_count;
    PassengerQueue* waiting_queue;
    PassengerStack* boarded_passengers;
} Flight;

Passenger* initPassenger(int id, const char* name) {
    Passenger* new_passenger = (Passenger*)malloc(sizeof(Passenger));
    if (new_passenger == NULL) {
        return NULL;
    }
    new_passenger->id = id;
    strncpy(new_passenger->name, name, 50);
    new_passenger->name[49] = '\0';
    new_passenger->next = NULL;
    return new_passenger;
}

void print_passenger(Passenger* passenger) {
    if (passenger != NULL) {
        printf("ID: %d, Nom: %s\n", passenger->id, passenger->name);
    }
}

PassengerQueue* initQueue() {
    PassengerQueue* queue = (PassengerQueue*)malloc(sizeof(PassengerQueue));
    if (queue == NULL) {
        exit(1);
    }
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

int isEmptyQueue(PassengerQueue* queue) {
    return queue->head == NULL;
}

void enqueue(PassengerQueue* queue, Passenger* passenger) {
    if (queue->tail == NULL) {
        queue->head = queue->tail = passenger;
    } else {
        queue->tail->next = passenger;
        queue->tail = passenger;
    }
}

Passenger* dequeue(PassengerQueue* queue) {
    if (isEmptyQueue(queue)) {
        return NULL;
    }
    Passenger* removed = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    return removed;
}

Passenger* search_queue(PassengerQueue* queue, int id) {
    Passenger* current = queue->head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void print_queueRec(Passenger* P) {
    if (P == NULL) return;
    print_passenger(P);
    print_queueRec(P->next);
}

void print_queue(PassengerQueue* queue) {
    printf("File d'attente:\n");
    print_queueRec(queue->head);
}

void freeQueue(PassengerQueue* queue) {
    Passenger* current = queue->head;
    while (current != NULL) {
        Passenger* temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}

PassengerStack* initStack() {
    PassengerStack* stack = (PassengerStack*)malloc(sizeof(PassengerStack));
    if (stack == NULL) {
        exit(1);
    }
    stack->first = NULL;
    return stack;
}

int isEmptyStack(PassengerStack* stack) {
    return stack->first == NULL;
}

void stackPassenger(PassengerStack* stack, Passenger* passenger) {
    passenger->next = stack->first;
    stack->first = passenger;
}

Passenger* unstack(PassengerStack* stack) {
    if (isEmptyStack(stack)) {
        return NULL;
    }
    Passenger* removed = stack->first;
    stack->first = stack->first->next;
    return removed;
}

Passenger* search_stack(PassengerStack* stack, int id) {
    Passenger* current = stack->first;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void print_stackRec(Passenger* current) {
    if (current == NULL) return;
    print_passenger(current);
    print_stackRec(current->next);
}

void print_stack(PassengerStack* stack) {
    printf("Pile d'embarquement:\n");
    print_stackRec(stack->first);
}

void freeStack(PassengerStack* stack) {
    Passenger* current = stack->first;
    while (current != NULL) {
        Passenger* temp = current;
        current = current->next;
        free(temp);
    }
    free(stack);
}

Flight* initFlight(int id, const char* destination, int capacity) {
    Flight* flight = (Flight*)malloc(sizeof(Flight));
    if (flight == NULL) {
        exit(1);
    }
    flight->id = id;
    strncpy(flight->destination, destination, 50);
    flight->capacity = capacity;
    flight->passenger_count = 0;
    flight->waiting_queue = initQueue();
    flight->boarded_passengers = initStack();
    return flight;
}

void addPassengerToFlightQueue(Flight* flight, Passenger* passenger) {
    if (flight->passenger_count >= flight->capacity) {
        return;
    }
    enqueue(flight->waiting_queue, passenger);
}

void finishPassengerCheckIn(Flight* flight) {
    if (isEmptyQueue(flight->waiting_queue)) {
        return;
    }
    Passenger* passenger = dequeue(flight->waiting_queue);
    stackPassenger(flight->boarded_passengers, passenger);
    flight->passenger_count++;
}

Passenger* search_passenger(Flight* flight, int id) {
    Passenger* found = search_queue(flight->waiting_queue, id);
    if (found) {
        return found;
    }
    found = search_stack(flight->boarded_passengers, id);
    return found;
}

void printFlight(Flight* flight) {
    printf("\nVol ID: %d, Destination: %s, Capacite: %d, Passagers embarques: %d\n", 
           flight->id, flight->destination, flight->capacity, flight->passenger_count);
    print_queue(flight->waiting_queue);
    print_stack(flight->boarded_passengers);
}

void freeFlight(Flight* flight) {
    freeQueue(flight->waiting_queue);
    freeStack(flight->boarded_passengers);
    free(flight);
}

int main() {
    Passenger* passengers[15];
    for (int i = 0; i < 15; i++) {
        char name[20];
        sprintf(name, "Passager%d", i + 1);
        passengers[i] = initPassenger(i + 1, name);
    }

    Flight* flights[3];
    flights[0] = initFlight(1, "Seoul", 5);
    flights[1] = initFlight(2, "Rome", 5);
    flights[2] = initFlight(3, "Tokyo", 5);

    for (int i = 0; i < 15; i++) {
        int flight_index = i / 5;
        addPassengerToFlightQueue(flights[flight_index], passengers[i]);
    }

    for (int i = 0; i < 3; i++) {
        printFlight(flights[i]);
    }

    for (int i = 0; i < 3; i++) {
        while (!isEmptyQueue(flights[i]->waiting_queue)) {
            Passenger* p = dequeue(flights[i]->waiting_queue);
            stackPassenger(flights[i]->boarded_passengers, p);
            flights[i]->passenger_count++;
            printf("Check-in for passenger %d in flight %d is done.\n", p->id, flights[i]->id);
        }
        printFlight(flights[i]);
    }

    int search_ids[5] = {3, 8, 10, 14, 20};
    for (int i = 0; i < 5; i++) {
        int found = 0;
        for (int j = 0; j < 3; j++) {
            Passenger* in_queue = search_queue(flights[j]->waiting_queue, search_ids[i]);
            Passenger* in_stack = search_stack(flights[j]->boarded_passengers, search_ids[i]);

            if (in_queue) {
                printf("Passenger %d is in the queue for flight %d.\n", search_ids[i], flights[j]->id);
                found = 1;
                break;
            } else if (in_stack) {
                printf("Passenger %d is in the stack for flight %d.\n", search_ids[i], flights[j]->id);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Passenger %d is not in any flight.\n", search_ids[i]);
        }
    }

    for (int i = 0; i < 3; i++) {
        freeFlight(flights[i]);
    }

    return 0;
}
