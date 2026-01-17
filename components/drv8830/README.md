# ESPHome DRV8830 Motor Driver Component

ESPHome component for the DRV8830 I2C motor driver.

## Installation

Add this to your ESPHome YAML configuration:

```yaml
external_components:
  - source: github://andrewsnodgrass/esphome-fireplace/components/drv8830
```

## Usage

### Basic Configuration

```yaml
i2c:
  sda: GPIO2
  scl: GPIO1
  scan: true

drv8830:
  id: motor_driver
  address: 0x60  # Optional, defaults to 0x60
```

### Using in Lambdas

```yaml
switch:
  - platform: template
    name: "Motor Forward"
    turn_on_action:
      - lambda: |-
          id(motor_driver)->set_motor(32, id(motor_driver)->FORWARD);
      - delay: 1s
      - lambda: |-
          id(motor_driver)->set_motor(0, id(motor_driver)->RELEASE);
```

### Manual Instantiation

If you need to manually create the component instance:

```yaml
globals:
  - id: drv8830_instance
    type: void*
    restore_value: false
    initial_value: 'nullptr'

switch:
  - platform: template
    name: "Motor Forward"
    turn_on_action:
      - lambda: |-
          auto *drv = static_cast<esphome::drv8830::DRV8830*>(id(drv8830_instance));
          if (drv == nullptr) {
            drv = new esphome::drv8830::DRV8830(id(bus_a), 0x60);
            App.register_component(drv);
            id(drv8830_instance) = static_cast<void*>(drv);
          }
          drv->set_motor(32, drv->FORWARD);
```

## API

### Methods

- `set_motor(uint8_t speed, Direction dir)` - Set motor speed (0-63) and direction
- `get_fault(uint8_t *fault)` - Read fault register

### Directions

- `RELEASE` (0) - Coast/Release
- `BACKWARD` (1) - Reverse
- `FORWARD` (2) - Forward
- `BRAKE` (3) - Brake

## License

BSD License
