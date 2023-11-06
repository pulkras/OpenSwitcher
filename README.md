# Requirements
## Debian
### For build
`sudo apt-get install build-essential xserver-xorg libx11-dev libxkbcommon-x11-dev libicu-dev xorg-dev`

# Install

1. `git clone -b test https://github.com/pulkras/OpenSwitcher`
2. `cd OpenSwitcher`
3. **Read INSTALL**
4. `./configure`
5. `make`
6. `sudo make install OPENSWITCHER_LOCAL_INSTALL=1`

## Make .deb package
1. `git clone -b test https://github.com/pulkras/OpenSwitcher`
2. `cd OpenSwitcher`
3. `./configure`
4. `make deb`

# Run

1. Select the text
2. Press default Ctr_L+Alt_L combination
3. Voila ! :)

# Run in terminal

1. Select the text
2.  `xkb-switch -n && xsel | sudo openswitcher --device "$(input-device-info.sh)"`

# Docs generating

1.  `sudo apt install doxygen`
