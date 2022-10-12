# OPENCHESSBOARD_MidiSequencer
This is a creative use case for using the openchessboard as midi-step-sequencer with Ableton Live.

# SETUP
Two third party programs, LoopMidi and Hairless MIDI, are needed to support the openchessboard as midi device in Ableton Live.

## LoopMIDI allows Ableton to use an external MIDI device.
Download LoopMIDI [here](https://www.tobias-erichsen.de/software/loopmidi.html).
Setup Ableton as shown in this image:
![](/images/ableton_setup.png)

## Hairless MIDI bridges between the serial communication of the openchessboard and the LoopMidi Port
Download Hairless MIDI [here](https://projectgus.github.io/hairless-midiserial).
Setup Hairless Midi as shown in this image:
![](/images/hairless_setting.png)
![](/images/hairless_setting2.png)
Note that the Baud rate must alwas math the Baud rate set in the Arduino project.
