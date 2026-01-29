// ====== DEFINES ======
#define ENA 5       // PWM pin for left motor speed
#define IN1 2       // Left motor direction pin 1
#define IN2 8       // Left motor direction pin 2

#define ENB 6       // PWM pin for right motor speed
#define IN3 4       // Right motor direction pin 1
#define IN4 7       // Right motor direction pin 2

#define TRIG_PIN 9     // Ultrasonic trigger pin
#define ECHO_PIN 10    // Ultrasonic echo pin
#define THRESHOLD 20   // Obstacle distance threshold in cm

// ====== GLOBAL VARIABLES ======
bool turnSide = true;     // true = turn right, false = turn left
long distance = 0;

// ====== SETUP ======
void setup() {
  // Motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);  // start serial communications at 9600 bps
}

// ====== MOTOR FUNCTIONS ======
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 150);  
  analogWrite(ENB, 150);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void stopRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ====== ULTRASONIC FUNCTION ======
long readDistance() {
  // Send trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Convert to distance in cm
  long cm = duration * 0.034 / 2;
  return cm;
}

// ====== LOOP ======
void loop() {
  distance = readDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance < THRESHOLD) {
    stopRobot();  // stop before turning

    if(turnSide){
      Serial.println("Turning Right");
      turnRight();
    } else {
      Serial.println("Turning Left");
      turnLeft();
    }

    delay(500);           // Adjust turn duration
    stopRobot();
    delay(100);           // Short pause

    turnSide = !turnSide; // Flip direction for next obstacle
  } else {
    moveForward();
  }
}
