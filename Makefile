# Install coffeetch
# This will check for updates and check if build-essential is installed. It will install build-essential if the system does not have one. Otherwise the compilation will start and proceed to install coffeetch.
install:
	sudo apt update && sudo apt install build-essential && gcc src/coffeetch.c -o src/coffeetch && sudo mv src/coffeetch /usr/bin/

# Uninstall coffeetch
# This will delete coffeetch from the /usr/bin/ directory.
uninstall:
	sudo rm coffeetch /usr/bin/coffeetch
