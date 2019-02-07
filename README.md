# CTCSS-tone-encoder
Code created to implement a CTCSS tone encoder board into a Yaesu radio as part of a Electronics Technician internship position

Code was created to run on a PIC16 chip

A tone encoder boards toargets to create a low frequency sin wave that access to a radio tower repeater and allows the user to increase its communication range.

The code written in C creates a PWM signal from a 6 bit input comming from the radio, each combination of input bits has an specific hexadecimal value that is tied to a frequency in the Hz range.
