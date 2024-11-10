import time
import logging
from sensor_module import read_soil_moisture, read_weather_condition

# Set up logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# Constants
DEFAULT_THRESHOLD_MOISTURE = 30  # Default moisture level threshold to activate the irrigation system
CHECK_INTERVAL = 60  # Interval in seconds between each check

def control_irrigation_system(threshold_moisture=DEFAULT_THRESHOLD_MOISTURE):
    """Controls the irrigation system based on soil moisture and weather conditions."""
    soil_moisture = read_soil_moisture()
    is_raining = read_weather_condition()

    logging.info(f"Current soil moisture level: {soil_moisture}%")
    logging.info(f"Is it raining? {'Yes' if is_raining else 'No'}")

    if soil_moisture < threshold_moisture and not is_raining:
        activate_irrigation()
    else:
        logging.info("Conditions are sufficient, irrigation system remains off.")

def activate_irrigation():
    """Activates the irrigation system."""
    logging.info("Soil moisture is low and it's not raining. Activating irrigation system...")
    # Add code here to activate the irrigation hardware if needed

def monitor_soil_and_weather(threshold_moisture=DEFAULT_THRESHOLD_MOISTURE, check_interval=CHECK_INTERVAL):
    """Continuously monitors soil moisture and weather conditions at regular intervals."""
    while True:
        control_irrigation_system(threshold_moisture)
        time.sleep(check_interval)

if __name__ == "__main__":
    monitor_soil_and_weather()
import time
import logging
from sensor_module import read_soil_moisture, read_weather_condition

# Set up logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# Constants
DEFAULT_THRESHOLD_MOISTURE = 30  # Default moisture level threshold to activate the irrigation system
CHECK_INTERVAL = 60  # Interval in seconds between each check

def control_irrigation_system(threshold_moisture=DEFAULT_THRESHOLD_MOISTURE):
    """Controls the irrigation system based on soil moisture and weather conditions."""
    soil_moisture = read_soil_moisture()
    is_raining = read_weather_condition()

    logging.info(f"Current soil moisture level: {soil_moisture}%")
    logging.info(f"Is it raining? {'Yes' if is_raining else 'No'}")

    if soil_moisture < threshold_moisture and not is_raining:
        activate_irrigation()
    else:
        logging.info("Conditions are sufficient, irrigation system remains off.")

def activate_irrigation():
    """Activates the irrigation system."""
    logging.info("Soil moisture is low and it's not raining. Activating irrigation system...")
    # Add code here to activate the irrigation hardware if needed

def monitor_soil_and_weather(threshold_moisture=DEFAULT_THRESHOLD_MOISTURE, check_interval=CHECK_INTERVAL):
    """Continuously monitors soil moisture and weather conditions at regular intervals."""
    while True:
        control_irrigation_system(threshold_moisture)
        time.sleep(check_interval)

if __name__ == "__main__":
    monitor_soil_and_weather()
