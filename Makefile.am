bin_PROGRAMS = openswitcher
openswitcher_SOURCES = src/main.c


openswitcher_CFLAGS = $(DEPS_CFLAGS)
openswitcher_LDADD = $(DEPS_LIBS)

bin_SCRIPTS = scripts/openswitcher.sh
EXTRA_DIST = scripts/openswitcher.sh.in

CLEANFILES = $(bin_SCRIPTS)

man1_MANS = doc/openswitcher.1


EXTRA_DIST += xkb-switch/.gitignore
EXTRA_DIST += xkb-switch/.vimrc_local.vim
EXTRA_DIST += xkb-switch/CMakeLists.txt
EXTRA_DIST += xkb-switch/COPYING
EXTRA_DIST += xkb-switch/default.nix
EXTRA_DIST += xkb-switch/localrc.vim
EXTRA_DIST += xkb-switch/README.md
EXTRA_DIST += xkb-switch/test.sh
EXTRA_DIST += xkb-switch/xkb-group.sh
EXTRA_DIST += xkb-switch/man/xkb-switch.1
EXTRA_DIST += xkb-switch/src/Utils.hpp
EXTRA_DIST += xkb-switch/src/XKbSwitch.cpp
EXTRA_DIST += xkb-switch/src/XKbSwitchApi.cpp
EXTRA_DIST += xkb-switch/src/XKeyboard.cpp
EXTRA_DIST += xkb-switch/src/XKeyboard.hpp





EXTRA_DIST += actkbd/actkbd.c
EXTRA_DIST += actkbd/actkbd.h
EXTRA_DIST += actkbd/AUTHORS
EXTRA_DIST += actkbd/ChangeLog
EXTRA_DIST += actkbd/config.c
EXTRA_DIST += actkbd/COPYING
EXTRA_DIST += actkbd/FAQ
EXTRA_DIST += actkbd/linux.c
EXTRA_DIST += actkbd/Makefile
EXTRA_DIST += actkbd/mask.c
EXTRA_DIST += actkbd/NEWS
EXTRA_DIST += actkbd/README
EXTRA_DIST += actkbd/TODO
EXTRA_DIST += actkbd/samples/actkbd.conf
EXTRA_DIST += actkbd/samples/udev.rules


install-exec-hook: $(bin_PROGRAMS)
	scripts/install.sh

uninstall-hook:
	scripts/uninstall.sh

