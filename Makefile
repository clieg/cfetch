.PHONY: all
all:
	@echo "Run 'make install' to install cfetch"

.PHONY: install
install:
	@cp -v cfetch /usr/bin/cfetch
	@chmod 755 /usr/bin/cfetch

.PHONY: uninstall
uninstall:
	@rm -rf -v /usr/bin/cfetch
