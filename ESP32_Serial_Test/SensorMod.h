#pragma once

#include "ModuleDefinition.h"
#include "BaseModule.h"

class SensorMod : public BaseModule {
public:
  typedef struct {
    unsigned switch_1: 1;
    unsigned switch_2: 1;
    unsigned switch_3: 1;
    unsigned switch_4: 1;
    unsigned switch_5: 1;
    unsigned switch_6: 1;
    unsigned switch_7: 1;
  }  in_switch_struct;

  typedef union {
      uint8_t all;
      in_switch_struct bits;
  } in_switch;

  in_switch switch_state;
  short adc_1;
  short adc_2;
  short adc_3;
  short adc_4;
  short adc_5;
  short adc_6;

  short* const adc_map[6];

  SensorMod(uint8_t _module_num) :
    BaseModule(ModuleID::SensorModule, _module_num),
    switch_state{0},
    adc_1(0),
    adc_2(0),
    adc_3(0),
    adc_4(0),
    adc_5(0),
    adc_6(0),
    adc_map{
      &adc_1,
      &adc_2,
      &adc_3,
      &adc_4,
      &adc_5,
      &adc_6
    }
  {

  }

  short GetDigital(uint8_t device_id) {
    if (device_id > 7 | device_id < 1) return 0;
    return switch_state.all >> (device_id - 1) & 1;
  }

  short GetAnalog(uint8_t device_id) {
    if (device_id > 6 | device_id < 1) return 0;
    return *adc_map[device_id - 1];
  }
};