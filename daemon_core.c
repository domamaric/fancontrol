#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function to retrieve CPU temperature
float get_cpu_temperature() {
    FILE* file = fopen("/sys/class/thermal/thermal_zone2/temp", "r");
    if (file == NULL) {
        perror("Failed to open temperature file");
        exit(1);
    }

    unsigned int temperature;
    if (fscanf(file, "%d", &temperature) != 1) {
        fclose(file);
        perror("Failed to read temperature");
        exit(1);
    }

    fclose(file);

    // Convert temperature from millidegrees Celsius to degrees Celsius
    float temp_celsius = (float)temperature / 1000.0;

    return temp_celsius;
}

int main() {
    while (1) {
        // Get CPU temperature
        float cpu_temperature = get_cpu_temperature();

        // Log CPU temperature
        printf("CPU Temperature: %.2f°C\n", cpu_temperature);

        // Sleep for a specified interval (e.g., 1 second)
        sleep(1);
    }

    return 0;
}
