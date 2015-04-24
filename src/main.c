
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <libudev.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  int sockid = -1, rv = 0;
  ssize_t size_read;
  char buf[32];
	struct sockaddr_rc bt_addr;
	// Clear the bt_addr
	memset(&bt_addr, 0, sizeof(bt_addr));

	// Check input.
  if(argc < 2) {
    fprintf(stderr, "Error, too small input.");
    rv = -1;
    goto out;
  }

	// Create a socket to connect.
  sockid = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if(sockid < 0) {
		fprintf(stderr, "Invalid socket. %d\n", sockid);
		rv = -2;
		goto out;
	}

	// Set up the bt_addr
  bt_addr.rc_family = AF_BLUETOOTH;
  bt_addr.rc_channel = (uint8_t) 1;
  str2ba(argv[1], &(bt_addr.rc_bdaddr));

	// Connect the socket to the remote host.
  rv = connect(sockid, (struct sockaddr *) &bt_addr, sizeof(bt_addr));
  if(rv != 0) {
    fprintf(stderr, "Failed to connect: %d\n", rv);
    goto out;
  }

	// Connection went ok, read data untill done.
  printf("Connected\n");
  while((size_read = read(sockid, buf, sizeof(buf))) > 0) {
    printf("Data: %d\n", atoi(buf));
  }
  printf("Disconnected\n");

out:
  if(sockid > -1) {
    close(sockid);
  }
  return rv;
}
