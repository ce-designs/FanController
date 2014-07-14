FanController
=============

This sketch runs two fans temperature controlled fans.

=============

This sketch is optimized for usage with the ATtiny45 or ATtiny85 at 8 mHz. 
For more information about programming the ATtiny visit the High-Low Tech webpage:
http://highlowtech.org/?p=1695

The uC consumes very little power with this sketch, because it goes to sleep after 60 seconds of inactivity. It wakes up after two minutes and checks the temperature. If needed it will turn the fans on and sets their speed accoriding to the measured temperature. It goes back to sleep when the measured temperature stays below the threshhold for as long as 60 seconds.

For more information about the schematics visit the following web page:
http://ce-designs.net/index.php/my-projects/other-builds/temperature-controlled-fans

