> :warning:&nbsp; This repository is no longer maintained. I made this for fun when I had an M1 Mac (it just came out) and got it to work. However, I just tested it on my M3 Pro and something is broken, which I don't have time to fix. 

# Mac Temperatures

This is a simple program to obtain readings from thermal sensors on M1 Macs. I adapted the Objective-C [code](https://github.com/fermion-star/apple_sensors) written by fermion-star as a C program and wrote Python bindings to make the code more flexible and easier to use.

## Install

```shell
pip install mactemperatures
```

## Usage

```python
>>> from mactemperatures import get_thermal_readings
>>> get_thermal_readings()
```
