from pysolar.solar import *
import datetime
import pytz

latitude_deg = 40.50397
longitude_deg = -74.51874

date = datetime.datetime.now()
timezone = pytz.timezone("America/New_York")
date_aware = timezone.localize(date)
altitude = get_altitude(latitude_deg, longitude_deg, date_aware)
radiation = radiation.get_radiation_direct(date_aware, altitude)
print(altitude)
print(radiation)