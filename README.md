# navdash
# Rocket Dashboard GUI Project

## Overview

This project is a GUI-based dashboard for a rocket that receives live telemetry data from the onboard systems. It includes various sensors like GPS, altitude, temperature, and others, and it sends commands to the rocket's Flight Control System (FCS). The GUI displays this data dynamically using Qt.

### Features:
- Live telemetry data display (GPS, altitude, temperature).
- Real-time graphical plots for various sensor data.
- Interactive map to display rocket location.
- Button interface with LED indicators to control commands.

## Project Files

- **GPSDataTelemetry.pro**: Qt project file for building the project.
- **qcustomplot.*:** Custom plotting library for displaying graphs.
- **main.cpp**: Entry point of the application.
- **mainwindow.*:** Main window GUI implementation.
- **map.qml**: QML file for the map display.
- **Led.*:** Handles LED indicators for button states.
- **README.md**: This documentation.

## Dependencies

This project requires several external Qt modules and libraries to function correctly. Make sure the following dependencies are installed:

1. **QtSerialPort**: For serial communication with the rocket's telemetry systems.
2. **QtQuick**: For the QML-based map interface.
3. **QtLocation**: For GPS and mapping functionality.
4. **QtPositioning**: For accurate GPS coordinate handling.

### Required Qt Modules:
- QtCore
- QtWidgets
- QtGui
- QtSerialPort
- QtQuick
- QtLocation
- QtPositioning

These modules should be available if you are using Qt 5.9 or later. You can install them via the Qt Maintenance Tool or configure them through your Qt Creator.

## Building the Project

### Step 1: Install Qt and Dependencies

Before building the project, ensure that you have the necessary Qt version installed along with the required modules:

1. Install **Qt Creator** (latest version).
2. Make sure to include the Qt modules for **QtSerialPort**, **QtQuick**, **QtLocation**, and **QtPositioning** when installing Qt.

You can verify that these modules are available by checking your Qt installation paths. If they are missing, you can install them using the **Qt Maintenance Tool** or directly through the Qt online installer.

### Step 2: Clone the Repository

If you haven't already, clone this repository using the following command:

```
git clone https://github.com/yourusername/RocketDashboard.git
cd RocketDashboard
```


### Step 3: Open the Project in Qt Creator

1. Launch **Qt Creator**.
2. Open the project by navigating to the project directory and selecting the `GPSDataTelemetry.pro` file.
   - Go to **File** > **Open File or Project** and browse to the `GPSDataTelemetry.pro` file within the project folder.
   - Select the `.pro` file and click **Open**.

### Step 4: Configure the Build Kit

1. Once the project is opened in Qt Creator, you need to select the appropriate **build kit** (compiler and Qt version).
   - In the **Projects** pane on the left, ensure that the right **Qt version** is selected for your platform.
   - Choose the correct **compiler** (e.g., GCC for Linux, MSVC for Windows, Clang for macOS).
   - You can configure the kit by going to **Tools** > **Options** > **Kits** and selecting or adding the correct Qt version if needed.
2. If Qt Creator detects any missing dependencies, such as **QtSerialPort**, **QtLocation**, or **QtPositioning**, it will prompt you to install them. Follow the on-screen instructions to install any missing modules.

### Step 5: Build the Project

1. Once the kit is configured, click on the **Build** button in the bottom-left of Qt Creator or press `Ctrl+B` to build the project.
2. Wait for the build process to finish. You can monitor the progress in the **Build** pane at the bottom of Qt Creator.
3. Ensure that there are no errors in the build process. If errors occur, check the **Issues** tab for details, and verify if all required libraries and modules are correctly installed.

### Step 6: Run the Project

1. Once the project is successfully built, click on the **Run** button (the green play button) in Qt Creator or press `Ctrl+R` to launch the application.
2. The GUI should now launch, and you will see the telemetry data being displayed on the dashboard.
3. If everything is set up correctly, the application will show live updates for telemetry, including sensor data and GPS location.
