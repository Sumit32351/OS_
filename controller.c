#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define NUM_ROOMS 4

int main() {
    mqd_t to_rooms, from_rooms;
    struct mq_attr attr = {0, 10, 256, 0};
    char msg[256];

    to_rooms = mq_open("/to_rooms", O_CREAT | O_WRONLY, 0644, &attr);
    from_rooms = mq_open("/from_rooms", O_CREAT | O_RDONLY, 0644, &attr);

    if (to_rooms == -1 || from_rooms == -1) {
        perror("mq_open");
        return 1;
    }

    const char* target_temp = "22.0";  // Target temperature

    // Send target temperature once to all rooms
    for (int i = 0; i < NUM_ROOMS; i++) {
        mq_send(to_rooms, target_temp, strlen(target_temp) + 1, 0);
    }

    printf("Controller: Sent target temperature %s°C to all rooms\n", target_temp);

    // Receive continuous temperature updates
    while (1) {
        for (int i = 0; i < NUM_ROOMS; i++) {
            if (mq_receive(from_rooms, msg, sizeof(msg), NULL) > 0) {
                printf("Controller: Received room update -> %s\n", msg);
            }
        }
        sleep(1);
    }

    mq_close(to_rooms);
    mq_close(from_rooms);
    mq_unlink("/to_rooms");
    mq_unlink("/from_rooms");

    return 0;
}
