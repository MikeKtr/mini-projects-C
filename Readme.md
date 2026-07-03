# Systems Programming - Lab Assignments

This repository contains a collection of mini-projects developed for the Operating Systems course. The assignments focus on low-level Unix system programming using C, covering process management, inter-process communication (IPC), multi-threading, and networking.

## Table of Contents
1. [Processes](#1. Processes (`lab01`))
2. [Signals](#2-signals)
3. [Pipes](#3-pipes)
4. [Message Queues](#4-message-queues)
5. [Semaphores and Shared Memory](#5-semaphores-and-shared-memory)
6. [Threads](#6-threads)
7. [Sockets](#7-sockets)

---

## 1. Processes (`lab01`)

*   **Task 1:** Compares `fork()` and `vfork()` behaviors. Spawns $N$ child processes that modify a global variable and output diagnostic information under `sleep()` delays to analyze process scheduling and memory separation.
*   **Task 2:** Implements a parent controller (`main.c`) that forks $N$ children and replaces their execution image with an external(`child.c`) using `exec`.
*   **Task 3:** Redirects child output from stdout to a shared file descriptor (`output.txt`). Uses advisory file locking (`fcntl`) to guarantee exclusion.

## 2. Signals (`lab02`)


*   **Task 1:** Demonstrates signal dispositions for `SIGUSR1`. Implements four modes based on user input: `default`, `mask`, `ignore`, and a custom `handle` function.
*   **Task 2:** The parent process spawns a child and uses `sigqueue` to send `SIGUSR2` along with an integer value indicating the chosen signal configuration.
*   **Task 3:** Separates individual signal functions into dedicated compilation units. Includes a `Makefile` configured to bundle these handlers into three distinct archetypes: a Static library, a Shared (dynamically linked) library, and a Dynamically loaded library via `dlopen` and `dlsym`.

## 3. Pipes (`lab03`)
 

*   **Task 1:** Approximates $\pi$ by calculating the Riemann integral of $4/(x^2+1)$ from 0 to 1 using the rectangle method. The workload is distributed across $k$ processes. Each child sends its local scalar result to the parent using an individual unnamed pipe.
*   **Task 2:** Implements two-way process communication using named pipes. Program 1 writes the integration intervals to a named pipe and Program 2 reads the data, calculates the result, and transmits the final value back through a separate named pipe.

## 4. Message Queues (`lab04`)


*   **Task 1:** A multi-client chat application utilizing POSIX message queues.
    *   **Server:** Initializes a central queue, processes `INIT` connection commands, tracks client identifiers, and multicasts incoming messages to all connected clients.
    *   **Client:** Connects to the server queue and creates an internal sub-process dedicated to listening for server broadcasts, preventing user input blocks on stdin.

## 5. Semaphores and Shared Memory (`lab05`)

*   **Task 1:** Implements a multi-process Producer-Consumer subsystem using shared memory segments. Access to the bounded buffer is synchronized using three semaphores: `empty`, `full`, and a `mutex`.
*   **Task 2:** Expands the buffer architecture into two distinct queues: `NORMAL` and `PRIORITY`.
*   **Task 3:** Introduces a supervisor process (`Manager`). The manager runs periodically every 5 seconds to move aging tasks from `NORMAL` to `PRIORITY`, preventing task starvation.

## 6. Threads (`lab06`)

*   **Task 1:** A multi-threaded real-time simulator mimicking a mobile robot subsystem.
    *   **Camera Threads:** Two independent threads generating visual frame frames asynchronously at 25 Hz.
    *   **Synchronization Thread:** Matches left and right frames into stereo pairs if their timestamps drift by less than 20 ms.
    *   **State & Logger Threads:** A high-priority state estimation loop running at 100 Hz paired with a discrete 10 Hz text logging routine. All threads use `pthread_mutex_t` and `sem_t` to avoid race conditions.

## 7. Sockets (`lab07`)

*   **Server:** A low-level HTTP daemon binding to a local port. It maintains a state counter, parses incoming headers, and formats standard HTTP/1.1 wire frames in response to `GET` and `POST` actions. It also parses a custom protocol string to increment its internal state directly.
*   **Client:** A network diagnostic utility that establishes a direct TCP connection via IPv4, transmits the custom `ZADANIE` data payload, prints the raw server response to stdout, and tears down the socket descriptor.
co ty na to