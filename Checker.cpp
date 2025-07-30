#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Struct for range definition
struct Range {
  float min;
  float max;
  string name;
};

// Struct for breach result
struct BreachInfo {
  bool isOk;
  string message;
};

// Generic range checker (for bounded parameters)
BreachInfo checkRange(float value, const Range& range, bool hasMin = true) {
  if (hasMin && value < range.min) {
    return {false, range.name + " is too low!"};
  }
  if (value > range.max) {
    return {false, range.name + " is too high!"};
  }
  return {true, ""};
}

// Main battery check function
bool batteryIsOk(float temperature, float soc, float chargeRate) {
  Range temperatureRange = {0, 45, "Temperature"};
  Range socRange = {20, 80, "State of Charge"};
  Range chargeRateRange = {0, 0.8, "Charge Rate"}; // Min 0 used for clarity, not enforced

  BreachInfo tempCheck = checkRange(temperature, temperatureRange);
  BreachInfo socCheck = checkRange(soc, socRange);
  BreachInfo crCheck = checkRange(chargeRate, chargeRateRange, false); // no min check

  if (!tempCheck.isOk) {
    cout << tempCheck.message << endl;
    return false;
  }
  if (!socCheck.isOk) {
    cout << socCheck.message << endl;
    return false;
  }
  if (!crCheck.isOk) {
    cout << crCheck.message << endl;
    return false;
  }
  return true;
}

// Unit test cases
void runTests() {
  // Valid case
  assert(batteryIsOk(25, 70, 0.7) == true);

  // Boundary values
  assert(batteryIsOk(0, 20, 0.0) == true);
  assert(batteryIsOk(45, 80, 0.8) == true);

  // Failures
  assert(batteryIsOk(-1, 70, 0.7) == false);  // Temperature too low
  assert(batteryIsOk(46, 70, 0.7) == false);  // Temperature too high
  assert(batteryIsOk(25, 19, 0.7) == false);  // SOC too low
  assert(batteryIsOk(25, 81, 0.7) == false);  // SOC too high
  assert(batteryIsOk(25, 70, 0.81) == false); // Charge Rate too high

  // Multiple errors
  assert(batteryIsOk(50, 85, 0.9) == false);

  cout << "All tests passed.\n";
}

// Main program
int main() {
  runTests();
  return 0;
}
