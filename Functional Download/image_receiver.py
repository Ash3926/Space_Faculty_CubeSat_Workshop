import serial
import time

# Port for Mac
# PORT = '/dev/cu.usbserial-11110' 
PORT = 'COM5'
BAUD = 9600
FILENAME = "received_photo.jpg"

try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    ser.write(input("Type 'SENDIMAGE_G1' to request image: "))
    print(f"Listening on {PORT}...")

    while True:
        line = ser.readline()
        
        if b"---START_IMAGE---" in line:
            print("Receiving Image...")
            
            # Remove the START IMAGE marker from the line just like you removed END IMAGE
            clean_start_line = line.replace(b"---START_IMAGE---", b"")
            
            with open(FILENAME, "wb") as f:
                # Write any data that might have been on the same line as the start marker
                f.write(clean_start_line)
                
                while True:
                    # Read raw bytes until the end marker
                    chunk = ser.read_until(b"---END_IMAGE---")
                    
                    if b"---END_IMAGE---" in chunk:
                        # Remove the END IMAGE marker from the final data
                        clean_end_data = chunk.replace(b"---END_IMAGE---", b"")
                        f.write(clean_end_data)
                        print(f"Success! Image saved as {FILENAME}")
                        break
                    
                    f.write(chunk)
        else:
            print(line.decode().strip()) # Prints any non image line to console for debugging
except Exception as e:
    print(f"Error: {e}")
finally:
    if 'ser' in locals():
        ser.close()