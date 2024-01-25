# CAN Simulation Case Study 3

## Overview

This case study involves simulating frames between two nodes using the CANoe tool. The communication involves the first node sending Acceleration state messages, influencing the speed of the second node. The project includes a comprehensive database defining the network, nodes, messages, and signals. Additionally, a simple UI is implemented to facilitate frame transmission during data tracing.

## Contents

- **Database:**
  - View the to understand the network structure, nodes, messages, and signals. ![Database Screenshot](CAN%20canoe%20case%20study%203/Images/database_canoe.png) 

- **CAPL Script:**
  - Refer to the for insights into the script that governs the functionality of the communication between nodes.![CAPL Script Screenshot](CAN%20canoe%20case%20study%203/Images/capl%20script%20.png)

- **Simulation GIF:**
  - Explore the showcasing the final output and the dynamic interaction between nodes. ![Simulation GIF](CAN%20canoe%20case%20study%203/Images/canoe%20can%20practicing%20%20(1).gif)

## Usage

1. Open the project in CANoe.
2. Run the simulation.
3. Utilize the provided UI to send frames during the data tracing process.

## Project Structure

- **`database.dbc`:** The DBC file defining the CAN network, nodes, messages, and signals.
- **`capl_script.c`:** The CAPL script implementing the functionality of Acceleration state messages.
- **`ui_interface.ui`:** The user interface file for frame transmission control.




