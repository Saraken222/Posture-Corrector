import requests
import time
import random
from datetime import datetime

URL = "http://127.0.0.1:8000/events"

tilt_threshold = 7.5
down_threshold = -0.7

#simulated baselines
tilt_baseline = 0.0
y_baseline = 9.8

while (1):
    pitch = round(random.uniform(0, 5), 2)
    y_value = round(random.uniform(8.5, 10.2), 2)

    tilt_change = pitch - tilt_baseline
    y_change = y_value - y_baseline
    bad_forward = tilt_change > tilt_threshold
    bad_down = y_change < down_threshold

    if bad_forward: status = "BAD_FORWARD"
    elif bad_down: status = "BAD_DOWN"
    else: status = "GOOD"

    event = {
        "timestamp": datetime.now().isoformat(),
        "pitch": pitch,
        "status": status
    }
    response = requests.post(URL, json=event)

    #debug
    print("Sent:", event)
    print("tilt_change:", round(tilt_change, 2), "y_change:", round(y_change, 2))
    print("Response:", response.json())
    print("-----")

    time.sleep(1)







