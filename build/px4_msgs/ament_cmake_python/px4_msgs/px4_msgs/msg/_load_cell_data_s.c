// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from px4_msgs:msg/LoadCellData.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "px4_msgs/msg/detail/load_cell_data__struct.h"
#include "px4_msgs/msg/detail/load_cell_data__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool px4_msgs__msg__load_cell_data__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[42];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("px4_msgs.msg._load_cell_data.LoadCellData", full_classname_dest, 41) == 0);
  }
  px4_msgs__msg__LoadCellData * ros_message = _ros_message;
  {  // timestamp
    PyObject * field = PyObject_GetAttrString(_pymsg, "timestamp");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->timestamp = PyLong_AsUnsignedLongLong(field);
    Py_DECREF(field);
  }
  {  // force_x
    PyObject * field = PyObject_GetAttrString(_pymsg, "force_x");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->force_x = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // force_y
    PyObject * field = PyObject_GetAttrString(_pymsg, "force_y");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->force_y = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // force_z
    PyObject * field = PyObject_GetAttrString(_pymsg, "force_z");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->force_z = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // torque_x
    PyObject * field = PyObject_GetAttrString(_pymsg, "torque_x");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->torque_x = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // torque_y
    PyObject * field = PyObject_GetAttrString(_pymsg, "torque_y");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->torque_y = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // torque_z
    PyObject * field = PyObject_GetAttrString(_pymsg, "torque_z");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->torque_z = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * px4_msgs__msg__load_cell_data__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of LoadCellData */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("px4_msgs.msg._load_cell_data");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "LoadCellData");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  px4_msgs__msg__LoadCellData * ros_message = (px4_msgs__msg__LoadCellData *)raw_ros_message;
  {  // timestamp
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLongLong(ros_message->timestamp);
    {
      int rc = PyObject_SetAttrString(_pymessage, "timestamp", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // force_x
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->force_x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "force_x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // force_y
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->force_y);
    {
      int rc = PyObject_SetAttrString(_pymessage, "force_y", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // force_z
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->force_z);
    {
      int rc = PyObject_SetAttrString(_pymessage, "force_z", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // torque_x
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->torque_x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "torque_x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // torque_y
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->torque_y);
    {
      int rc = PyObject_SetAttrString(_pymessage, "torque_y", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // torque_z
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->torque_z);
    {
      int rc = PyObject_SetAttrString(_pymessage, "torque_z", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
