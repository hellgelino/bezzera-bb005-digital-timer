# bezzera-bb005-digital-timer based on Arduino:
Bezzera BB005 modifikation to setup your grinding Time via digital Timer based on Arduino Node32

## This Modifikation features:
- You can install this mod without any modifikation on the original Hardware
- Get 1 or 2 Cups in a difinded time grinded and be able to abort the grinding
- Define your time for each Cup via a Standalone WLAN in extra Servermode
- Demo: https://youtu.be/tWOO4BNrsw0

## This Hardware is needed to run this Software
- Arduino - we choose NodeMCU Amica Modul V2 ESP8266 ESP-12F WIFI Development Board with CP2102
- OLED Display - we choose 1,3" OLED Display I2C 128 x 64 Pixel I2C
- USB Power Supplie - we choose MeanWell RS-15-5 Netzteilbaustein, 15 W, 5 V, 3 A
- Digital Relay - we choose a 1-Channel-Relay-Modul, 5V, with Optocoupler, High and Low Level Trigger, for Arduino

## circuit diagram
- see the file in sources: grinder-timer.png

## How to Setup the grinding Time (Servermode):
1. Press the Button and hold it while powering on the Grinder - Now it shows you some Informations in the Display
2. Via your MobilePhone, Notebook, etc. pp. you can conncet to the WLAN Gateway (Name: Grinder-Configuration) and then access a small Website via a Browser on your Device
3. Set the Time for 1 or 2 Cups (Type http://192.168.4.1 in your Browser) and follow the content of the site.

## Changelog
Feel free to optimize your own Branch of this Code, we will not Change this one.

# Special Thanks
To the great work Joerky!
  
!! Thank you so much for the tasty cup of coffee I enjoy each day !!  


# Disclaimer
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
