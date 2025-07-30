#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Struct for defining parameter range
struct Range {
  float min;
  float max;
  string name;
};

// Struct for result status and message
struct BreachInfo {
  bool isOk;
  string message;
};

// Reusable function to check only maximum limit
BreachInfo checkMaxOnly(float value, float max, const string& name) {
  if (value > max) {
    return {false, name + " is too high!"};
  }
  return {true, ""};
}

// Reusable function to check range (min optional), uses checkMaxOnly
BreachInfo checkRange(float value, const Range& range, bool hasMin = true) {
  if (hasMin && value < range.min) {
    return {false, range.name + " is too low!"};
  }
  return checkMaxOnly(value, range.max, range.name);  // max check via helper
}

// Centralized reporter for out-of-range messages
void reportIfNotOk(const BreachInfo& info) {
  if (!info.isOk) {
    cout << info.message << endl;
  }
}

// Main battery check function
bool batteryIsOk(float temperature, float soc, float chargeRate) {
  Range tempRange = {0, 45, "Temperature"};
  Range socRange = {20, 80, "State of Charge"};
  Range chargeRateRange = {0, 0.8, "Charge Rate"};

  BreachInfo tempCheck = checkRange(temperature, tempRange);
  BreachInfo socCheck = checkRange(soc, socRange);
  BreachInfo chargeCheck = checkRange(chargeRate, chargeRateRange, false); // skip min check

  reportIfNotOk(tempCheck);
  reportIfNotOk(socCheck);
  reportIfNotOk(chargeCheck);

  return tempCheck.isOk && socCheck.isOk && chargeCheck.isOk;
}

// Unit test cases
void runTests() {
  // Valid test cases
  assert(batteryIsOk(25, 70, 0.7) == true); // Normal
  assert(batteryIsOk(0, 20, 0.0) == true);  // Lower bounds
  assert(batteryIsOk(45, 80, 0.8) == true); // Upper bounds

  // Failures (one parameter each)
  assert(batteryIsOk(-1, 70, 0.7) == false);  // Temperature too low
  assert(batteryIsOk(46, 70, 0.7) == false);  // Temperature too high
  assert(batteryIsOk(25, 19, 0.7) == false);  // SOC too low
  assert(batteryIsOk(25, 81, 0.7) == false);  // SOC too high
  assert(batteryIsOk(25, 70, 0.81) == false); // Charge rate too high

  // Multiple issues
  assert(batteryIsOk(50, 85, 0.9) == false);

  cout << "All tests passed.\n";
}

// Main program
int main() {
  runTests();
  return 0;
}
