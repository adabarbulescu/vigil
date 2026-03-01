#!/bin/bash
# stress.sh — generates CPU load for vigil demonstration
# Usage: ./scripts/stress.sh [num_workers] [duration_seconds]

WORKERS=${1:-4}
DURATION=${2:-30}

echo "Starting $WORKERS CPU workers for ${DURATION}s..."
echo "Run ./vigil in another terminal to observe"

pids=()

for i in $(seq 1 $WORKERS); do
    python3 -c "
import time
end = time.time() + $DURATION
while time.time() < end:
    x = 99999 ** 99999 % 100003
" &
    pids+=($!)
    echo "Worker $i started (PID $!)"
done

for pid in "${pids[@]}"; do
    wait $pid
done

echo "Done."
