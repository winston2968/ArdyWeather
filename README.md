# ArdyWeather


This project implements a radio communication protocol for data transmission between sensor stations. The system consists of an emitter (sensor station) that sends data (temperature or humidity) to a receiver (base station). The receiver sends acknowledgements (ACK) back to the emitter to confirm the receipt of data. This protocol is designed for low-power communication systems with a focus on data integrity and efficiency.

## Features

- **Data Transmission**: The emitter sends sensor data (temperature and humidity) in the form of datagrams.
- **Acknowledgements**: The receiver sends ACK datagrams to confirm the successful reception of data.
- **Checksum Validation**: A checksum is used to ensure the integrity of the transmitted data. The receiver checks the validity of the checksum before processing the received datagram.
- **Dynamic Resending**: If a datagram is not acknowledged, the emitter will automatically resend the last transmitted datagram.
- **Efficient Encoding**: Sensor data is encoded into ASCII characters for efficient transmission.
- **Custom Datagram Structure**: The datagram format includes a protocol identifier, target station, emitter station, datagram type (data or ACK), the number of sent datagrams, the number of acknowledged datagrams, and the sensor data.

## How it Works

1. **Emitter**: 
   - The emitter gathers sensor data (temperature or humidity) and encodes it into a datagram.
   - The datagram is transmitted to the receiver.
   - If the emitter does not receive an acknowledgement for the sent datagram, it will resend the last datagram.

2. **Receiver**:
   - The receiver listens for incoming datagrams.
   - Upon receiving a data datagram, it validates the checksum.
   - If the checksum is valid, the receiver processes the data and sends an ACK datagram back to the emitter.
   - If the checksum is invalid, the receiver discards the datagram and does not send an ACK.

3. **Checksum**:
   - A checksum is calculated for each datagram to verify the integrity of the data.
   - The checksum is a simple bitwise sum of the datagram's data, formatted to fit within two bytes.
   
## Datagram Format

Each datagram follows this structure:

- **Protocol**: Always "AW", the name of the protocol.
- **Target**: The target station number.
- **Emitter**: The emitter station number.
- **Type**: Either "A" (ACK) or "D" (Data).
- **Datas Type**: Either 'T' (Temperature) or 'H' (Humidity)
- **Checksum Values**
- **Nb Send**: Number of datagrams sent.
- **Nb ACK**: Number of acknowledged datagrams.
- **Datas**: The actual sensor data (temperature or humidity) encoded in ASCII.

## Installation

1. Clone the repository:

   ```bash
   git clone <https://github.com/winston2968/ArdyWeather.git>
   ````
2. Open the project in your prefered IDE. 

## Acknowledgements

- https://github.com/lsongdev/VirtualWire.git for radio controler. 
- https://github.com/Seeed-Studio/Grove_Temper_Humidity for sensor. 
