# DHT11 / DTH22 thermostat

[DHT11 / DTH22](https://learn.adafruit.com/dht) based simple thermostat to control a relay.

Based on [MakerduDimanche's gist](https://gist.github.com/MakerduDimanche/cb51ab456589a577b802cc33670b567a)

Integrated led will blink when relay is passing (heat in ON).
At program start, led blinks quickly to let user identify it.

## Usage

- download `DHT sensor library` zip from https://github.com/adafruit/DHT-sensor-library/releases
- download `Adafruit Sensor library` zip from https://github.com/adafruit/Adafruit_Sensor/releases
- open `thermostat_DTH22.ino` in [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- add previously downloaded libraries to project (details [here](https://www.arduino.cc/en/Guide/Libraries#toc4))
- edit file, and set:
    - desired temperature: `TARGET_TEMP`
    - DTH type `DHT_TYPE` ("DHT22" or "DHT11")
- take note (or edit) of DTH and relay pins:
    - `DHT_PIN`
    - `RELAIS_PIN`
- when program starts, it makes