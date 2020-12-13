<h1 align="center">
    coffeetch <br>
    <img height="300px" widht="100%" src="https://raw.githubusercontent.com/clieg/coffeetch/master/coffeetch.png">
</h1>

<p align="center">
  :coffee: coffeetch: a minimal CLI system information tool written in C.<br>
</p>

<img width="50%" height="100%" align="left" src="https://raw.githubusercontent.com/clieg/coffeetch/master/coffeetch.png">

```coffeetch``` is a minimal CLI system information tool written in C.

It displays the **username** and **hostname**, **OS**, **kernel**, machine **model**, **CPU**, **RAM**, **shell**, number of **packages** installed, and **uptime** information.

If you want your distro to be supported by ```coffeetch```, open an issue.

If there is some incorrect system information shown in ```coffeetch```, open an issue.

<br>

## Configuration

Your distro may or may not come with a C compiler called **GCC**, which is required to build ```coffeetch```. You can check if you have it by running ```gcc --version``` in a terminal. If it outputs GCC version information, you can skip to installing coffeetch. If not, follow the instructions to install the compiler. 

**If you do not have a compiler**:
| Steps | Commands |
|-------|----------|
| Download compiler (Debian/Ubuntu) | $```sudo apt update && sudo apt install build-essential``` |
| Download compiler (Arch) | $```sudo pacman -Sy && sudo pacman -S base-devel``` |

**Install:**
| Steps | Commands |
|-------|----------|
| Download coffeetch | $ ```git clone https://github.com/clieg/coffeetch``` |
| Go to coffeetch directory | $ ```cd coffeetch``` |
| Compile coffeetch | $ ```make``` |
| Install coffeetch | $```make install``` |
| Run coffeetch | $ ``` coffeetch ``` |

**Uninstall:**
| Steps | Commands |
|-------|----------|
| Go to coffeetch directory | $ ```cd coffeetch``` |
| Uninstall coffeetch | $ ```make uninstall``` |


## Author
Copyright (c) 2020 [Clint](https://github.com/clieg)

## License
This programme is provided under the [GPL-3.0 License](https://github.com/clieg/coffeetch/blob/master/LICENSE). See [LICENSE](https://github.com/clieg/coffeetch/blob/master/LICENSE) for more details.


## Credits
[pfetch](https://github.com/dylanaraps/pfetch): Tux ASCII art
  * ```coffeetch``` shares zero code with ```pfetch```.
  * ```coffeetch``` is written in C, whilst ```pfetch``` is written in Shell.
  * Only the Tux ASCII art was used.
