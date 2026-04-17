#Create named pipes (FIFO) for communication from GUI to Sequencer
mkfifo /tmp/input_pipe
mkfifo /tmp/output_pipe
#Start GUI
python3 ./apps/jwc-gui/main.py &
midori http://127.0.0.1:5041
#Start Serial Communication Script
python3 ./apps/astctl/scripts/seq2ard.py &
# Start Sequencer and restart it if it crashes
SEQUENCER="./build/jwc_embedded"
while true; do
  $SEQUENCER &
  PID=$!
  echo "Started process with PID: $PID"
  wait $PID
  echo "Process with PID $PID has exited. Restarting..."
  sleep 5
done
