#include <TinyGPS.h>
#include <SoftwareSerial.h>
SoftwareSerial sgsm(9, 10);
SoftwareSerial sgps(4, 5);
float gpslat, gpslon;
TinyGPS gps;

void setup() {
    sgsm.begin(9600);  // Setting the baud rate of GSM Module
    Serial.begin(9600);
    sgps.begin(9600);  // Setting the baud rate of Serial Monitor (Arduino)
    delay(100);
}

void loop() {
    sgps.listen();
    while (sgps.available()) {
        int c = sgps.read();
        if (gps.encode(c)) {
            gps.f_get_position(&gpslat, &gpslon);
        }
    }

    if (Serial.available() > 0) {
        switch (Serial.read()) {
            case 's':
                sgsm.println("AT+CMGF=1");  // Sets the GSM Module in Text Mode
                delay(1000);               // Delay of 1 second
                sgsm.println("AT+CMGS=\"+212622485469\"\r"); // Replace x with mobile number
                delay(1000);
                sgsm.println("les coordonnees de la localisation sont:");
                sgsm.print(" Latitude: ");
                sgsm.print(gpslat, 6);
                sgsm.print(" Longitude: ");
                sgsm.print(gpslon, 6);
                delay(1000);
                sgsm.write(0x1A);
                delay(1000);
                break;

            case 'r':
                sgsm.println("AT+CNMI=2,2,0,0,0");  // AT Command to receive a live SMS
                delay(1000);
                break;
        }
    }

    if (sgsm.available() > 0) {
        Serial.write(sgsm.read());
    }
}

