#include <ncurses.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

WINDOW *game_window, *msg_window;
MEVENT event;

void repeatChar(int n, const chtype ch1, const chtype ch2)
{
	for(int i = 0; i < n; ++i) {
		waddch(game_window, ch1);
		waddch(game_window, ch2);
	}
}

void printBoard(char **game_state)
{
	int center_x, center_y;
	getmaxyx(game_window, center_y, center_x);
	center_x /= 2;
	center_y /= 2;

	// mvwaddch(game_window, 2, 6, ACS_ULCORNER);
	mvwaddch(game_window, center_y - 6, center_x -5, ACS_ULCORNER);
	repeatChar(4, ACS_HLINE, ACS_HLINE);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_URCORNER);
	mvwaddch(game_window, center_y - 5, center_x - 5, ACS_LTEE);
	repeatChar(4, ACS_HLINE, ACS_TTEE);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_RTEE);
	mvwaddch(game_window, center_y - 4, center_x - 5, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	mvwaddch(game_window, center_y - 3, center_x - 9, ACS_ULCORNER);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_TTEE);
	repeatChar(6, ACS_HLINE, ACS_PLUS);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_URCORNER);
	mvwaddch(game_window, center_y - 2, center_x - 9, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	mvwaddch(game_window, center_y - 1, center_x - 9, ACS_LTEE);
	repeatChar(7, ACS_HLINE, ACS_PLUS);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_RTEE);
	mvwaddch(game_window, center_y, center_x - 9, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	mvwaddch(game_window, center_y + 1, center_x - 9, ACS_LLCORNER);
	repeatChar(6, ACS_HLINE, ACS_PLUS);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_BTEE);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_LRCORNER);
	mvwaddch(game_window, center_y + 2, center_x - 7, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	waddch(game_window, ' ');
	waddch(game_window, ACS_VLINE);
	mvwaddch(game_window, center_y + 3, center_x - 7, ACS_LTEE);
	repeatChar(4, ACS_HLINE, ACS_BTEE);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_RTEE);
	mvwaddch(game_window, center_y + 4, center_x - 7, ACS_LLCORNER);
	repeatChar(4, ACS_HLINE, ACS_HLINE);
	waddch(game_window, ACS_HLINE);
	waddch(game_window, ACS_LRCORNER);

	wrefresh(game_window);
}

int main(int argc, char *argv[])
{
	// socket init
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char stream[256];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(argv[1]);
	address.sin_port = atoi(argv[2]);
	len = sizeof(address);

	//result = connect(sockfd, (struct sockaddr*)&address, len);

	if(result == -1) {
		exit(1);
	}

	// while(1) {
	// 	fgets(stream, 256, stdin);
	// 	write(sockfd, &stream, 256);
	// 	read(sockfd, &stream, 256);
	// 	printf("%s\n", stream);
	// }

	// ncurses init
	initscr();
	raw();
	noecho();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	mousemask(BUTTON1_CLICKED, NULL);
	refresh();

	int row, col;
	getmaxyx(stdscr, row, col);
	int center_x = col/2, center_y = row/2;
	int cursor_x = center_x, cursor_y = center_y;
	game_window = newwin(row - 3, col, 0, 0);
	wborder(game_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	msg_window = newwin(3, col, row - 3, 0);
	wborder(msg_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	wrefresh(game_window);
	wrefresh(msg_window);
	refresh();

	printBoard(NULL);

	getch();
	endwin();

	return 0;
}
