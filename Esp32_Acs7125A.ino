
const int analogPin = 35;  // Analog input pin 
double mVperAmp = 0.185;//185.0; // use 100 for 20A****66 for 30A **** 185 for 5A
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
int muestrasPromedio=0;
const int nUmb = 4095;
const double vcc =3.3;
const double vEsc = vcc/nUmb ;
const float minUmbral =0.020;
const float correccion =0;//0.025;

////******** FIN ACS712////////

void task_ADC( void * parameter ){
  
  while(1){
    Serial.println("****************************************");
    Serial.print("Valor Escalon 3.3/");Serial.print(nUmb,1);Serial.print(":");Serial.println(vEsc,8);
    float valor = TrueRMSMuestras();
    AmpsRMS=valor/mVperAmp - correccion;
    Serial.print("AmpsRMS medido: "); Serial.println(AmpsRMS,3);
    if(AmpsRMS < minUmbral){  AmpsRMS=0;}  
    String Potencia = String(220*AmpsRMS);
    String Corriente = String(AmpsRMS);
    Serial.print("AmpsRMS RMS: "); Serial.println(AmpsRMS,3);
    Serial.println("POWER RMS: " + String(Potencia));
   
    delay(3000);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreate( task_ADC,"ADC",10000,NULL,1,NULL);
}

void loop() {
}

float TrueRMSMuestras(){
  
 double result =0,conv=0,Acumulador=0,suma=0;
 int readValue =0;         
 int Count = 0;
 int promedio = 0;
 double promedioRead = 0;
 int sumatoria = 0;
 const int n = 320;
 int diferencia =0;
 int signo = 0;
 double Vo =0;
 
 
 for(int i=0;i<n;i++){
    delay(1);
    sumatoria = sumatoria + analogRead(analogPin);
  } 
 
 promedio=(int)(sumatoria/n);
 Serial.print("sumatoria: "); Serial.println(sumatoria);
 Serial.print("n: "); Serial.println(n);
 Serial.print("promedio: "); Serial.println(promedio);
 Vo=promedio*vEsc;
 Serial.print("Vo : "); Serial.println(Vo,3);
 uint32_t start_time = millis();
 while(Count < 320){     
    delay(1);
     Count++;
     readValue =  analogRead(analogPin);
     promedioRead=promedioRead + readValue;
     signo = promedio-2793;
     diferencia=abs(signo);
     
    
     conv=(readValue-signo)*vEsc - 2.5;//2793= 2.25v 
    
     Acumulador=Acumulador+sq(conv);  
     }
   suma=Acumulador/Count;
   result=sqrt(suma);
   Serial.println("Count: "+String(Count)+"Muestras");
   Serial.print("analogRead(analogPin): "); Serial.println(analogRead(analogPin));
   Serial.println(String(n)+" Muestras Promedio : "+String(promedio));
  return result;
  }

