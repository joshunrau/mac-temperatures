#define PY_SSIZE_T_CLEAN
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hidsystem/IOHIDEventSystemClient.h>
#include <Python.h>

typedef struct __IOHIDEvent *IOHIDEventRef;
typedef struct __IOHIDServiceClient *IOHIDServiceClientRef;
typedef double IOHIDFloat;

// from IOHIDFamily/IOHIDEventTypes.h
// e.g., https://opensource.apple.com/source/IOHIDFamily/IOHIDFamily-701.60.2/IOHIDFamily/IOHIDEventTypes.h.auto.html

#define IOHIDEventFieldBase(type) (type << 16)
#define kIOHIDEventTypeTemperature 15

IOHIDEventSystemClientRef IOHIDEventSystemClientCreate(CFAllocatorRef allocator);
int IOHIDEventSystemClientSetMatching(IOHIDEventSystemClientRef client, CFDictionaryRef match);
int IOHIDEventSystemClientSetMatchingMultiple(IOHIDEventSystemClientRef client, CFArrayRef match);
IOHIDEventRef IOHIDServiceClientCopyEvent(IOHIDServiceClientRef, int64_t, int32_t, int64_t);
CFStringRef IOHIDServiceClientCopyProperty(IOHIDServiceClientRef service, CFStringRef property);
IOHIDFloat IOHIDEventGetFloatValue(IOHIDEventRef event, int32_t field);

// create a dict ref, like for temperature sensor {"PrimaryUsagePage":0xff00, "PrimaryUsage":0x5}
CFDictionaryRef matching(int page, int usage) {
	CFNumberRef nums[2];
	CFStringRef keys[2];

	keys[0] = CFStringCreateWithCString(0, "PrimaryUsagePage", 0);
	keys[1] = CFStringCreateWithCString(0, "PrimaryUsage", 0);
	nums[0] = CFNumberCreate(0, kCFNumberSInt32Type, &page);
	nums[1] = CFNumberCreate(0, kCFNumberSInt32Type, &usage);

	CFDictionaryRef dict = CFDictionaryCreate(0, (const void **)keys, (const void **)nums, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFRelease(keys[0]);
	CFRelease(keys[1]);
	return dict;
}

CFArrayRef getSensorNames(CFDictionaryRef sensors) {
    IOHIDEventSystemClientRef system = IOHIDEventSystemClientCreate(kCFAllocatorDefault); // in CFBase.h = NULL
    // ... this is the same as using kCFAllocatorDefault or the return value from CFAllocatorGetDefault()
    IOHIDEventSystemClientSetMatching(system, sensors);
    CFArrayRef matchingsrvs = IOHIDEventSystemClientCopyServices(system); // matchingsrvs = matching services

    long count = CFArrayGetCount(matchingsrvs);
    CFMutableArrayRef array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

    for (int i = 0; i < count; i++) {
        IOHIDServiceClientRef sc = (IOHIDServiceClientRef)CFArrayGetValueAtIndex(matchingsrvs, i);
        CFStringRef name = IOHIDServiceClientCopyProperty(sc, CFSTR("Product")); // here we use ...CopyProperty
        if (name) {
            CFArrayAppendValue(array, name);
        } else {
            CFArrayAppendValue(array, CFSTR("noname"));
        }
    }
    return array;
}

CFArrayRef getSensorReadings(CFDictionaryRef sensors) {
    IOHIDEventSystemClientRef system = IOHIDEventSystemClientCreate(kCFAllocatorDefault);
    IOHIDEventSystemClientSetMatching(system, sensors);
    CFArrayRef matchingsrvs = IOHIDEventSystemClientCopyServices(system);

    long count = CFArrayGetCount(matchingsrvs);
    CFMutableArrayRef array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

    for (int i = 0; i < count; i++) {
        IOHIDServiceClientRef sc = (IOHIDServiceClientRef)CFArrayGetValueAtIndex(matchingsrvs, i);
        IOHIDEventRef event = IOHIDServiceClientCopyEvent(sc, kIOHIDEventTypeTemperature, 0, 0); // here we use ...CopyEvent

        CFNumberRef value;
        if (event != 0) {
            double temp = IOHIDEventGetFloatValue(event, IOHIDEventFieldBase(kIOHIDEventTypeTemperature));
            value = CFNumberCreate(kCFAllocatorDefault, kCFNumberDoubleType, &temp);
        } else {
            double temp = 0;
            value = CFNumberCreate(kCFAllocatorDefault, kCFNumberDoubleType, &temp);
        }
        CFArrayAppendValue(array, value);
    }
    return array;
}

PyObject* getThermalReadings(PyObject* self) {
    
    CFDictionaryRef thermalSensors = matching(0xff00, 5);
    CFArrayRef sensorNames = getSensorNames(thermalSensors);
    CFIndex lengthSensorNames = CFArrayGetCount(sensorNames);
    CFArrayRef sensorValues = getSensorReadings(thermalSensors);
    CFIndex lengthSensorValues = CFArrayGetCount(sensorValues);
    
    if (lengthSensorNames != lengthSensorValues) {
        return NULL;
    }

    PyObject *sensorReadings = PyDict_New();
    
    for (int i = 0; i < lengthSensorValues; i++) {
        CFStringRef name = CFArrayGetValueAtIndex(sensorNames, i);
        CFNumberRef value = CFArrayGetValueAtIndex(sensorValues, i);

        char buffer[256];
        CFStringGetCString(name, buffer, 256, kCFStringEncodingUTF8);

        double temp;
        CFNumberGetValue(value, kCFNumberDoubleType, &temp);

        PyDict_SetItem(sensorReadings, PyUnicode_FromString(buffer), PyFloat_FromDouble(temp));
    }
    return sensorReadings;
};

static struct PyMethodDef moduleMethods[] = {
    {"get_thermal_readings", (PyCFunction)getThermalReadings, METH_NOARGS},
    {NULL, NULL, NULL}
};

static struct PyModuleDef macsensorsModule = {
    PyModuleDef_HEAD_INIT,
    "macsensors",
    NULL,
    -1,
    moduleMethods
};

PyMODINIT_FUNC PyInit_macsensors(void) {
    return PyModule_Create(&macsensorsModule);
}