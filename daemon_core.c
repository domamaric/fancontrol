#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// PID Controller Structure
typedef struct {
    double Kp;  // Proportional gain
    double Ki;  // Integral gain
    double Kd;  // Derivative gain
    double setpoint;  // Desired setpoint
    double integral;  // Integral term
    double prev_error;  // Previous error
} PIDController;

// Initialize the PID controller
void PID_Init(PIDController *pid, double kp, double ki, double kd, double setpoint) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->setpoint = setpoint;
    pid->integral = 0.0;
    pid->prev_error = 0.0;
}

// Compute the control output
double PID_Compute(PIDController *pid, double current_state) {
    // Calculate the error (the difference between setpoint and current state)
    double error = pid->setpoint - current_state;

    // Calculate the integral term
    pid->integral += error;

    // Calculate the derivative term
    double derivative = error - pid->prev_error;

    // Calculate the control output
    double control_output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);

    // Update the previous error
    pid->prev_error = error;

    return control_output;
}

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
    PIDController pid;
    PID_Init(&pid, 1.0, 0.1, 0.01, 50.0);

    while (1) {
        // Get CPU temperature
        float cpu_temperature = get_cpu_temperature();
        double control_output = PID_Compute(&pid, cpu_temperature);
        // Log CPU temperature
        printf("CPU Temperature: %.2f°C, Control Output: %.2f\n", cpu_temperature, control_output);

        // Sleep for a specified interval (e.g., 1 second)
        sleep(1);
    }

    return 0;
}
