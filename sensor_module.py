import random

def read_soil_moisture():
    """Simulate reading soil moisture level (0-100)."""
    return random.randint(0, 100)

def read_weather_condition():
    """Simulate reading weather condition (True for rain, False for no rain)."""
    return random.choice([True, False])
