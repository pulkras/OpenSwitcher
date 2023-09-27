bin_PROGRAMS = openswitcher
openswitcher_SOURCES = src/main.c


openswitcher_CFLAGS = $(DEPS_CFLAGS)
openswitcher_LDADD = $(DEPS_LIBS)

install-exec-hook: $(bin_PROGRAMS)
	chmod +x scripts/install.sh
	scripts/install.sh

uninstall-hook:
	sudo rm /etc/sudoers.d/openswitcher

