// C++ code
//
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define Passcode_Length 5

Servo myservo;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

int pos = 0;

char Data[Passcode_Length];
char Master[Passcode_Length] = "0007";
byte data_count = 0, master_count = 0;

bool Pass_is_good;
bool bin = false;
char customKey;

const int trig = 11;  
const int echo = 10;  
int green = 8;
long duration,cm;  

/*-----keypad--controls-----*/

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);


/*----Passcode mechanism----*/
void setup()
{
  myservo.attach(9, 2000, 2400);
  ServoClose();
  lcd.begin(16, 2);
  lcd.print("Chipp Recycle");
  loading("Loading");
  lcd.clear();
  pinMode(trig, OUTPUT);  
  pinMode(echo, INPUT);   
  pinMode(green, OUTPUT);
  myservo.attach(9); 
  myservo.write(0);
}


void loop()
{
  if (bin == true)
  {
    customKey = customKeypad.getKey();
    if (customKey == '#')
    {
      lcd.clear();
      ServoClose();
      lcd.print("Bin is closed");
      delay(1000);
      bin = false;
    }
  }
  else
    Open();
    working();
  	

}

void loading (char msg[]) {
  lcd.setCursor(0, 1);
  lcd.print(msg);

  for (int i = 0; i < 9; i++) {
    delay(100);
    lcd.print(".");
  }
}

void clearData()
{
  while (data_count != 0)
  { 
    Data[data_count--] = 0;
  }
  return;
}

void ServoClose()
{
  for (pos = 90; pos >= 0; pos -= 10) { 
    myservo.write(pos);
  }
}

void ServoOpen()
{
  for (pos = 0; pos <= 90; pos += 10) {
    myservo.write(pos);  
  }
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print("Enter your pin");
  
  customKey = customKeypad.getKey();
  if (customKey)
  {
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
  }

  if (data_count == Passcode_Length - 1)
  {
    if (!strcmp(Data, Master))
    {
      lcd.clear();
      ServoOpen();
      lcd.print("valid User!");
      bin = true;
      delay(3000);
      loading("Waiting");
      lcd.clear();
      lcd.print(" Time is up! ");
      delay(1000);
      ServoClose();
      lcd.clear();
      lcd.print("Thank You...");
      delay(10000000);
      bin = false;  
     
    }
    else
    {
      lcd.clear();
      lcd.print("Not Registered!");
      delay(30);
      lcd.clear();
      lcd.print("Please Sign Up");
      lcd.clear();
      bin = false;
    }
    delay(500);
    lcd.clear();
    clearData();
  }
}

void working()   
{  
  //Serial.begin(9600);  
  digitalWrite(trig, LOW);  
  delay(100);  
  digitalWrite(trig, HIGH);  
  delay(100);  
  digitalWrite(trig, LOW);
  
  duration = pulseIn(echo, HIGH);  
  cm = 0.034*(duration/2);  
  //Serial.println(distance);  
  if (cm <= 300 && cm>15)  
  { 
    
   myservo.write(180);  
   digitalWrite(green,HIGH); 
   delay(1000);  
  }
  else   
  {   
   digitalWrite(green,LOW);
   myservo.write(0);  
  } 
  
    
}



