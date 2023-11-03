// Compile: "cc fanctrl.c -o fanctrl"
// Run as root.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <unistd.h>

#define EC_SC 0x66
#define EC_DATA 0x62

#define IBF 1
#define OBF 0
#define EC_SC_READ_CMD 0x80
#define EC_SC_WRITE_CMD 0x81
#define EC_SC_SCI_CMD 0x84

// EC addresses
#define EC_TEMP 0x68
#define EC_CURRENT_FAN_SPEED 0x71
#define EC_TEMP_THRESHOLDS_1 0x69
#define EC_FAN_SPEEDS_1 0x72

// "Good values": Fan speeds:   0  31  45  60  65  80  80  85
//                Temp thresh   0  50  59  64  76  80  85  93
// Not used fan values (leftovers from a second fan?)
#define EC_TEMP_THRESHOLDS_2 0x81
#define EC_FAN_SPEEDS_2 0x89

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static int wait_ec(const uint32_t port, const uint32_t flag, const char value) {
	uint8_t data;
	int i = 0;
	data = inb(port);

	while ((((data >> flag)&0x1) != value) && (i++ < 100)) {
		usleep(1000);
		data = inb(port);
	}
	if (i >= 100) {
		printf("[" ANSI_COLOR_RED "ERR" ANSI_COLOR_RESET "] ");
		printf("wait_ec error on port 0x%x, data=0x%x, flag=0x%x, value=0x%x\n", port, data, flag, value);
		return 1;
	}

	return 0;
}

// For read_ec & write_ec command sequence see
// section "4.10.1.4 EC Command Program Sequence" in
// https://wiki.laptop.org/go/Revised_EC_Port_6C_Command_Protocol
static uint8_t read_ec(const uint32_t port) {
	uint8_t value;

	wait_ec(EC_SC, IBF, 0);
	outb(EC_SC_READ_CMD, EC_SC);

	wait_ec(EC_SC, IBF, 0);
	outb(port, EC_DATA);

	wait_ec(EC_SC, OBF, 1);
	value = inb(EC_DATA);

	return value;
}

static void write_ec(const uint32_t port, const uint8_t value) {
	wait_ec(EC_SC, IBF, 0);
	outb(EC_SC_WRITE_CMD, EC_SC);

	wait_ec(EC_SC, IBF, 0);
	outb(port, EC_DATA);

	wait_ec(EC_SC, IBF, 0);
	outb(value, EC_DATA);

	wait_ec(EC_SC, IBF, 0);

	return;
}

static void do_ec(const uint32_t cmd, const uint32_t port, const uint8_t value) {
	wait_ec(EC_SC, IBF, 0);
	outb(cmd, EC_SC);

	wait_ec(EC_SC, IBF, 0);
	outb(port, EC_DATA);

	wait_ec(EC_SC, IBF, 0);
	outb(value, EC_DATA);

	wait_ec(EC_SC, IBF, 0);

	return;
}

static void dump_fan_config() {
    int raw_duty = read_ec(0xCE);
    int val_duty = (int) ((double) raw_duty / 255.0 * 100.0);
	int raw_rpm = (read_ec(0xD0) << 8) + (read_ec(0xD1));
	int val_rpm;

	if (raw_rpm == 0)
		val_rpm = 0;
	else
		val_rpm = 2156220 / raw_rpm;

	printf("[" ANSI_COLOR_GREEN "OK" ANSI_COLOR_RESET "] Dump FAN\n");
	printf("     FAN Duty: %d%%\n", val_duty);
	printf("     FAN RPMs: %d RPM\n", val_rpm);
	printf("     CPU Temp: %d°C\n", read_ec(0x07));
}

static void test_fan_config(int duty_percentage) {
	double v_d = ((double) duty_percentage) / 100.0 * 255.0;
	int v_i = (int) v_d;

	printf("Test FAN %d%% to %d\n", duty_percentage, v_i);

	do_ec(0x99, 0x01, v_i);
	dump_fan_config();
}

static void print_help() {
    printf("Simple fan control utility for MSI Wind and clones\n");
    printf("USE AT YOUR OWN RISK!!\n");
    printf("Usage:\n");
    printf("  fanctrl [OPTION]\n");
    printf("Options:\n");
    printf("  --help, -h    Display this help message\n");
    printf("  [DUTY]        Set the fan duty cycle to the specified percentage (0-100)\n");
}

int main(int argc, char *argv[]) {
	// Check for the --help or -h option
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
				print_help();
				return 0;
		}
	}

	// Initialize I/O port permissions
	int result = ioperm(0x62, 1, 1);
	result += ioperm(0x66, 1, 1);

	// No sudo permissions exit gracefully
	if (result != 0) {
		printf("[" ANSI_COLOR_RED "ERR" ANSI_COLOR_RESET "] ");
		printf("ioperm() failed! Try using sudo privileges!\n");
		exit(1);
	}

	// No additional arguments passed, print current fan configuration
	if (argc == 1) {
		dump_fan_config();
		exit(0);
	}

	// More than one additional argument is sent, exit gracefully
	if (argc != 2) {
		printf("[" ANSI_COLOR_RED "ERR" ANSI_COLOR_RESET"] ");
		printf("Invalid number of arguments! See: sudo ./fanctl --help\n");
		exit(1);
	}

	int duty = atoi(argv[1]);  // argv[0] = "./fanctl"
	test_fan_config(duty);
	
	return 0;
}
