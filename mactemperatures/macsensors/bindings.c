#include "sensors.h"

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