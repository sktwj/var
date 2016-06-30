#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include "server.h"

/* HTTP response and header for a successful request.
*/
static char *ok_pic_response = 
"HTTP/1.1 200 OK\r\n"
"Content-type: image/png\r\n"
"\r\n";

static char *ok_mp3_response = 
"HTTP/1.1 200 OK\r\n"
"Content-type: audio/mpeg\r\n"
"\r\n";
		
static char* ok_response =
"HTTP/1.1 200 OK\r\n"
"Content-type: text/html\r\n"
"\r\n";

static char* ok_text_response =
"HTTP/1.1 200 OK\r\n"
"Content-type: text/plain;charset=utf-8\r\n"
"\r\n";
/* HTTP response, header, and body, indicating that we didn't
   understand the request. */
static char* bad_request_response =
"HTTP/1.0 400 Bad Request\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <body>\n"
" <h1>Bad Request</h1>\n"
" <p>This server did not understand your request.</p>\n"
" </body>\n"
"</html>\n";
/* HTTP response, header, and body template, indicating that the
   requested document was not found. */
static char* not_found_response_template =
"HTTP/1.0 404 Not Found\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <body>\n"
" <h1>404 Not Found</h1>\n"
" <p>The requested URL %s was not found on this server.</p>\n"
" </body>\n"
"</html>\n";
/* HTTP response, header, and body template, indicating that the
   method was not understood. */
static char* bad_method_response_template =
"HTTP/1.0 501 Method Not Implemented\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <body>\n"
" <h1>Method Not Implemented</h1>\n"
" <p>The method %s is not implemented by this server.</p>\n"
" </body>\n"
"</html>\n";
/* Handler for SIGCHLD, to clean up child processes that have
   terminated. */
static void clean_up_child_process (int signal_number)
{
	int status;
	wait (&status);
}
/* Process an HTTP "GET" request for PAGE, and send the results to the
   file descriptor CONNECTION_FD. */
static int my_send_file(int connection_fd, const char *file, const char *resp)
{
	
		char response[1024] = {0};
		int fd = open(file, O_RDONLY);
		if (fd == -1) {
			/* Generate the response message. */
			snprintf (response, sizeof (response), not_found_response_template, file);
			/* Send it to the client. */
			write (connection_fd, response, strlen (response));
			return -1;
		} else {
			write (connection_fd, /*ok_response*/ resp, strlen (resp));
			struct stat stat_buf = {0};
			fstat(fd, &stat_buf);

			char *pbuf = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

			write(connection_fd, pbuf, stat_buf.st_size);
		}
		return 0;
}

static void handle_get (int connection_fd, const char* page)
{
	struct server_module* module = NULL;
	/* Make sure the requested page begins with a slash and does not
	   contain any additional slashes -- we don't support any
	   subdirectories. */
	printf("page:%s\n", page);
	if (strcmp(page, "/") == 0) {
		/*打开索引文件*/
#if 0
		int fd = open("./index.html", O_RDONLY);
		if (fd == -1) {
			char response[1024];
			/* Generate the response message. */
			snprintf (response, sizeof (response), not_found_response_template, page);
			/* Send it to the client. */
			write (connection_fd, response, strlen (response));
		} else {
				
		write (connection_fd, ok_response, strlen (ok_response));
		struct stat stat_buf = {0};
		fstat(fd, &stat_buf);

		char *pbuf = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

		write(connection_fd, pbuf, stat_buf.st_size);
		}
#endif
		my_send_file(connection_fd, "./index.html", ok_response);
	} else if (*page == '/' && strstr(page + 1, ".png") != NULL) {
#if 0
		char response[1024] = {0};
		const char *fname = page + 1;
		int fd = open(fname, O_RDONLY);
		if (fd == -1) {
			/* Generate the response message. */
			snprintf (response, sizeof (response), not_found_response_template, page);
			/* Send it to the client. */
			write (connection_fd, response, strlen (response));
		} else {
			struct stat stat_buf = {0};
			fstat(fd, &stat_buf);
			int n = snprintf (response, sizeof(response), ok_pic_response, "png");
			write(connection_fd, response, n);
			sendfile(connection_fd, fd, NULL, stat_buf.st_size);
		}
#endif
		my_send_file(connection_fd, page + 1, ok_pic_response);
	} else if (strstr(page + 1, ".mp3")){
		my_send_file(connection_fd, page + 1, ok_mp3_response);	
	} else if (*page == '/' && strchr (page + 1, '.') == NULL) {
		//"没有后缀的表示加载模块"
		char module_file_name[64];
		/* The page name looks OK. Construct the module name by appending
		   ".so" to the page name. */
		snprintf (module_file_name, sizeof (module_file_name),
				"%s.so", page + 1);
		printf("open module: %s\n", module_file_name);
		/* Try to open the module. */
		module = module_open (module_file_name);
		if (module == NULL) { 
			/* Either the requested page was malformed, or we couldn't open a
			   module with the indicated name. Either way, return the HTTP
			   response 404, Not Found. */
			/*TODO  file*/
			char response[1024];
			/* Generate the response message. */
			snprintf (response, sizeof (response), not_found_response_template, page);
			/* Send it to the client. */
			write (connection_fd, response, strlen (response));
		} else {
			/* The requested module was loaded successfully.
			 */
			/* Send the HTTP response indicating success, and the HTTP header
			   for an HTML page. */
			write (connection_fd, ok_response, strlen (ok_response));
			/* Invoke the module, which will generate HTML output and send it
			   to the client file descriptor. */
			(*module->generate_function) (connection_fd);
			/* We're done with the module. */
			module_close (module);
		}
	} else {
		//ignore '/' of '/1.txt' == './1.txt'
		my_send_file(connection_fd, page + 1, ok_text_response);
	}
}
/* Handle a client connection on the file descriptor CONNECTION_FD.
gd: go to dest  */
static void handle_connection (int connection_fd)
{
	char buffer[256];
	ssize_t bytes_read;
	/* Read some data from the client. */
	bytes_read = read (connection_fd, buffer, sizeof (buffer) - 1);
	if (bytes_read > 0) {
		printf("buffer:%s\n", buffer);
		char method[sizeof (buffer)];
		char url[sizeof (buffer)];
		char protocol[sizeof (buffer)];
		/* Some data was read successfully. NUL-terminate the buffer so
		   we can use string operations on it. */
		buffer[bytes_read] = '\0';
		printf("%s\n", buffer);
		/* The first line the client sends is the HTTP request, which is
		   composed of a method, the requested page, and the protocol
		   version. */
		sscanf (buffer, "%s %s %s", method, url, protocol);
		/* The client may send various header information following the
		   request. For this HTTP implementation, we don't care about it.
		   However, we need to read any data the client tries to send. Keep
		   on reading data until we get to the end of the header, which is
		   delimited by a blank line. HTTP specifies CR/LF as the line
		   delimiter. */


		while (strstr (buffer, "\r\n\r\n") == NULL)
			bytes_read = read (connection_fd, buffer, sizeof (buffer));
		/* Make sure the last read didn't fail. If it did, there's a
		   problem with the connection, so give up. */
		if (bytes_read == -1) {
			close (connection_fd);
			return;
		}
		/* Check the protocol field. We understand HTTP versions 1.0 and
		   1.1. */
		if (strcmp (protocol, "HTTP/1.0") && strcmp (protocol, "HTTP/1.1")) {
			/* We don't understand this protocol. Report a bad response. */
			write (connection_fd, bad_request_response,
					sizeof (bad_request_response));
		}
		else if (strcmp (method, "GET")) {
			/* This server only implements the GET method. The client
			   specified some other method, so report the failure. */
			char response[1024];
			snprintf (response, sizeof (response),
					bad_method_response_template, method);
			write (connection_fd, response, strlen (response));
		} else
			/* A valid request. Process it. */
			   handle_get (connection_fd, url);
	}
	else if (bytes_read == 0)
		/* The client closed the connection before sending any data.
		   Nothing to do. */
		;
	else
		/* The call to read failed. */
		system_error ("read");
}
void server_run (struct in_addr local_address, uint16_t port)
{
	struct sockaddr_in socket_address;
	int rval;
	struct sigaction sigchld_action;
	int server_socket;
	/* Install a handler for SIGCHLD that cleans up child processes that
	   have terminated. */
	memset (&sigchld_action, 0, sizeof (sigchld_action));
	sigchld_action.sa_handler = clean_up_child_process;
	sigaction (SIGCHLD, &sigchld_action, NULL);

	/* Create a TCP socket. */
	server_socket = socket (PF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
		system_error ("socket");
	/* Construct a socket address structure for the local address on
	   which we want to listen for connections. */
	memset (&socket_address, 0, sizeof (socket_address));
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = port;
	socket_address.sin_addr = local_address;

	int reuseaddr = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
	/* Bind the socket to that address. */
	rval = bind (server_socket, (const struct sockaddr *)&socket_address, sizeof (socket_address));
	if (rval != 0)
		system_error ("bind");
	/* Instruct the socket to accept connections. */
	rval = listen (server_socket, 10);
	if (rval != 0)
		system_error ("listen");
	if (verbose) {
		/* In verbose mode, display the local address and port number
		   we're listening on. */
		socklen_t address_length;
		/* Find the socket's local address. */
		address_length = sizeof (socket_address);
		rval = getsockname (server_socket, (struct sockaddr *)&socket_address, &address_length);
		assert (rval == 0);
		/* Print a message. The port number needs to be converted from
		   network byte order (big endian) to host byte order. */
		printf ("server listening on %s:%d\n",
				inet_ntoa (socket_address.sin_addr),
				(int) ntohs (socket_address.sin_port));
	}
	/* Loop forever, handling connections. */
	   while (1) {
	   struct sockaddr_in remote_address;
	   socklen_t address_length;
	   int connection;
	   pid_t child_pid;
	/* Accept a connection. This call blocks until a connection is
	   ready. */
	address_length = sizeof (remote_address);
	connection = accept (server_socket, (struct sockaddr *)&remote_address, &address_length);
	if (connection == -1) {
		/* The call to accept failed. */
		if (errno == EINTR)
			/* The call was interrupted by a signal.*/
			continue;
		else
			/* Something else went wrong. */
			system_error ("accept");
	}
	/* We have a connection. Print a message if we're running in
	   verbose mode. */
	if (verbose) {
		socklen_t address_length;
		/* Get the remote address of the connection. */
		address_length = sizeof (socket_address);
		rval = getpeername (connection, (struct sockaddr *)&socket_address, &address_length);
		assert (rval == 0);
		/* Print a message. */
		printf ("connection accepted from %s:%d\n",
				inet_ntoa (socket_address.sin_addr),
				(int)ntohs(socket_address.sin_port));
	}
	/* Fork a child process to handle the connection. */
	child_pid = fork ();
	if (child_pid == 0) {
		/* This is the child process. It shouldn't use stdin or stdout,
		   so close them. */
		close (STDIN_FILENO);
		//close (STDOUT_FILENO);
		/* Also this child process shouldn't do anything with the
		   listening socket. */
		close (server_socket);
		/* Handle a request from the connection. We have our own copy
		   of the connected socket descriptor. */
		handle_connection (connection);
		/* All done; close the connection socket, and end the child
		   process. */
		close (connection);
		exit(0);
	} else if (child_pid > 0) {
		/* This is the parent process. The child process handles the
		   connection, so we don't need our copy of the connected socket
		   descriptor. Close it. Then continue with the loop and
		   accept another connection. */
		close (connection);
	} else
		/* Call to fork failed. */
		system_error ("fork");
	}
}
