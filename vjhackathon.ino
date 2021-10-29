#define USE_ARDUINO_INTERRUPTS true // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>  // Includes the PulseSensorPlayground Library.
#define tempin A2
#include <TinyGPS.h>
#include <SoftwareSerial.h>
SoftwareSerial SIM900(2, 3); // configure software serial port (rx, tx)
TinyGPS gps;                 //Creates a new instance of the TinyGPS object
//  Variables
const int PulseWire = 0;           // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;              // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;               // Determine which Signal to "count as a beat" and which to ignore.
                                   // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                                   // Otherwise leave the default "550" value.
PulseSensorPlayground pulseSensor; // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
int val;

void setup()
{
    Serial.begin(9600);
    SIM900.begin(9600);
    pulseSensor.analogInput(PulseWire);
    pulseSensor.blinkOnPulse(LED13); //auto-magically blink Arduino's LED with heartbeat.
    pulseSensor.setThreshold(Threshold);
 
    // Double-check the "pulseSensor" object was created and "began" seeing a signal.
    if (pulseSensor.begin())
    {
        Serial.println("We created a pulseSensor Object !"); //This prints one time at Arduino power-up,  or on Arduino reset.
    }

    pinMode(tempin, INPUT);

    Serial.print("Simple TinyGPS library v. ");
    Serial.println(TinyGPS::library_version());
    Serial.println("Testing GPS");
    Serial.println();

    
}
void loop()
{
  val = analogRead(tempin);
    float mv = (val / 1024.0) * 5000;
    float cell = mv / 10;
    Serial.println();
    Serial.print("TEMPARATURE = ");
    Serial.print(cell);
    Serial.print("*c");
    Serial.println();
    delay(1000);

    
   int myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".
                                                 // "myBPM" hold this BPM value now.

    if (pulseSensor.sawStartOfBeat()) // Constantly test to see if "a beat happened".
    {
        Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
        Serial.print("BPM: ");                        // Print phrase "BPM: "
        Serial.println(myBPM);                        // Print the value inside of myBPM.
    }
    
    delay(20); // considered best practice in a simple sketch
    bool newData = false; //gps
    unsigned long chars;
    unsigned short sentenes, failed;
     int coordinates[2] = {0, 0};

    // For one second we parse GPS data and report some key values
    for (unsigned long start = millis(); millis() - start < 1000;)
    {
        while (Serial.available())
        {
            char c = Serial.read();
            //Serial.print(c);
            if (gps.encode(c))
                newData = true;
        }
    }

    if (newData) //If newData is true
    {
        float flat, flon;
        unsigned long age;
        gps.f_get_position(&flat, &flon, &age);
        coordinates[0] = flat;
        coordinates[1] = flon;
    }



    if (cell < 35 || cell > 40 ||  myBPM < 60 ||  myBPM > 125)

    {
      
    SIM900.println("ATD + +919845678903;"); // Enter mobile number here to call
    Serial.println("ATD + +91984678903;");
    Serial.println(("ATD + +108;"));
    delay(100);
    SIM900.println();
    delay(30);          // wait for 30 seconds...
    SIM900.println("ATH"); // disconnect after 30 second
     
    Serial.println("making phone call at emergency situation");
    Serial.println("ATH");
    
    SIM900.println(" AT+CMGSF=1");
    SIM900.println("AT + CMGS =\" +917569951331\"\r");
    Serial.println("AT + CMGS =\" +917569951331\"\r");
    Serial.println("AT + CMGS =\" +108\"\r");
    String datamessage = ("Temperature: " + String(cell) + "*C " + " BPM " + String(myBPM));
    SIM900.println(datamessage);
    
    Serial.println("message=");
    Serial.println(datamessage);
    SIM900.println(coordinates[0]);
    
    Serial.println("location");
    Serial.print("longitude=");
    Serial.println( coordinates[0]);
    SIM900.println(coordinates[1]);
    Serial.print("latitude=");
    Serial.println(coordinates[1]);
    SIM900.println((char)26);

    }


    
    
}
