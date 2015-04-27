
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define REMOTE_ADDR "00:06:66:6C:67:3C"


int init_socket();

int main()
{
  int sockfd;
  ssize_t size_read, line_left;
  char line_buf[32];
  char *line_start;




  while(1) {
    sockfd = init_socket();
    if(sockfd < 0) {
      sleep(1);
      continue;
    }

    line_left = sizeof(line_buf);
    line_start = line_buf;

    // Connection went ok, read data until done.
    printf("Connected\n");

    while((size_read = read(sockfd, line_start, line_left)) > 0) {
      if(strchr(line_start, '\n') != NULL) {
        printf("Data: %s\n", line_buf);
        bzero(line_buf, sizeof(line_buf));

        line_left = sizeof(line_buf);
        line_start = line_buf;
      } else {
        line_start += size_read;
        line_left -= size_read;
        if(line_left <= 0) {
          fprintf(stderr, "Line overflow.\n");
          return -1;
        }
      }
    }
    printf("Disconnected\n");
  }
}

int init_socket()
{
  int sockid = -1, rv = 0;
  struct sockaddr_rc bt_addr;
  struct timeval timeout;

  timeout.tv_usec = 0;
  timeout.tv_sec = 2;


  // Clear the bt_addr
  bzero(&bt_addr, sizeof(bt_addr));
  printf("Connecting...\n");

  // Create a socket to connect.
  sockid = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if(sockid < 0) {
    fprintf(stderr, "Invalid socket. %d\n", sockid);
    rv = -1;
    goto out;
  }

  // Blocking comms is okay.
  // Set a timeout as necessary.
  setsockopt(sockid, SOL_SOCKET, SO_RCVTIMEO,
             &timeout, sizeof(struct timeval));

  // Set up the bt_addr
  bt_addr.rc_family = AF_BLUETOOTH;
  bt_addr.rc_channel = (uint8_t) 1;
  str2ba(REMOTE_ADDR, &(bt_addr.rc_bdaddr));

  // Connect the socket to the remote host.
  rv = connect(sockid, (struct sockaddr *) &bt_addr, sizeof(bt_addr));
  if(rv != 0) {
    fprintf(stderr, "Failed to connect: %d\n", rv);
    rv = -2;
    goto out;
  }


out:
  if(rv < 0) {
    close(sockid);
    sockid = -1;
  }

  return sockid;
}
