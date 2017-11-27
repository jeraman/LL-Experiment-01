# Prototyping Circuitry

As materials, we are going to need:
- A 5v Arduino;
- A [Bluefruit board](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/configuration) (Bluefruit LE UART Friend);
- A [button](https://www.adafruit.com/product/3489);
- A bunch of jumpers;

That said, these are the steps to get the 'pedal' working:
- [x] Get started on Arduino;
- [x] Bluetooth BLE board (Bluefruit & Arduino);
- [x] MIDI on Arduino;
- [x] MIDI BLE from Arduino to a Mobile;
- [ ] Connect the button;
- [ ] Connect the two potentiometers;
- [ ] Test all together with independent power;
- [ ] Record a video of the prototype;

# Get started on Arduino!
Because there was no cable (get it ASAP!), decided to use my old Arduino Uno (still working!) instead of the brand new Arduino Micro. I also updated Arduino's IDE software.

# Bluetooth BLE board
First, I soldered the connectors into the board. Then, I tested to see if the module was working all right by following the tutorial [here](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend).

In short, you'll need:

## Wiring
Just like in this image:
![wiring](https://cdn-learn.adafruit.com/assets/assets/000/025/182/medium800/adafruit_products_UARTFriend_bb.png)

## Setting Adafruit's BLE on Arduino
For this I used [this tutorial](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/software).

The tutorial is effective, covering things such as [configuring the board](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/configuration); sketch examples (e.g. using [AT commands](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/atcommand) from your computer to the board). By the end, you'll hopefully get your arduino, your BLE board, and you computer via serial port!

PS: Detailed documentation of AT commands can be found [here](https://learn.adafruit.com/introducing-adafruit-ble-bluetooth-low-energy-friend/standard-at).

## Testing the BLE with a tablet/smartphone
You can test communication with your Android/iOs doing as demonstrate in [this video](https://www.youtube.com/watch?v=4hWlDKzn7tA).

# Midi on Arduino
Arduino has a [MIDI library](https://playground.arduino.cc/Main/MIDILibrary). By following this tutorial [here](http://bvavra.github.io/MIDI_Accordion/sending-midi/), I was able to send continuous MIDI messages (rate: 1 per second) to my computer via serial. The code is available [here](../sketches/midi-demo/midi-demo.ino).

**Sidenote:** If I was to synthesize these notes on the computer, we would need someone like [the Hairless MIDI](http://projectgus.github.io/hairless-midiserial/) to route messages from serial to your MIDI synth. For my purposes, won't bother doing that.

# MIDI BLE from Arduino to a Mobile
Afterwards, my goal moved towards replicating the same 'MIDI on Arduino' example now over BLE to a mobile device, which will synthesize the notes.

It wasn't hard. The default MIDI example available on the Adafruits'BLE library seemed to have worked (almost) out-of-the-box:

- Inside the code, I needed to adjust the [configurations](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/configuration) to use Software Serial (I have an Arduino Uno);
- Also needed to use Adafruit's BLE connect app to connect to the board;
- To route the MIDI to GarageBand, I used the [MidiMttr](https://itunes.apple.com/us/app/midimittr/id925495245?mt=8) app.

I think that was all. The sketch used is available [here](../sketches/midi-ble/midi-ble).

**Sidenote 1:** Very often, I get errors when uploading the sketches to the board. These errors differ: Sometimes I get "Couldn't factory reset"; Sometimes, I get "Could not enable MIDI". Sometimes, fortunately, it works fine (i.e. "waiting for a connection"). Need to study why that happens.

**Sidenote 2:** Need to study how to make the BLE connection more reliable & straightforward (e.g. no need of external Adafruit's or MidiMttr app).

# Connecting the button
Regarding [Arcade buttons I use](https://www.adafruit.com/product/3489), there are two pairs of connectors: one pair for controlling the LEDs, another for using the button itself.

For the LED connectors, [specification](https://www.adafruit.com/product/3489#technical-details-anchor) says there is no need to use resistors so that it is possible to connect the button directly to the 5v and GND (these have built-in 1k resistors).

For the button, you can follow [this basic tutorial ](https://www.arduino.cc/en/Tutorial/Button) in order to read the button inside Arduino. Here, I used a 10k resistor.

I've modified this demo to work with the LEDs: if the button is pressed, the LED will light up. The code is [here](../sketches/arcade-button-LED/arcade-button-LED/ino).
