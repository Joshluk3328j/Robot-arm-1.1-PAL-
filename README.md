# Robot Arm v1.1 – PAL

Welcome to the second iteration of the **6 DOF Robotic Arm** project, codenamed **PAL** (Precision Articulated Limb). This version brings significant improvements over its predecessor, featuring lighter yet stronger components, enhanced torque calculations, and refined control logic. Developed during my Industrial Training at **NCAIR**, with guidance and support from **@Mac Anthony**.

🔗 **Related Links**  
- [Robot Arm v1.0](https://github.com/Joshluk3328j/robot-arm-1.0)
- [Robot Arm v1.1 PAL Repository](https://github.com/Joshluk3328j/Robot-arm-1.1-PAL-)

---

## 📂 Repository Structure

```
├── blocks/                  # Conceptual block diagrams
├── code/                    # Arduino Nano control code (servo, joystick, playback)
├── drawings/                # Technical drawings of parts
├── Gcode/                   # 3D printer G-code files
├── PRT files/               # Native CAD part files (SolidWorks/Creo)
│   ├── Base/                # Base components
│   ├── controller/          # Controller housing
│   ├── end_effector/        # Gripper/end effector parts
│   ├── Link/                # Arm linkage components
│   └── ...
├── Rendered/                # Rendered images of the design
│   ├── arm/                 # Arm renders
│   └── controller_box/      # Controller renders
├── STL files/               # 3D-printable STL models
│   ├── base/                # Base STL files
│   ├── controller_box/      # Controller housing STL files
│   ├── end_effector/        # Gripper STL files
│   └── links/               # Arm linkage STL files
└── torque_calc/             # Torque requirement calculations
```

---

## ⚙️ Features

- **6 Degrees of Freedom (DOF)**: Full range of motion for precise control.
- **Arduino Nano Control**: Powered by an Arduino Nano for reliable operation.
- **Joystick & Button Inputs**: Intuitive manual control with joysticks and buttons.
- **Motion Recording & Playback**: Store motion sequences and replay them autonomously.
- **Optimized Torque Calculations**: Ensures reliable performance under load.
- **Lightweight Design**: Uses optimized CAD parts for reduced weight and increased strength.
- **3D-Printable**: Includes STL files and G-code for easy fabrication.

---

## 🚀 Getting Started

### Hardware Requirements
- **Arduino Nano** (microcontroller)
- **6x Servo Motors** (for arm joints)
- **Joysticks** (potentiometer-based for axes control)
- **Control Buttons + Buzzer** (for user inputs and feedback)
- **3D-Printed Parts** (STL and G-code files provided in `STL files/` and `Gcode/`)

### Software Requirements
- **Arduino IDE**: For uploading control code (located in `code/`).
- **SolidWorks/Creo**: For viewing/editing CAD files (located in `PRT files/`).
- **Cura/PrusaSlicer**: For slicing STL files for 3D printing (located in `STL files/`).

### Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Joshluk3328j/Robot-arm-1.1-PAL-
   ```
2. **Upload Arduino Code**:
   - Open `code/` in the Arduino IDE.
   - Connect the Arduino Nano and upload the code.
3. **3D Print Parts**:
   - Use Cura/PrusaSlicer to slice STL files in `STL files/`.
   - Print using a 3D printer or use provided G-code in `Gcode/`.
4. **Assemble Hardware**:
   - Follow technical drawings in `drawings/` for assembly.
   - Connect servo motors, joysticks, buttons, and buzzer to the Arduino Nano as per the schematics in `blocks/`.

---

## 🎮 Controls

### Joystick Controls
- **Axes**: Control **base**, **shoulder**, **elbow**, and **wrist pitch**.
- **Button Left/Right**:
  - Single press: Control **wrist roll** and **gripper**.
  - Double press: Toggle direction of wrist roll/gripper.

### Button Controls
- **Button1 (Pin 12)**:
  - **Single Press**: Save the current arm position.
  - **Double Press**: Switch between **MANUAL** and **PLAY** modes.
- **Button2 (Pin 4)**:
  - **Single Press (in PLAY mode)**: Pause or resume playback.
  - **Double Press (in MANUAL mode)**: Delete the last recorded motion.
  - **Double Press (in PLAY mode)**: Clear all saved motions.

---

## 📸 Demo

### Rendered Arm
![Rendered Arm](https://github.com/Joshluk3328j/Robot-arm-1.1-PAL-/blob/main/rendered%20arm.jpg) 

### Built Arm (Real Prototype)
![Built Arm](https://github.com/Joshluk3328j/Robot-arm-1.1-PAL-/blob/main/arm.jpg)

---

## 🙏 Acknowledgements

- **God**: For strength and inspiration.
- **@Mac Anthony**: For guidance and mentorship.
- **NCAIR**: For providing the opportunity and resources to develop this project.

---

## 📜 License

This project is licensed under the **MIT License**.  
Feel free to fork, modify, and enhance the project! 🚀
