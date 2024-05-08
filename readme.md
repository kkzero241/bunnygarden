# Bunny Garden
![Bunny Garden](bunnygarden.gif)

A DS homebrew tech demo where one can hop around some 3D platforms with some bouncing bunnies.

Made as my degree's final project.

Relies on [BlocksDS](https://github.com/blocksds/sdk/), [Nitro Engine](https://github.com/AntonioND/nitro-engine), and [NightFox's Lib](https://github.com/knightfox75/nds_nflib), and based on [a template using the latter two](https://github.com/AntonioND/nitro-engine/tree/master/examples/templates/using_nflib).

## Controls
**D-Pad -** Move forward, backward, or to the sides.

**L/R Buttons -** Turn to the sides.

**A Button -** Jump or scale a platform/object.

**Start Button -** Pause gameplay.

**Select Button -** Exit to loader.

## Building
After [setting up BlocksDS, Nitro Engine, and NightFox's Lib via Wonderful Toolchain](https://blocksds.github.io/docs/setup/options/), run `make` in the project directory.

Modified assets can be rebaked into `nitrofiles` by running `convert.sh` in `assets`.

## License
This project uses the MIT license.

If you use this as a launchpad into other BlocksDS things, you may be interested in [the licensing info about the libraries the SDK is comprised of](https://blocksds.github.io/docs/introduction/licenses/).