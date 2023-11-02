#ifndef DATAPACKET_H
#define DATAPACKET_H

struct DataPacket {
  String timestamp = "";
  String datestamp = "";
  float temperature = 0.0;
  float pressure = 0.0;
  float humidity = 0.0;
  float latitude = 0.0;
  float longitude = 0.0;
  float speed = 0.0;
  float angle = 0.0;
  float altitude = 0.0;
};

#endif 
