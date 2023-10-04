bin_PROGRAMS = openswitcher
openswitcher_SOURCES = src/main.c


openswitcher_CFLAGS = $(DEPS_CFLAGS)
openswitcher_LDADD = $(DEPS_LIBS)

bin_SCRIPTS = scripts/openswitcher.sh
EXTRA_DIST = scripts/openswitcher.sh.in

bin_SCRIPTS += scripts/switchLayout.sh
EXTRA_DIST += scripts/switchLayout.sh.in

CLEANFILES = $(bin_SCRIPTS)

TEST_EXTENSIONS = .sh


install-exec-hook: $(bin_PROGRAMS)
	scripts/install.sh

uninstall-hook:
	scripts/uninstall.sh

