# OPENCHESSBOARD_MidiSequencer
This is a creative use case for using the openchessboard as midi-step-sequencer with Ableton Live.

# SETUP
Two third party programs, LoopMidi and Hairless MIDI, are needed to support the openchessboard as midi device in Ableton Live.

## LoopMIDI allows Ableton to use an external MIDI device.
Download LoopMIDI [here](https://www.tobias-erichsen.de/software/loopmidi.html).
Setup Ableton as shown in this image:<br/>
![](/images/ableton_setup.png)<br/>

## Hairless MIDI bridges between the serial communication of the openchessboard and the LoopMidi Port
Download Hairless MIDI [here](https://projectgus.github.io/hairless-midiserial).
Setup Hairless Midi as shown in this image:<br/>
![](/images/hairless_setting2.png)<br/>
Settings of Hairless Midi as shown in this image:<br/>
![](/images/hairless_setting.png)<br/>
Note that the Baud rate must always match the Baud rate defined in the Arduino project.
