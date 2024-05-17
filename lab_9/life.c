#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>


typedef struct {
    char **foreground;
    char **background;
    int start;
    int end;
} thread_data;


void *update_grid_thread(void *arg) {
    thread_data *data = (thread_data *)arg;
    while (1) {
        pause();
        for (int i = data->start; i < data->end; i++) {
            int row = i / grid_width;
            int col = i % grid_width;
            (*data->background)[i] = is_alive(row, col, *data->foreground);
        }
    }
    return NULL;
}
void handler(int signo) {
    return;
};

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);


    srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

    int n = atoi(argv[1]);
    pthread_t threads[n];
    thread_data thread_data_array[n];

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

    int cells_per_thread = (grid_width * grid_height) / n;
    for(int i =0; i < n; i++) {
        thread_data_array[i].foreground = &foreground;
        thread_data_array[i].background = &background;
        thread_data_array[i].start = i * cells_per_thread;
        thread_data_array[i].end = (i == n - 1) ? (grid_width * grid_height) : ((i + 1) * cells_per_thread);
        pthread_create(&threads[i], NULL, update_grid_thread, &thread_data_array[i]);
    }
    init_grid(foreground);
	while (true)
	{
		draw_grid(foreground);
        for(int i = 0; i < n; i++) {
            pthread_kill(threads[i], SIGUSR1);
        }

		usleep(500 * 1000);

		// Step simulation
//      update_grid(foreground, background);
		tmp = foreground;
		foreground = background;
		background = tmp;
	}
	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
