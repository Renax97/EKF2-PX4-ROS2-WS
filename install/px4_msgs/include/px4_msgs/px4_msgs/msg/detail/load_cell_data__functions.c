// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from px4_msgs:msg/LoadCellData.idl
// generated code does not contain a copyright notice
#include "px4_msgs/msg/detail/load_cell_data__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
px4_msgs__msg__LoadCellData__init(px4_msgs__msg__LoadCellData * msg)
{
  if (!msg) {
    return false;
  }
  // timestamp
  // force_x
  // force_y
  // force_z
  // torque_x
  // torque_y
  // torque_z
  return true;
}

void
px4_msgs__msg__LoadCellData__fini(px4_msgs__msg__LoadCellData * msg)
{
  if (!msg) {
    return;
  }
  // timestamp
  // force_x
  // force_y
  // force_z
  // torque_x
  // torque_y
  // torque_z
}

bool
px4_msgs__msg__LoadCellData__are_equal(const px4_msgs__msg__LoadCellData * lhs, const px4_msgs__msg__LoadCellData * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // timestamp
  if (lhs->timestamp != rhs->timestamp) {
    return false;
  }
  // force_x
  if (lhs->force_x != rhs->force_x) {
    return false;
  }
  // force_y
  if (lhs->force_y != rhs->force_y) {
    return false;
  }
  // force_z
  if (lhs->force_z != rhs->force_z) {
    return false;
  }
  // torque_x
  if (lhs->torque_x != rhs->torque_x) {
    return false;
  }
  // torque_y
  if (lhs->torque_y != rhs->torque_y) {
    return false;
  }
  // torque_z
  if (lhs->torque_z != rhs->torque_z) {
    return false;
  }
  return true;
}

bool
px4_msgs__msg__LoadCellData__copy(
  const px4_msgs__msg__LoadCellData * input,
  px4_msgs__msg__LoadCellData * output)
{
  if (!input || !output) {
    return false;
  }
  // timestamp
  output->timestamp = input->timestamp;
  // force_x
  output->force_x = input->force_x;
  // force_y
  output->force_y = input->force_y;
  // force_z
  output->force_z = input->force_z;
  // torque_x
  output->torque_x = input->torque_x;
  // torque_y
  output->torque_y = input->torque_y;
  // torque_z
  output->torque_z = input->torque_z;
  return true;
}

px4_msgs__msg__LoadCellData *
px4_msgs__msg__LoadCellData__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  px4_msgs__msg__LoadCellData * msg = (px4_msgs__msg__LoadCellData *)allocator.allocate(sizeof(px4_msgs__msg__LoadCellData), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(px4_msgs__msg__LoadCellData));
  bool success = px4_msgs__msg__LoadCellData__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
px4_msgs__msg__LoadCellData__destroy(px4_msgs__msg__LoadCellData * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    px4_msgs__msg__LoadCellData__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
px4_msgs__msg__LoadCellData__Sequence__init(px4_msgs__msg__LoadCellData__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  px4_msgs__msg__LoadCellData * data = NULL;

  if (size) {
    data = (px4_msgs__msg__LoadCellData *)allocator.zero_allocate(size, sizeof(px4_msgs__msg__LoadCellData), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = px4_msgs__msg__LoadCellData__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        px4_msgs__msg__LoadCellData__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
px4_msgs__msg__LoadCellData__Sequence__fini(px4_msgs__msg__LoadCellData__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      px4_msgs__msg__LoadCellData__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

px4_msgs__msg__LoadCellData__Sequence *
px4_msgs__msg__LoadCellData__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  px4_msgs__msg__LoadCellData__Sequence * array = (px4_msgs__msg__LoadCellData__Sequence *)allocator.allocate(sizeof(px4_msgs__msg__LoadCellData__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = px4_msgs__msg__LoadCellData__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
px4_msgs__msg__LoadCellData__Sequence__destroy(px4_msgs__msg__LoadCellData__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    px4_msgs__msg__LoadCellData__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
px4_msgs__msg__LoadCellData__Sequence__are_equal(const px4_msgs__msg__LoadCellData__Sequence * lhs, const px4_msgs__msg__LoadCellData__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!px4_msgs__msg__LoadCellData__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
px4_msgs__msg__LoadCellData__Sequence__copy(
  const px4_msgs__msg__LoadCellData__Sequence * input,
  px4_msgs__msg__LoadCellData__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(px4_msgs__msg__LoadCellData);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    px4_msgs__msg__LoadCellData * data =
      (px4_msgs__msg__LoadCellData *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!px4_msgs__msg__LoadCellData__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          px4_msgs__msg__LoadCellData__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!px4_msgs__msg__LoadCellData__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
