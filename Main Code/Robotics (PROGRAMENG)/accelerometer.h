#define accelAddress 0x02

typedef struct{
    byte MsgSize;
    byte DeviceAddress;
    byte LocationPtr;
   }accelBlock;

typedef struct{
   int x;
   int y;
   int z;
   }accelData;

accelBlock accelI2C;
tSensors Accelport;

void accelStart(tSensors port)
{
   Accelport =port;

   accelI2C.MsgSize =2;
   accelI2C.DeviceAddress = accelAddress;
   accelI2C.LocationPtr = 0x42;

   SensorType[Accelport]= sensorI2CCustomFast;
}

void readAccel(accelData &values)
{
   byte rawData[6];

   nI2CBytesReady[Accelport] = 0;

  sendI2CMsg(Accelport, accelI2C.MsgSize, 6);

  while (nI2CStatus[Accelport] == STAT_COMM_PENDING)
     wait1Msec(2);

   if (nI2CStatus[Accelport] == NO_ERR)
   {
      readI2CReply(Accelport, rawData[0], 6);

      values.x =  ( rawData[0] << 2) + (0xFF & rawData[3]);
      values.y =  ( rawData[1] << 2) + (0xFF & rawData[4]);
      values.z =  ( rawData[2] << 2) + (0xFF & rawData[5]);
   }
}