#pragma once

#include <chrono>
#include <vector>

namespace lamp {

  using namespace std;
  using namespace std::chrono;

  // Four 12 bit values total 6 bytes
  struct LedValue {
    unsigned int r : 12;
    unsigned int g : 12;
    unsigned int b : 12;
    unsigned int w : 12;
  };

  enum AnimationType {
    Linear = 0
  };

  struct KeyFrame {
    milliseconds duration;
    AnimationType type;
    vector<LedValue> frame;
  };

  struct ILedBoardChain {

    virtual ~ILedBoardChain() noexcept = default;

    virtual void addKeyframe(KeyFrame keyframe) = 0;

    // TODO
    // ====
    //  * Set startup animation
    //  * Global brightness adjust
    //  * Get LED status
    //  * Get firmware version
    //  * Firmware upgrade

  };

}