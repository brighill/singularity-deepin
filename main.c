#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <X11/Xlib.h>

static void * threadf(void * data) {
	char * socket_loc = data;

	int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "Can not create UNIX socket.\n");
		exit(1);
	}

	struct sockaddr_un server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_un));
	server_addr.sun_family = AF_UNIX;
	strncpy(server_addr.sun_path, socket_loc, sizeof(server_addr.sun_path) - 1);
	if (bind(fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_un)) < 0) {
		fprintf(stderr, "Can not bind UNIX socket.\n");
		exit(1);
	}

	chmod(socket_loc, 0666);
	int buffer_size = 1024;
	char buffer[buffer_size];

	while (1) {
		struct sockaddr_un client_addr;
		int length = sizeof(struct sockaddr_un);
		int bytes = recvfrom(fd, buffer, buffer_size - 1, 0,
			(struct sockaddr *) &client_addr, &length);

		if (bytes > 0) {
			buffer[bytes] = '\0';
			char * substr = strchr(buffer, '\n');
			if (substr != 0) {
				int index = substr - buffer;
				buffer[index] = '\0';
			}

			if (!fork()) {
				execlp("xdg-open", "xdg-open", buffer, NULL);
				exit(1);
			}
		}
	}
	exit(1);
}

static int error_handler(Display * display) {
	// Stop xdg-open-server with X
	exit(1);
}

int main(int argc, char ** argv) {
	int uid = getuid();
	if (uid == 0) {
		fprintf(stderr, "Can start server from root.\n");
		return 1;
	}

	char socket_dir[100];
	sprintf(socket_dir, "/run/user/%d/xdg-open-server", uid);
	if (!fork()) {
		execlp("mkdir", "mkdir", "-p", socket_dir, NULL);
		exit(1);
	}
	wait(0);
	chmod(socket_dir, 0700);

	struct stat filestat;
	if (stat(socket_dir, &filestat) != 0 || !S_ISDIR(filestat.st_mode)) {
		fprintf(stderr, "Not a directory: %s.\n", socket_dir);
		return 1;
	}

	Display * display;
	if (!(display = XOpenDisplay(0))) {
		fprintf(stderr, "Can open display.\n");
		return 1;
	}

	char * display_var = getenv("DISPLAY");
	if (!display_var || strlen(display_var) == 0) {
		fprintf(stderr, "Can not find display variable.\n", socket_dir);
		return 1;
	}

	char * socket_loc = malloc(100);
	sprintf(socket_loc, "%s/socket%s", socket_dir, display_var);
	unlink(socket_loc);

	// Don't wait for children
	signal(SIGCHLD, SIG_IGN);

	pthread_t thread;
	if (pthread_create(&thread, 0, threadf, socket_loc)) {
		fprintf(stderr, "Can not create thread.\n");
		return 1;
	}

	XSetIOErrorHandler(error_handler);
	while (1) {
		XEvent event;
		XNextEvent(display, &event);
	}

	return 0;
}
