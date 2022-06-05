import macsensors
import platform

from .exceptions import InvalidArchitectureError, InvalidOSError

def check_platform(fn):
    def wrapper():
        if not platform.system() == 'Darwin':
            raise InvalidOSError("Invalid OS: " + platform.system())
        if platform.machine() not in ['arm64']: # 'x86_64' TBD
            raise InvalidArchitectureError("Invalid architecture" + platform.machine())
        return fn()
    return wrapper

@check_platform
def get_thermal_readings():
    return macsensors.get_thermal_readings()