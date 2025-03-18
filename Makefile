# Board and project parameters
BOARD1 = arduino:avr:uno         		# First board (e.g., an Uno)
PORT1 = /dev/cu.usbserial-A50285BI      # Serial port of the first board (adjust based on your system)
FQBN1 = arduino:avr:uno          		# Full board name (FQBN) for the first board

BOARD2 = arduino:avr:nano        		# Second board (e.g., a Nano)
PORT2 = /dev/ttyUSB1             		# Serial port of the second board (adjust based on your system)
FQBN2 = arduino:avr:nano         		# Full board name (FQBN) for the second board

ARDUINO_DIR = ~/Documents/Arduino   	# Arduino directory (may vary)

# List of source files for each board
SRC1 = src/sensor_module.ino
SRC2 = src/station_module.ino

# Compile command
COMPILER = arduino-cli compile --build-path build

# Default target to compile both programs
all: $(SRC1) $(SRC2)
	$(COMPILER) $(SRC1) --fqbn $(FQBN1) --build-path build/uno
	$(COMPILER) $(SRC2) --fqbn $(FQBN2) --build-path build/nano

# Upload command for the first board
upload_sensor:
	arduino-cli upload -p $(PORT1) --fqbn $(FQBN1) build/uno

# Upload command for the second board
upload_station:
	arduino-cli upload -p $(PORT2) --fqbn $(FQBN2) build/nano

# Target to upload both programs at the same time
upload: upload1 && upload2

# Clean up build files
clean:
	rm -rf build/
