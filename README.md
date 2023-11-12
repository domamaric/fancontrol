# Fan Control Driver and Control Center

<!---
![Project Logo](your_project_logo.png)
-->

## Overview

The Fan Control Driver and Control Center is a project designed to empower users with the ability to manage and optimize their laptop or desktop computer's cooling system. This project consists of two main components:

1. **Fan Control Driver**: This component provides a low-level driver interface that enables dynamic control over the fan speed based on various environmental factors, such as CPU temperature. It ensures efficient cooling while minimizing unnecessary noise, ultimately contributing to better system performance and hardware longevity.

2. **Control Center**: The Control Center is a user-friendly application that serves as the graphical interface for configuring and monitoring the fan control settings. It provides an intuitive dashboard for users to customize fan speed profiles, view real-time temperature data, and make informed decisions to maintain their system's thermal balance.

With this project, users can enjoy a tailored and responsive cooling system that adapts to their specific needs, ensuring a comfortable computing experience and optimal hardware health.

## Features

- **Dynamic Fan Control**: The driver adapts fan speed in real-time based on temperature data to strike a balance between cooling efficiency and noise levels.

- **User-Friendly Control Center**: The Control Center offers an intuitive graphical interface for adjusting fan speed profiles, setting temperature thresholds, and monitoring system parameters.

- **Real-Time Monitoring**: The Control Center displays real-time temperature and fan speed information, providing insight into system health and performance.

- **Cross-Platform Compatibility**: The project is designed to work with a wide range of Linux distributions and hardware configurations, ensuring flexibility and accessibility.

## Getting Started

So far the only way of controlling fan is to utilize driver component and change values manually. To set up and start using the Fan Control Driver on your system follow these installation and usage instructions.

Clone this GitHub repo and position working directory accordingly.
```
git clone https://github.com/domamaric/fancontrol.git && cd fancontrol
```

Compile C code and run it using sudo privileges. It is strongly recommended to put `--help` flag in order to know program functionalities.

```
gcc fanctl.c -o fanctl
```

```
./fanctl --help
```

## License

This project is licensed under the [GNU GENERAL PUBLIC LICENSE](LICENSE) - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

I want to express our gratitude to the open-source community and the contributors who have helped make this project possible.

---
<!---
[Optional: Additional sections such as Installation, Usage, Screenshots, Troubleshooting, etc., can be added as per your project's requirements.]
-->
