
#define RED 9
#define GREEN 10
#define BLUE 11

#define SWITCH 3
#define LEDS 8


unsigned char red, green, blue = 255;


void setHSV(int hue)
{
  char r, g, b;
  unsigned int accent = hue / 60;
  unsigned char bottom = 0;
  unsigned char top = 255;
  unsigned char rising = (255 * (hue % 60)) / 60;
  unsigned char falling = (255 * (60 - hue % 60)) / 60;

  switch (accent)
  {
  case 0:
    r = top;
    g = rising;
    b = bottom;
    break;
  case 1:
    r = falling;
    g = top;
    b = bottom;
    break;
  case 2:
    r = bottom;
    g = top;
    b = rising;
    break;
  case 3:
    r = bottom;
    g = falling;
    b = top;
    break;
  case 4:
    r = rising;
    g = bottom;
    b = top;
    break;
  case 5:
    r = top;
    g = bottom;
    b = falling;
    break;
  }

  red = r;
  green = g;
  blue = b;


}



void setup() {
  // put your setup code here, to run once:
  pinMode(SWITCH,INPUT);
  pinMode(LEDS,OUTPUT);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int val = digitalRead(SWITCH);
  Serial.println(val);

  if(val){
    digitalWrite(LEDS,LOW);

    

    for(int i =0;i<360;i++){

      setHSV(i);
      
      for(int j=0;j<2;j++){
        for(int k=0;k<255;k++){
          if(red<k){
            digitalWrite(RED,HIGH);
          }
          delayMicroseconds(10);
          digitalWrite(RED,LOW);

          if(green<k){
            digitalWrite(GREEN,HIGH);
          }
          delayMicroseconds(10);
          digitalWrite(GREEN,LOW);

          if(blue<k){
            digitalWrite(BLUE,HIGH);
          }
          delayMicroseconds(10);
          digitalWrite(BLUE,LOW);
          

          
        }


        
      }
      

    


      
    }


    

    
  }else{
    digitalWrite(LEDS,HIGH);
    digitalWrite(RED,LOW);
    digitalWrite(GREEN,LOW);
    digitalWrite(BLUE,LOW);

    delay(500);
  }
  
  
  

}
