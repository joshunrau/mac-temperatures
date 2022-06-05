class MacTemperaturesError(Exception):
    pass

class InvalidOSError(MacTemperaturesError):
    pass

class InvalidArchitectureError(MacTemperaturesError):
    pass
