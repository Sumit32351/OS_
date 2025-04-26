#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

float update_temperature(float current_temp, float outside_temp, float target_temp, float heat_transfer_rate, float cooling_power) {
    float temp_change = (outside_temp - current_temp) * heat_transfer_rate;
    if (current_temp > target_temp) {
        temp_change -= cooling_power;
    }
    return current_temp + temp_change;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <room_number>\n", argv[0]);
        return 1;
    }

    int room_id = atoi(argv[1]);
    char msg[256], send_msg[256];
    mqd_t to_rooms = mq_open("/to_rooms", O_RDONLY);
    mqd_t from_rooms = mq_open("/from_rooms
