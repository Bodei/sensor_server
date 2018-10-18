from pysolar.solar import *
import datetime
import pytz

def solarRadiaiton():
    latitude_deg = 40.50397
    longitude_deg = -74.51874
    date = datetime.datetime.now()
    timezone = pytz.timezone("America/New_York")
    date_aware = timezone.localize(date)
    altitude = get_altitude(latitude_deg, longitude_deg, date_aware)
    rad = radiation.get_radiation_direct(date_aware, altitude) #W/m2
    print(date_aware)
    print(altitude)
    print(rad)
    return rad
