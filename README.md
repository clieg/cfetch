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
**Install:**
| Steps | Commands |
|-------|----------|
| Download coffeetch | $ ```git clone https://github.com/clieg/coffeetch``` |
| Go to coffeetch directory | $ ```cd coffeetch``` |
| Install coffeetch | $ ```make install``` |
| Run coffeetch | $ ``` coffeetch ``` |


**If you do not have a compiler**:
| Steps | Commands |
|-------|----------|
| Download coffeetch | $ ```git clone https://github.com/clieg/coffeetch``` |
| Go to coffeetch directory | $ ```cd coffeetch``` |
| Download compiler | $ ```make gcc``` |
| Install coffeetch | $ ```make install``` |
| Run coffeetch | $ ``` coffeetch ``` |


**Uninstall:**
| Steps | Commands |
|-------|----------|
| Go to coffeetch directory | $ ```cd coffeetch``` |
| Uninstall coffeetch | $ ```make uninstall``` |


## Author
Copyright (c) 2020 [Clint](https://github.com/clieg)

## License
This program is provided under the [GPL-3.0 License](https://github.com/clieg/coffeetch/blob/master/LICENSE). See [LICENSE](https://github.com/clieg/coffeetch/blob/master/LICENSE) for more details.


## Credits
[pfetch](https://github.com/dylanaraps/pfetch): Tux ASCII art
  * ```coffeetch``` shares zero code with ```pfetch```.
  * ```coffeetch``` is written in C, whilst ```pfetch``` is written in Shell.
  * Only the Tux ASCII art was used.
