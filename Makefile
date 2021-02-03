.PHONY: all
all:
	@echo "Run 'make install' to install cfetch"

.PHONY: install
install:
	@cp -v cfetch /usr/bin/cfetch
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/cfetch

.PHONY: uninstall
uninstall:
	@rm -rf -v $(DESTDIR)$(PREFIX)/bin/cfetch
