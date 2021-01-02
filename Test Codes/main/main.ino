#include "DHT.h"
#include "dust.h"
#include "mq135.h"
#include "mq8.h"
#include "mq9.h"

DHT dht(23,DHT11);
int t,h;
char temp[200];

void setup() 
{
  pinMode(ledPower,OUTPUT);
  Serial2.begin(9600);
  delay(200);
  dht.begin();
  delay(200);
  Serial.begin(9600);
  Serial.print("Calibrating...Mq9\n");                
  Ro = MQCalibration(MQ9PIN);
  Serial.print("Calibration is done...\n"); 
  Serial.print("Ro=");
  Serial.print(Ro);
  Serial.print("kohm");
  Serial.print("\n");
  Serial.print("Calibrating... MQ8\n");                
  Ro1 = MQ8Calibration(MQ8PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air 
                                                    //when you perform the calibration                    
  Serial.print("Calibration is done...\n"); 
  Serial.print("Ro1=");
  Serial.print(Ro1);
  Serial.print("kohm");
  Serial.print("\n");
  Serial.print("Calibrating...Mq135\n");                
  Ro2 = MQ135Calibration(MQ135PIN);
  Ro2=2.511;//Calibrating the sensor. Please make sure the sensor is in clean air                     
  Serial.print("Calibration is done...\n"); 
  Serial.print("Ro2=");
  Serial.print(Ro2);
  Serial.print("kohm");
  Serial.print("\n");
}

void loop()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  int l_pg=MQGetGasPercentage(MQRead(MQ9PIN)/Ro,GAS_LPG);
  int c_m=MQGetGasPercentage(MQRead(MQ9PIN)/Ro,GAS_CARBON_MONOXIDE);
  int met=MQGetGasPercentage(MQRead(MQ9PIN)/Ro,GAS_METHANE);
  int hy=MQ8GetGasPercentage(MQ8Read(MQ8PIN)/Ro1,GAS_HYDROGEN);
  int al=MQ8GetGasPercentage(MQ8Read(MQ8PIN)/Ro1,GAS_ALCOHOL);
  int cd=MQ135GetGasPercentage(MQ135Read(MQ135PIN)/Ro2,GAS_CARBON_DIOXIDE);
  int am=MQ135GetGasPercentage(MQ135Read(MQ135PIN)/Ro2,GAS_AMMONIUM);
  int ace=MQ135GetGasPercentage(MQ135Read(MQ135PIN)/Ro2,GAS_ACETONE);
  read_dust();
  Serial.print("Temp:");
  Serial.print(t);
  Serial.print(",Hum:");
  Serial.print(h);
  Serial.print(",LPG:");
  Serial.print(l_pg);
  Serial.print(",Carbon Monoxide:");
  Serial.print(c_m);
  Serial.print(",Methane:");
  Serial.print(met);
  Serial.print(",Hydrogen:");
  Serial.print(hy);
  Serial.print(",Alcohol:");
  Serial.print(al);
  Serial.print(",Carbon Dioxide:");
  Serial.print(cd);
  Serial.print(",Ammonium:");
  Serial.print(am);
  Serial.print(",Acetone:");
  Serial.print(ace);
  Serial.print(",Dust Density:");
  Serial.println(dustDensity);
  sprintf(temp,"<M Tem:%d><S 4><D U1>",t);
  update_first_message(temp);
  clear_data();
  sprintf(temp,"<M Hum:%d><S 4><D U1>",h);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M LPG:%d><S 4><D U1>",l_pg);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M CO:%d><S 4><D U1>",c_m);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M CH4:%d><S 4><D U1>",met);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M H2:%d><S 4><D U1>",hy);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M Al:%d><S 4><D U1>",al);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M CO2:%d><S 4><D U1>",cd);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M NH3:%d><S 4><D U1>",am);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M Ace:%d><S 4><D U1>",ace);
  update_message(temp);
  clear_data();
  sprintf(temp,"<M PM25:%f><S 4><D U1>",dustDensity);
  update_message(temp);
  clear_data();
  delay(40000);
  delay(20000);
}
void clear_data()
{
  for(int i=0;i<sizeof(temp);i++)
  {
    temp[i]=0;
    delay(20);
  }
  
}
void update_first_message(String msg)
{
  format_data();
  delay(550);
  SendCommand("*");
  delay(550);
  SendMessage(msg);
  delay(550);
  Serial.println("Message Updated SuccessFully");
}
void update_message(String msgg)
{
  SendCommand("#");
  delay(550);
  Serial2.print("6");
  delay(550);
  Serial2.print("6");
  delay(550);
  SendCommand("*");
  delay(550);
  SendMessage(msgg);
  delay(550);
  Serial.println("Message Updated SuccessFully");
}
void format_data()
{
  SendCommand("#");
  delay(550);
  SendCommand("4");
  delay(550);
  Serial2.print("6");
  delay(550);
  Serial2.print("6");
  delay(550);
  Serial.println("SuccessFully Formatted Data");
}
void SendCommand(String chh)
{
  xx:
  //Serial.println(ch);
    Serial2.print(chh);
    delay(20);
    if(Serial2.available()>0)
    {
      goto begin;
    }
    else
    {
      goto xx;
    }
  begin:
    while(!Serial2.available());
    while(Serial2.available()>0)
    {
      String ch=Serial2.readString();
      //Serial.println(ch);
      delay(5);
    }
    Serial2.flush();
    delay(200);
    Serial2.end();
    delay(200);
    Serial2.begin(9550);
    delay(200);
    //Serial.println("Data Read");
}
void SendMessage(String chh)
{
  xx:
  //Serial.println(ch);
    Serial2.println(chh);
    delay(20);
    if(Serial2.available()>0)
    {
      goto begin;
    }
    else
    {
      goto xx;
    }
  begin:
    while(!Serial2.available());
    while(Serial2.available()>0)
    {
      String ch=Serial2.readString();
      //Serial.println(ch);
      delay(5);
    }
    Serial2.flush();
    delay(200);
    Serial2.end();
    delay(200);
    Serial2.begin(9550);
    delay(200);
    //Serial.println("Data Read");
}
