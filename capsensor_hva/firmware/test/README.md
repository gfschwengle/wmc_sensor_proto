# Test Directory

This directory contains unit tests for the firmware.

## Purpose

Write unit tests to verify firmware functionality without requiring hardware.

## Framework

PlatformIO uses Unity test framework by default.

## Writing Tests

Create test files with `test_*.cpp` naming:

```cpp
#include <unity.h>
#include "mymodule.h"

void test_function_behavior() {
    int result = myFunction(5);
    TEST_ASSERT_EQUAL(10, result);
}

void setUp(void) {
    // Setup code before each test
}

void tearDown(void) {
    // Cleanup code after each test
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_function_behavior);
    UNITY_END();
}

void loop() {
    // Leave empty
}
```

## Running Tests

```bash
# Run all tests
pio test

# Run specific test
pio test -f test_sensor

# Run tests on specific environment
pio test -e esp32dev
```

## Best Practices

- Test small units of functionality
- Use descriptive test names
- Mock hardware dependencies when possible
- Aim for good test coverage
- Keep tests fast and focused

## Resources

- [PlatformIO Testing](https://docs.platformio.org/en/latest/plus/unit-testing.html)
- [Unity Framework](https://github.com/ThrowTheSwitch/Unity)
