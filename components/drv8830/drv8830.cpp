#include "drv8830.h"
#include "esphome/core/log.h"

namespace esphome {
namespace drv8830 {

static const char *const TAG = "drv8830";

void DRV8830::setup() {
  uint8_t fault;
  if (!this->read_register_(0x01, &fault)) {
    ESP_LOGE(TAG, "DRV8830 not found at 0x%02X", this->address_);
  } else {
    ESP_LOGI(TAG, "DRV8830 detected at 0x%02X", this->address_);
    if (fault != 0) {
      ESP_LOGW(TAG, "DRV8830 fault register: 0x%02X", fault);
    }
  }
}

void DRV8830::dump_config() {
  ESP_LOGCONFIG(TAG, "DRV8830 Motor Driver");
  LOG_I2C_DEVICE(this);
}

bool DRV8830::set_motor(uint8_t speed, Direction dir) {
  if (speed > 63) speed = 63;
  uint8_t control = (speed << 2) | (dir & 0x03);
  return write_register_(0x00, control);
}

bool DRV8830::get_fault(uint8_t *fault) {
  return read_register_(0x01, fault);
}

bool DRV8830::write_register_(uint8_t reg, uint8_t value) {
  return this->write_byte(reg, value);
}

bool DRV8830::read_register_(uint8_t reg, uint8_t *value) {
  return this->read_byte(reg, value);
}

}  // namespace drv8830
}  // namespace esphome
