import serial

# Define the COM port and baud rate
port = 'COM6'  # Replace 'X' with the COM port number
baud_rate = 115200

# File to write the data
output_file = "arduino_data.txt"

# Create a serial object
ser = serial.Serial(port, baud_rate)

try:
    # Open the output file in write mode
    with open(output_file, 'w') as file:
        while True:
            # Read a line of data from the serial port
            line = ser.readline().decode('utf-8').rstrip()
            
            # Write the line to the output file
            file.write(line + "\n")
except KeyboardInterrupt:
    print("Keyboard interrupt detected. Exiting...")

# Close the serial port
ser.close()
