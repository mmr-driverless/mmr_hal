#include "./inc/mmr_linux/mmr_socketcan.h"

#include <fcntl.h>
#include <errno.h>

#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/errno.h>

#include <stdio.h>
#include <delay.h>

const char* iface;

int socketNum;
struct sockaddr_can addr;
struct ifreq ifr;

bool MMR_SOCKETCAN_Start(const char* interface) {
  iface = interface;

  if ((socketNum = socket(PF_CAN, SOCK_RAW | SOCK_NONBLOCK, CAN_RAW)) < 0) {
		perror("MmrSocketCan | Socket connection Failed");
		return false;
	}

	/* Connection to CAN with interface = iface */
	strcpy(ifr.ifr_name, iface);

	if (ioctl(socketNum, SIOCGIFINDEX, &ifr))
	{
		perror("MmrSocketCan | Error during connection to interface");
		return false;
	}

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	/* Open Socket Connection */
	if (bind(socketNum, (struct sockaddr *)&addr, sizeof(addr)))
	{
		perror("MmrSocketCan | Error during socket connection");
		return false;
	}

	return true;
}

static bool __mmr_socketcan_trySend(MmrCanMessage *message) {
	/* 
	  We needed O_NONBLOCK for tryReceive but not for trySend...
	   Perform a polling, blocking write with timeout.
	*/
	
	MmrDelay timeout = { .ms = 200 };
	MMR_DELAY_Reset(&timeout);
	
	struct can_frame frame = {
    .can_id = message->id,
    .can_dlc = message->length
  };

	memcpy(frame.data, message->payload, message->length);
	
	char* ptr = (char*)&frame;
	const char* end = ptr + sizeof(struct can_frame);

	while (ptr < end) {
		int nbytes = write(socketNum, ptr, sizeof(struct can_frame));

		if (nbytes >= 0)
			ptr += nbytes;
		else if (errno != EAGAIN) {
			perror("MmrSocketCan | Write failed");
			return false;
		}

		if (MMR_DELAY_WaitAsync(&timeout)) {
			perror("MmrSocketCan | Write timed out");
			return false;
		}
	}

	return true;
}

static bool __mmr_socketcan_tryReceive(MmrCanMessage *message) {
	struct can_frame frame;

  int nbytes;
	nbytes = read(socketNum, &frame, sizeof(struct can_frame));

	if (nbytes < 0) {
		if (errno == EAGAIN)
			return false;
		
		perror("MmrSocketCan | Error during read.");
		return false;
	}

	if (nbytes != (int)sizeof(struct can_frame)) {
		perror("MmrSocketCan | Linux is high on cocaine");
		return false;
	}

	message->id = frame.can_id;
	message->isStandardId = true;
	message->length = frame.can_dlc;
	memcpy(message->payload, frame.data, frame.can_dlc);
	return true;
}

static uint8_t __mmr_socketcan_getPendingMessages() {
	return 1; // whatever
}

static bool __mmr_socketcan_trySetFilter(MmrCanFilter *filter) { return false; }

MmrCan can0 = {
  .__trySend = __mmr_socketcan_trySend,
  .__tryReceive = __mmr_socketcan_tryReceive,
  .__trySetFilter = __mmr_socketcan_trySetFilter,
  .__getPendingMessages = __mmr_socketcan_getPendingMessages,
};