import argparse
import logging
import os
import sys
import time
import serial

def setup_logger(debug_mode):
    """Configures the logging level based on debug flag."""
    level = logging.DEBUG if debug_mode else logging.INFO
    logging.basicConfig(
        level=level,
        format="%(asctime)s [%(levelname)s] %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S"
    )

def init_grbl(s):
    """Sends the startup and homing sequences to the GRBL controller."""
    logging.info("Initializing GRBL and clearing alarms...")
    
    s.write(b"\x18")  # Soft Reset
    time.sleep(2)
    s.write(b"$X\n")  # Kill Alarm
    time.sleep(0.5)

    # Turn OFF the settings causing the lock
    s.write(b"$21=1\n") # Disable Hard Limits
    s.write(b"$22=1\n") # Disable Homing
    time.sleep(0.5)

    # Set "Fake Home"
    s.write(b"$$\n")
    s.write(b"G1 F1000\n")
    s.write(b"S1000\n")
    s.write(b"G92 X0 Y0\n")
    s.write(b"G21\n") # Set units to mm
    s.write(b"G90\n")
    # time.sleep(2)
    # s.write(b"G92 X0 Y0 Z0\n") 
    # time.sleep(2)
    # s.write(b"G0 X80 Y65\n")
    # time.sleep(2)
    # s.write(b"G0 X0 Y0\n")

    # Clear out the startup messages from the buffer
    while s.in_waiting:
        msg = s.readline().decode().strip()
        if msg:
            logging.debug(f"GRBL Init: {msg}")
            
    logging.info("GRBL connected and ready.")

def process_command(s, line):
    """Sends a single G-code line and waits for the controller's response."""
    clean = line.strip()
    if not clean or clean.startswith(';'):
        return # Skip empty lines and comments

    logging.debug(f"Sending: {clean}")
    s.write((clean + '\n').encode())

    # Wait for 'ok' or catch errors
    while True:
        response = s.readline().decode().strip()
        if not response:
            continue
            
        logging.debug(f"Received: {response}")
        
        if 'ok' in response:
            break
        if 'error' in response or 'ALARM' in response:
            logging.error(f"Machine Error triggered by '{clean}': {response}")
            # Depending on your workflow, you might want this to sys.exit() 
            # or just log it and drop the command. Exiting here:
            sys.exit(1)

def main():
    parser = argparse.ArgumentParser(description="FIFO Pipe to Serial G-code Worker")
    parser.add_argument("-f", "--fifo", default="/tmp/output_pipe", help="Path to FIFO pipe")
    parser.add_argument("-p", "--port", default="/dev/ttyUSB0", help="Serial port name")
    parser.add_argument("-b", "--baudrate", default=115200, type=int, help="Baud rate")
    parser.add_argument("-d", "--debug", action="store_true", help="Enable verbose debug logging")
    args = parser.parse_args()

    setup_logger(args.debug)

    # === Setup FIFO Pipe ===
    if not os.path.exists(args.fifo):
        logging.info(f"FIFO pipe not found. Creating one at {args.fifo}")
        try:
            os.mkfifo(args.fifo)
        except OSError as e:
            logging.error(f"Failed to create FIFO: {e}")
            sys.exit(1)
    else:
        logging.info(f"Using existing FIFO pipe at {args.fifo}")

    # === Open serial connection ===
    logging.info(f"Connecting to {args.port} at {args.baudrate} baud...")
    try:
        s = serial.Serial(args.port, args.baudrate, timeout=2)
        time.sleep(3) # Allow time for controller to reboot on connection
        init_grbl(s)
    except Exception as e:
        logging.error(f"Failed to open serial port: {e}")
        sys.exit(1)

    # === Main Worker Loop ===
    logging.info("Worker is listening for G-code. Waiting for data...")
    try:
        while True:
            # open() blocks until another process opens the pipe for writing.
            # Once the writer closes its end, the 'for' loop ends, and we loop 
            # back to wait for the next writer connection.
            with open(args.fifo, 'r') as pipe:
                for line in pipe:
                    process_command(s, line)
    except KeyboardInterrupt:
        logging.info("Worker shut down by user.")
    finally:
        if 's' in locals() and s.is_open:
            s.close()
        logging.info("Serial port closed.")

if __name__ == "__main__":
    main()
