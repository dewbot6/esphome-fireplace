#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace drv8830 {

class DRV8830 : public Component {
 public:
  // Default constructor for ESPHome code generation
  DRV8830() : bus_(nullptr), address_(0x60) {}
  
  // Constructor for manual instantiation
  DRV8830(i2c::I2CBus *bus, uint8_t address) : bus_(bus), address_(address) {
  }
  
  // Set I2C bus (for ESPHome code generation)
  void set_i2c_bus(i2c::I2CBus *bus) { bus_ = bus; }
  
  // Set I2C address (for ESPHome code generation)
  void set_i2c_address(uint8_t address) { address_ = address; }

  void setup() override;
  void dump_config() override;

  enum Direction : uint8_t {
    RELEASE = 0,
    BACKWARD = 1,
    FORWARD  = 2,
    BRAKE    = 3
  };

  bool set_motor(uint8_t speed, Direction dir);
  bool get_fault(uint8_t *fault);

 protected:
  bool write_register_(uint8_t reg, uint8_t value);
  bool read_register_(uint8_t reg, uint8_t *value);
  
  i2c::I2CBus *bus_;
  uint8_t address_;
};

}  // namespace drv8830
}  // namespace esphome
