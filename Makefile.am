bin_PROGRAMS = openswitcher
openswitcher_SOURCES = src/main.c


openswitcher_CFLAGS = $(DEPS_CFLAGS)
openswitcher_LDADD = $(DEPS_LIBS)

bin_SCRIPTS = scripts/openswitcher.sh
EXTRA_DIST = scripts/openswitcher.sh.in

CLEANFILES = $(bin_SCRIPTS)

dist_man1_MANS = doc/openswitcher.1


install-exec-hook: $(bin_PROGRAMS)
	scripts/install.sh

uninstall-hook:
	scripts/uninstall.sh

