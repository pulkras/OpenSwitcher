VERSION = @PACKAGE_VERSION@
OPENSWITCHER_INSTALL = 0

bin_PROGRAMS = openswitcher
openswitcher_SOURCES = src/main.c


openswitcher_CFLAGS = $(DEPS_CFLAGS)
openswitcher_LDADD = $(DEPS_LIBS)

bin_SCRIPTS = scripts/openswitcher.sh
EXTRA_DIST = scripts/openswitcher.sh.in

CLEANFILES = $(bin_SCRIPTS)

man1_MANS = doc/openswitcher.1


EXTRA_DIST += xkb-switch

EXTRA_DIST += actkbd

EXTRA_DIST += debian


install-exec-hook: $(bin_PROGRAMS)
	@if [ $(OPENSWITCHER_INSTALL) = 1 ]; then\
		scripts/install.sh;\
	fi

uninstall-hook:
	@if [ $(OPENSWITCHER_INSTALL) = 1 ]; then\
		scripts/uninstall.sh;\
	fi

distclean-local:
	rm -rf openswitcher*

$(PACKAGE_TARNAME)-$(VERSION).tar.gz: $(bin_PROGRAMS)
	$(MAKE) dist

deb: $(PACKAGE_TARNAME)-$(VERSION).tar.gz
	cp $(PACKAGE_TARNAME)-$(VERSION).tar.gz $(PACKAGE_TARNAME)_$(VERSION).orig.tar.gz
	mkdir -p $(PWD)/$(PACKAGE_TARNAME)-$(VERSION)
	tar -xzf $(PACKAGE_TARNAME)-$(VERSION).tar.gz
	cd $(PACKAGE_TARNAME)-$(VERSION) && pwd && debuild
	
