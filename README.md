# Multithreaded Ticket Reservation System in C

This project simulates a ticket reservation system implemented in C using POSIX threads (`pthreads`). It was developed as part of a university assignment in a Operating Systems course.


## Problem Description

The program simulates a theater ticket booking system where multiple customers call to reserve seats. It models:

- Two seating zones: Zone A and Zone B
- Shared resources: telephones, cashiers, available seats, and payment balance
- Concurrency: multiple threads (one per customer) making concurrent reservations
- Thread synchronization: mutexes and condition variables to manage race conditions


##  How It Works

Each customer is represented by a separate thread and goes through the following process:

1. Phone Reservation
   Waits until a phone line is free. Attempts to book `rand_seats` in either Zone A or Zone B, depending on random assignment.

2. Seat Assignment
   The thread searches for available sequential seats in the assigned zone. If enough are found, they are reserved.

3. Cashier Interaction
   Waits for a cashier to be available, simulates a payment delay, and proceeds with payment.

4. Payment Handling 
   Payment may randomly fail (10% chance). If it does, seats are released.

5. Result Display
   If payment is successful, the reservation is confirmed and printed.


## Key Features

- Concurrency: Threads are used to simulate multiple customers booking simultaneously.
- Mutexes and Conditions: Protect shared data (like seat availability and balance).
- Error Handling: Robust locking/unlocking, thread join, and exit checking.
- Randomized Simulation: Seat numbers, zone assignment, payment outcome, and service delays are randomized using a seed.


### Execution
gcc main.c -o reservation -lpthread
./reservation <number_of_customers> <seed>




