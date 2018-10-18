from smbus import SMBus
import time
import requests

bus = SMBus(1)
address = 8

dweetIO = "https://dweet.io/dweet/for/"
myName = "rutgers_solar_dashboard"
myKeyHum = "Humidity"
myKeyTemp = "&Temperature"

def readLong():
    block = bus.read_i2c_block_data(address,0)
    list_char1 = [chr(i) for i in block[0:6]]
    list_char2 = [chr(i) for i in block[7:11]]
    list_char3 = [chr(i) for i in block[12:18]]
    n1 = "".join(list_char1)
    n2 = "".join(list_char2)
    n3 = "".join(list_char3)
    return n1, n2, n3

def readByte():
    b = bus.read_byte_data(address,0)
    return b

while True:
    time.sleep(.5)
    number1, number2, number3 = readLong()
    print("Humidity: %s" %number1, "%")
    print("Temperature: %s" %number2, "F")
    print("Solar Panel Current: %s" %number3, "mA")
    rqsString = dweetIO+myName+'?'+myKeyHum+'='+str(number1)+myKeyTemp+'='+str(number2)
    rqs = requests.get(rqsString)
