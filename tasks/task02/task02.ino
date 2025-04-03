const int INTERRUPT_PIN = 18;
const int SAMPLE_SIZE = 1000;

volatile uint32_t count = 0;
volatile uint32_t meanTotal = 0;
uint32_t measures[SAMPLE_SIZE];
uint32_t prevTime = 0;

void setup() {
    Serial.begin(9600);
    pinMode(INTERRUPT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), onInterrupt, HIGH);
}

void loop() {
    if (count >= SAMPLE_SIZE) {
        noInterrupts();
        processMeasurements();
        count = 0;
        meanTotal = 0;
        interrupts();
    }
}

void onInterrupt() {
    uint32_t now = micros();
    uint32_t delta = now - prevTime;
    
    if (count < SAMPLE_SIZE) {
        measures[count] = delta;
        meanTotal += delta;
        count++;
    }
    
    prevTime = now;
}

void processMeasurements() {
    float mean = meanTotal / (float)SAMPLE_SIZE;
    float sqDeviationTotal = 0;
    
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        float deviation = measures[i] - mean;
        sqDeviationTotal += deviation * deviation;
    }
    
    Serial.print("Mean: ");
    Serial.print(mean);
    Serial.print(" us, Avg. Sq. Deviation: ");
    Serial.print(sqrt(sqDeviationTotal / SAMPLE_SIZE));
    Serial.println(" us");
}
