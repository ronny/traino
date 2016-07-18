# Traino

A [Pebble] watchface to help young kids establish daily routines by showing large
image icons at certain scheduled times to remind them what they're supposed to do,
e.g. brushing teeth, lunch, bath, reading, bed time, etc.

It only shows the current time (12h format), the currently active 120×120
image based on schedule, and a subtle thin battery level indicator bar at the
top.

The watch will do short double vibrations and will change the image when a new
scheduled event occurs.

It's heavily insipired by the [Octopus] watch. You should check them out.

## Requirements

- A Pebble watch (I'm using Pebble Classic)
- Pebble SDK (I use v3) — see https://developer.pebble.com/sdk/
- A supported phone (I'm using an iPhone)

## How to build the watchface

Assuming you have all the requirements, and have supplied your own images (see
`Images` below), you should be able to do the following:

```
pebble build
```

To upload the app to your pebble

```
pebble install --phone 1.2.3.4
```

where `1.2.3.4` is the IP address of your phone. See [Developer Connection] for more info.

## Images

For my own personal use, I'm using images that I'm not allowed to republish,
so I have omitted them from this repository. You should supply your own images
and put them in the `resources` directory. Check out `package.json` for what
the expected file names are, and `events.h` for what events they're mapped to.

The images should be 120px × 120px, black stroke on white background PNGs,
otherwise you'd need to adjust the source to accommodate.

## Limitations

- Not configurable yet, but you should be able to easily change schedules and images
  in `events.h` and/or `package.json` to your needs
- Untested on other Pebble watches, only on Pebble Classic
- Untested on Android phones

## License

MIT License

Copyright (c) 2016 Ronny Haryanto

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


[Pebble]: https://www.pebble.com
[Octopus]: http://www.octopus.watch/joy
[Developer Connection]: https://developer.pebble.com/guides/tools-and-resources/developer-connection/
