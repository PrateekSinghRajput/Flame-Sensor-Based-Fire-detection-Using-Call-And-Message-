
//Prateek
//wwww.prateeks.in
//https://www.youtube.com/c/JustDoElectronics/videos

#include <EEPROM.h>
#include <SoftwareSerial.h> 
SoftwareSerial GSM(2, 3);  

char phone_no[]="+918830584864"; 

#define sensorPin 6 
#define led_sms  3 
#define led_call  4 
int read_value; 

int sms_Status, call_Status;
int flag=0;
int var_1=0, var_2=0;
char input_string[15]; 

void setup(){ 

Serial.begin(9600);
GSM.begin(9600);   
  
pinMode(sensorPin, INPUT); 
pinMode(led_sms,OUTPUT); 
pinMode(led_call,OUTPUT); 

Serial.println("Initializing....");
initModule("AT","OK",1000);               
initModule("ATE1","OK",1000);             
initModule("AT+CPIN?","READY",1000);      
initModule("AT+CMGF=1","OK",1000);        
initModule("AT+CNMI=2,2,0,0,0","OK",1000);  
Serial.println("Initialized Successfully");
//Prateek
//wwww.prateeks.in
//https://www.youtube.com/c/JustDoElectronics/videos
sendSMS(phone_no,"Start GSM Fire Alert System");


sms_Status  = EEPROM.read(1);
call_Status = EEPROM.read(2);
}

void loop(){
readSMS();
  
read_value = digitalRead(sensorPin); 

if(read_value==1){  
flag=0;
}
else{    
if(flag==0){flag=1;
 if(sms_Status==1){sendSMS(phone_no,"Fire Alert....");}
 delay(1000);
 if(call_Status==1){callUp(phone_no);}  
}
delay(1000);
}

  if (var_1 == 1){
     if (!(strncmp(input_string, "Sms On", 6))){sms_Status=1; 
  EEPROM.write(1, sms_Status);
  sendSMS(phone_no,"Message is Active");
  }
else if (!(strncmp(input_string, "Sms Off", 7))){sms_Status=0;
  EEPROM.write(1, sms_Status);
  sendSMS(phone_no,"Message is Deactivate");
  }

else if (!(strncmp(input_string, "Call On", 7))){call_Status=1;
  EEPROM.write(2, call_Status);
  sendSMS(phone_no,"Call is Active");
  }
else if (!(strncmp(input_string, "Call Off", 8))){call_Status=0;
  EEPROM.write(2, call_Status);
  sendSMS(phone_no,"Call is Deactivate");
  }  
  
    var_1 = 0;
    var_2 = 0;
  }

digitalWrite(led_sms, sms_Status); 
digitalWrite(led_call, call_Status); 

}

void sendSMS(char *number, char *msg){
GSM.print("AT+CMGS=\"");GSM.print(number);GSM.println("\"\r\n"); 
delay(500);
GSM.println(msg); 
delay(500);
GSM.write(byte(26)); 
delay(5000);  
}

void callUp(char *number){
GSM.print("ATD + "); GSM.print(number); GSM.println(";"); 
delay(1000);       
}


void readSMS(){
  while(GSM.available()>0){
   
    if (GSM.find("/123")) 
     
    {
      delay(1000);
      while (GSM.available())
      {
        char input_char = GSM.read();
        input_string[var_2++] = input_char;
        if (input_char == '/')
        {
          var_1 = 1;
          return;
        }
      }
    }
 }
}


void initModule(String cmd, char *res, int t){
while(1){
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while(GSM.available()>0){
       if(GSM.find(res)){
        Serial.println(res);
        delay(t);
        return;
       }else{Serial.println("Error");}}
    delay(t);
  }
}
