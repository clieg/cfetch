# Check for updates and install GCC compiler.
gcc:
	sudo apt update && sudo apt install build-essential


# Install coffeetch
install:
	gcc src/coffeetch.c -o src/coffeetch && sudo mv src/coffeetch /usr/bin/


# Uninstall coffeetch
# This will delete coffeetch from the /usr/bin/ directory.
uninstall:
	sudo rm coffeetch /usr/bin/coffeetch
