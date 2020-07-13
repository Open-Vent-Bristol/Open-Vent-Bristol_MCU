#include <thread>
#include <cstdio>
#include <cstring>

#include "sensors_priv.hpp"
#include "drivers/system.hpp"

extern void application_main(void);

int main(int argc, char *argv[])
{
  System::initialize();
  // The main application should run forever in theory, so there's no way to join
  std::thread (application_main).detach();

  char scanbuf[512] = "";

  while(1) {
    scanf(" %255s", scanbuf);
    if(0 == strcmp("quit", scanbuf)) {
      break;
    }else if(0 == strcmp("systick", scanbuf)) {
      System::getScheduler().triggerCallbacks();
    }else if(0 == strcmp("sensors", scanbuf)) {
      ADC_resolution_t readings[static_cast<unsigned>(SensorType::NUM_SENSORS)];
      for(unsigned i=0; i<static_cast<unsigned>(SensorType::NUM_SENSORS); i++) {
        scanf(" %" SCN_ADC_RESOLUTION, &readings[i]);
      }
      updateSensorValues(readings);
    }
  }

}