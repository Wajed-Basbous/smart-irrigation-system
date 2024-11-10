from sensor_module import read_soil_moisture, read_weather_condition

THRESHOLD_MOISTURE = 30  # Moisture level threshold to activate the irrigation system

def control_irrigation_system():
    soil_moisture = read_soil_moisture()
    is_raining = read_weather_condition()

    print(f"Current soil moisture level: {soil_moisture}%")
    print(f"Is it raining? {'Yes' if is_raining else 'No'}")

    if soil_moisture < THRESHOLD_MOISTURE and not is_raining:
        print("Soil moisture is low and it's not raining. Activating irrigation system...")
    else:
        print("Conditions are sufficient, irrigation system is off.")

if __name__ == "__main__":
    control_irrigation_system()
