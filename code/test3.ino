// ---------------- ROBOT ARM CONTROL ----------------
#include <Servo.h>

#define NUM_AXES 4
int joyPins[NUM_AXES] = {A0, A1, A2, A3};  // Joystick axes
Servo servos[6];                            // 6 DOF robot

// Buttons
int buttonLeft   = 7;   // Wrist roll
int buttonRight  = 8;   // Gripper
int buttonRecord = 12;  // Button1
int buttonPlay   = 4;   // Button2

// Buzzer
int buzzerPin = 2;

// Servo positions
int servoPos[6] = {90, 90, 90, 90, 90, 90};
int stepSize = 2;      // Manual move step size
int deadzone = 100;

// Button timing
unsigned long lastPressLeft  = 0;
unsigned long lastPressRight = 0;
unsigned long lastPressRec   = 0;
unsigned long lastPressPlay  = 0;
unsigned long doublePressWindow = 400; // ms
bool leftReverse  = false;
bool rightReverse = false;

// Step timing (manual control)
unsigned long lastStepTime = 0;
unsigned long stepInterval = 50; // ms between increments

// Playback timing
unsigned long lastPlayTime = 0;
unsigned long playInterval = 1000; // ms between motions

// Mode and motion memory
enum Mode { MANUAL, PLAY };
Mode currentMode = MANUAL;
bool isPlaying = false;

const int MAX_MOTIONS = 50;
int motions[MAX_MOTIONS][6]; // store up to 50 frames
int motionCount = 0;
int playIndex = 0;

// Playback helpers
int targetPos[6];
bool moving = false;

// ---------------- BUZZER HELPERS ----------------
void beep(int freq, int dur) {
  tone(buzzerPin, freq, dur);
  delay(dur);
  noTone(buzzerPin);
}
void longBuzz() { beep(600, 600); }

// ---------------- MOTION MEMORY ----------------
void saveMotion() {
  if (motionCount < MAX_MOTIONS) {
    for (int i = 0; i < 6; i++) {
      motions[motionCount][i] = servoPos[i];
    }
    motionCount++;
    beep(2000, 100);
    Serial.print("Saved motion #");
    Serial.println(motionCount);
  } else {
    beep(400, 200); // error beep if full
    Serial.println("Memory full!");
  }
}

void deleteLastMotion() {
  if (motionCount > 0) {
    motionCount--;
    beep(1200, 150);
    Serial.print("Deleted last motion. Remaining: ");
    Serial.println(motionCount);
  } else {
    beep(400, 200);
    Serial.println("No motions to delete!");
  }
}

void clearAllMotions() {
  motionCount = 0;
  beep(800, 300);
  Serial.println("All motions cleared!");
}

void setup() {
  Serial.begin(9600);

  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(buttonRecord, INPUT_PULLUP);
  pinMode(buttonPlay, INPUT_PULLUP);

  servos[0].attach(3);   // Base
  servos[1].attach(5);   // Shoulder
  servos[2].attach(6);   // Elbow
  servos[3].attach(9);   // Wrist pitch
  servos[4].attach(10);  // Wrist roll
  servos[5].attach(11);  // Gripper

  pinMode(buzzerPin, OUTPUT);

  Serial.println("=== ROBOT ARM CONTROL STARTED ===");
}

void loop() {
  unsigned long now = millis();

  bool leftPressed   = (digitalRead(buttonLeft) == LOW);
  bool rightPressed  = (digitalRead(buttonRight) == LOW);
  bool recPressed    = (digitalRead(buttonRecord) == LOW);
  bool playPressed   = (digitalRead(buttonPlay) == LOW);

  // ---------------- MANUAL MODE CONTROL ----------------
  if (currentMode == MANUAL && now - lastStepTime >= stepInterval) {
    lastStepTime = now;

    // Joysticks 0–3
    for (int i = 0; i < NUM_AXES; i++) {
      int val = analogRead(joyPins[i]);
      if (val < (512 - deadzone)) {
        servoPos[i] = constrain(servoPos[i] - stepSize, 0, 180);
      } else if (val > (512 + deadzone)) {
        servoPos[i] = constrain(servoPos[i] + stepSize, 0, 180);
      }
      servos[i].write(servoPos[i]);
    }

    // Wrist roll (buttonLeft)
    static bool leftPrev = false;
    if (leftPressed && !leftPrev) {
      if (now - lastPressLeft < doublePressWindow) {
        leftReverse = !leftReverse;
        beep(1200, 80);
      } else {
        beep(1000, 50);
      }
      lastPressLeft = now;
    }
    leftPrev = leftPressed;

    if (leftPressed) {
      servoPos[4] = constrain(servoPos[4] + (leftReverse ? -stepSize : stepSize), 0, 180);
      servos[4].write(servoPos[4]);
    }

    // Gripper (buttonRight)
    static bool rightPrev = false;
    if (rightPressed && !rightPrev) {
      if (now - lastPressRight < doublePressWindow) {
        rightReverse = !rightReverse;
        beep(1700, 80);
      } else {
        beep(1500, 50);
      }
      lastPressRight = now;
    }
    rightPrev = rightPressed;

    if (rightPressed) {
      servoPos[5] = constrain(servoPos[5] + (rightReverse ? -stepSize : stepSize), 0, 180);
      servos[5].write(servoPos[5]);
    }
  }

  // ---------------- PLAYBACK MODE ----------------
  if (currentMode == PLAY && isPlaying && motionCount > 0) {
    if (!moving) {
      // Load next target frame
      for (int i = 0; i < 6; i++) {
        targetPos[i] = motions[playIndex][i];
      }
      Serial.print("Playing motion #");
      Serial.println(playIndex + 1);
      beep(1000, 30); // short beep between frames
      moving = true;
    }

    // Smooth step towards target
    bool reached = true;
    for (int i = 0; i < 6; i++) {
      if (servoPos[i] < targetPos[i]) {
        servoPos[i]++;
        reached = false;
      } else if (servoPos[i] > targetPos[i]) {
        servoPos[i]--;
        reached = false;
      }
      servos[i].write(servoPos[i]);
    }

    // If all joints reached target, wait interval then go to next frame
    if (reached && millis() - lastPlayTime >= playInterval) {
      lastPlayTime = millis();
      playIndex++;
      if (playIndex >= motionCount) playIndex = 0; // loop playback
      moving = false;
    }
  }

  // ---------------- BUTTON1: SAVE / MODE ----------------
  static bool recPrev = false;
  if (recPressed && !recPrev) {
    if (now - lastPressRec < doublePressWindow) {
      // Double press -> toggle mode
      currentMode = (currentMode == MANUAL) ? PLAY : MANUAL;
      isPlaying = false;
      playIndex = 0;
      moving = false;
      longBuzz();
      Serial.print(">>> Mode changed to: ");
      Serial.println((currentMode == MANUAL) ? "MANUAL" : "PLAY");
    } else {
      // Single press -> save motion
      saveMotion();
    }
    lastPressRec = now;
  }
  recPrev = recPressed;

  // ---------------- BUTTON2: PLAY / DELETE ----------------
  static bool playPrev = false;
  if (playPressed && !playPrev) {
    if (now - lastPressPlay < doublePressWindow) {
      if (currentMode == MANUAL) {
        deleteLastMotion();
      } else {
        clearAllMotions();
      }
    } else {
      if (currentMode == PLAY) {
        isPlaying = !isPlaying;
        beep(isPlaying ? 2200 : 800, 150);
        Serial.println(isPlaying ? ">>> PLAYING" : ">>> PAUSED");
      }
    }
    lastPressPlay = now;
  }
  playPrev = playPressed;
}
