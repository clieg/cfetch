# Compile coffeetch
all: src/coffeetch.c
	gcc src/coffeetch.c -o coffeetch

clean:
	rm -f coffeetch

# Install coffeetch
# This will move coffeetch to the /usr/bin directory
install:
	sudo cp coffeetch /usr/bin

# Uninstall coffeetch
# This will delete coffeetch from the /usr/bin/ directory.
uninstall:
	sudo rm /usr/bin/coffeetch
