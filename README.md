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