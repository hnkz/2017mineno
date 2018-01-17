#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

int main(){
  FILE* out;
  
  while(1){
      int pin = 4;
      int data[100];
      int counter = 0;
      int laststate = HIGH;
      int i = 0, j = 0;

      if(wiringPiSetupGpio() == -1) return 1;
      pinMode(pin, OUTPUT);

      digitalWrite(pin, HIGH);
      usleep(500000);
      digitalWrite(pin, LOW);
      usleep(20000);

      pinMode(pin, INPUT);

      data[0] = data[1] = data[2] = data[3] = data[4] = 0;
        
      while (digitalRead(pin) == 1) usleep(1);

      for (i = 0; i < 100; i++) {
          counter = 0;
          while ( digitalRead(pin) == laststate) {
              counter++;
              if (counter == 1000) break;
          }
          laststate = digitalRead(pin);
          if (counter == 1000) break;

          if ((i > 3) && (i % 2 == 0)) {
              data[j / 8] <<= 1;
              if (counter > 200) data[j / 8] |= 1;
              j++;
          }
      }
      if ((j >= 39) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        float f, h;
        f = (data[2] & 0x7F)* 256 + data[3];
        f /= 10.0;
        if (data[2] & 0x80) f *= -1;
        
        h = data[0] * 256 + data[1];
        h /= 10; 
        out = fopen("temp_humid.csv", "a");
        fprintf(out, "%f,%f\n", f, h);
        fclose(out);
      }
      sleep(1);
  }

  return 0;
}
