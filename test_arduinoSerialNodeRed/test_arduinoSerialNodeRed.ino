void setup(){
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}
char buffer[7];
byte inByte=0;
int pointer = 0;

void loop(){
  //while (Serial.available()){
  if (Serial.available()>0){
    //inByte = Serial.read();
    
    int code = Serial.parseInt();
    
    if(Serial.read() =='#'){
    
      if (code==3){
         shortLight(300);
      }
      if (code==4){
         shortLight(1000); 
      }
      Serial.println(code);
    }
  } 
}

void shortLight(int delayTime){
  digitalWrite(13,HIGH);
  delay(delayTime);
  digitalWrite(13,LOW);
}
