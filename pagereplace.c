#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

// Function to check if page is in frame
int isHit(int frames[], int f, int page) {
    for (int i = 0; i < f; i++) {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}

// FIFO Algorithm
int fifo(int ref[], int n, int f) {
    int frames[f];
    for (int i = 0; i < f; i++) frames[i] = -1;

    int faults = 0, index = 0;

    for (int i = 0; i < n; i++) {
        if (!isHit(frames, f, ref[i])) {
            frames[index] = ref[i];
            index = (index + 1) % f;
            faults++;
        }
    }
    return faults;
}

// LRU Algorithm
int lru(int ref[], int n, int f) {
    int frames[f], time[f];
    for (int i = 0; i < f; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    int faults = 0, t = 0;

    for (int i = 0; i < n; i++) {
        t++;
        int found = 0;

        for (int j = 0; j < f; j++) {
            if (frames[j] == ref[i]) {
                time[j] = t;
                found = 1;
                break;
            }
        }

        if (!found) {
            int min = 0;
            for (int j = 1; j < f; j++) {
                if (time[j] < time[min])
                    min = j;
            }
            frames[min] = ref[i];
            time[min] = t;
            faults++;
        }
    }
    return faults;
}

// Optimal Algorithm
int optimal(int ref[], int n, int f) {
    int frames[f];
    for (int i = 0; i < f; i++) frames[i] = -1;

    int faults = 0;

    for (int i = 0; i < n; i++) {
        if (isHit(frames, f, ref[i]))
            continue;

        int pos = -1, farthest = i;

        for (int j = 0; j < f; j++) {
            int k;
            for (k = i + 1; k < n; k++) {
                if (frames[j] == ref[k])
                    break;
            }

            if (k > farthest) {
                farthest = k;
                pos = j;
            }
        }

        if (pos == -1)
            pos = 0;

        frames[pos] = ref[i];
        faults++;
    }
    return faults;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <length> <frames (1-7)>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int f = atoi(argv[2]);

    if (f < 1 || f > 7) {
        printf("Frames must be between 1 and 7\n");
        return 1;
    }

    int ref[MAX];

    srand(time(0));

    printf("Reference String:\n");
    for (int i = 0; i < n; i++) {
        ref[i] = rand() % 10; // pages 0–9
        printf("%d ", ref[i]);
    }
    printf("\n\n");

    int fifo_faults = fifo(ref, n, f);
    int lru_faults = lru(ref, n, f);
    int opt_faults = optimal(ref, n, f);

    printf("FIFO Page Faults   = %d\n", fifo_faults);
    printf("LRU Page Faults    = %d\n", lru_faults);
    printf("Optimal Faults     = %d\n", opt_faults);

    return 0;
}
