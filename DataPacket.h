#ifndef DATAPACKET_H
#define DATAPACKET_H

struct DataPacket {
  volatile String timestamp = "";
  volatile String datestamp = "";
  volatile float temperature = 0.0;
  volatile float pressure = 0.0;
  volatile float humidity = 0.0;
  volatile float latitude = 0.0;
  volatile float longitude = 0.0;
  volatile float speed = 0.0;
  volatile float angle = 0.0;
  volatile float altitude = 0.0;
};

#endif 
