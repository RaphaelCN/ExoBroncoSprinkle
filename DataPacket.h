#ifndef DATAPACKET_H
#define DATAPACKET_H

struct DataPacket {
    String timestamp = "";
    float temperature = 0.0;
    float pressure = 0.0;
    float humidity = 0.0;
};

#endif 
