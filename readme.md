# backlite

Simple program to change the brightness of "intel_backlight" driven monitors on Linux.

### Building:
```
gcc backlite.c -o backlite
```

### Usage:
```
./backlite device_path [+-]brightness
# where:
#  device_path is a path to your intel_backlight device path
#  brightness is the desired brightness expressed as a percentage
#  + or - are optionally used to increment or decrement brightness

# Bash Examples:
sudo backlite $(find /sys/devices | grep intel_backlight$) 50;   # Sets the backlight to 50%
sudo backlite $(find /sys/devices | grep intel_backlight$) +10;  # Increments the backlight by 10%
sudo backlite $(find /sys/devices | grep intel_backlight$) -10;  # Decrements the backlight by 10%

# Fish Examples:
sudo backlite (find /sys/devices | grep 'intel_backlight$') 50;  # Sets the backlight to 50%
sudo backlite (find /sys/devices | grep 'intel_backlight$') +10; # Increments the backlight by 10%
sudo backlite (find /sys/devices | grep 'intel_backlight$') -10; # Decrements the backlight by 10%
```
