
const int analogPin = 35;  // Analog input pin 
float mVperAmp = 0.185;//185.0; // use 100 for 20A****66 for 30A **** 185 for 5A
double Voltage = 0;
float VRMS = 0;
float AmpsRMS = 0;
int muestrasPromedio=0;
const int nUmb = 4095;
const float vcc =3.3;
const float vEsc = vcc/nUmb ;
const float minUmbral =0.03;
const float correccion =0.000;
const int seg = 3;

////******** FIN ACS712////////

void task_ADC( void * parameter ){
  
  while(1){
    Serial.println("****************************************");
    Serial.print("V Escalon :");Serial.println(vEsc,8);
    Serial.print("12 bits -> ");Serial.println(nUmb);
    float valor = TrueRMSMuestras();
    AmpsRMS=valor/mVperAmp - correccion;
    Serial.print("AmpsRMS medido: "); Serial.println(AmpsRMS,3);
    if(AmpsRMS < minUmbral){  AmpsRMS=0;}  
    float Potencia = 220*AmpsRMS;
    Serial.print("AmpsRMS RMS: "); Serial.println(AmpsRMS,3);
    Serial.print("POWER RMS: ");Serial.println(Potencia,2);
   
    delay(seg*1000);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreate( task_ADC,"ADC",10000,NULL,1,NULL);
}

void loop() {
}

float TrueRMSMuestras(){
  
 float result =0,conv=0,Acumulador=0,suma=0;
 int readValue =0;         
 int Count = 0;
 int promedio = 0;
 float promedioRead = 0;
 int sumatoria = 0;
 const int n = 2000;
 const int m = 2000;
 
 
 int signo = 0;
 float Vo =0;
 int diferencia = 0;
 
 for(int i=0;i<n;i++){
    delay(.1);
    sumatoria = sumatoria + analogRead(analogPin);
  } 
 
 promedio=(int)(sumatoria/n);
 Serial.print("sumatoria: "); Serial.println(sumatoria);
 Serial.print("n: "); Serial.println(n);
 Serial.print("promedio: "); Serial.println(promedio);
 Vo=promedio*vEsc;
 Serial.print("Vo : "); Serial.println(Vo,3);
 uint32_t start_time = millis();
 while(Count < m){     
     delay(.1);
     Count++;
     readValue =  analogRead(analogPin);
     diferencia = abs(readValue - promedio);
     if(diferencia<10){
       diferencia = 0;
     }
     conv=diferencia*vEsc;// 2.25;//2793= 2.25v 
     Acumulador=Acumulador+sq(conv);  
     }
   suma=Acumulador/Count;
   Serial.print("Acumulador: ");Serial.println(Acumulador,3);
   Serial.print("Count: ");Serial.println(Count);
   result=sqrt(suma);
   Serial.print("analogRead(analogPin): "); Serial.println(analogRead(analogPin));
   Serial.println(String(n)+" Muestras Promedio : "+String(promedio));
  return result;
  }

