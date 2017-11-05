
int measurement = 0;
float temp_builtin = 0 ;
const int analogPin = 35;  // Analog input pin 
float mVperAmp = 0.185;//185.0; // use 100 for 20A****66 for 30A **** 185 for 5A
double Voltage = 0;
float VRMS = 0;
float AmpsRMS = 0;
int muestrasPromedio=0;
const int nUmb = 4095;
uint8_t bits = 12 ;
const float vcc =3.3;
const float vEsc = vcc/nUmb ;
const float minUmbral =0.05;
const float correccion =0.9;
const int seg = 3;
const float releOnPlus = 0.65;   // resta el 35% agregado del campo
const float releOnMinus = 1.15;  // suma el 15% restado del campo

////******** FIN ACS712////////

void task_ADC( void * parameter ){
  
  while(1){
    Serial.println("****************************************");
    Serial.print("V Escalon :");Serial.println(vEsc,10);
    Serial.print("12 bits -> ");Serial.println(nUmb);
    float valor = TrueRMSMuestras();

    
    AmpsRMS=(valor/mVperAmp)*correccion ;
    Serial.print("AmpsRMS medido: "); Serial.println(AmpsRMS,3);
    if(AmpsRMS < minUmbral){  AmpsRMS=0;}  
    float Potencia = 220*AmpsRMS;
    Serial.print("AmpsRMS RMS: "); Serial.println(AmpsRMS,3);
    Serial.print("POWER RMS: ");Serial.println(Potencia,2);
    
    measurement = hallRead();
    Serial.print("Hall sensor measurement: ");
    Serial.println(measurement);

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
 const int n = 200;
 const int m = 2000;
 const int mseg = 40;
 uint32_t start_time = millis();
 int signo = 0;
 float Vo =0;
 int diferencia = 0;
 while( (millis()- start_time) < mseg){  
    sumatoria = sumatoria + analogRead(analogPin);
    Count++;
  }
 promedio=(int)(sumatoria/Count);
 Serial.print("***** Muestars promedio**********");
 Serial.print("sumatoria: "); Serial.println(sumatoria);
 Serial.print("Count: "); Serial.println(Count);
 Serial.print("promedio: "); Serial.println(promedio);
 Vo=promedio*vEsc;
 Serial.print("Vo : "); Serial.println(Vo,3);
 start_time = millis();
 Count = 0;
 
 while(( millis()- start_time) < mseg){     
     Count++;
     readValue =  analogRead(analogPin);
     conv=(readValue - promedio)*vEsc;// 2.25;//2793= 2.25v 
     Acumulador = Acumulador+sq(conv);  
    
     }
 suma=Acumulador/Count;
 Serial.print("Acumulador: ");Serial.println(Acumulador,3);
 Serial.print("Count: ");Serial.println(Count);
 result=sqrt(suma);
 Serial.print("analogRead(analogPin): "); Serial.println(analogRead(analogPin));
 Serial.println(String(n)+" Muestras Promedio : "+String(promedio));
 return result;
  }

