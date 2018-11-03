#pragma once


#ifndef BASIC
#include <chrono>
#include <vector>
#endif

namespace lamp {

  #ifndef BASIC
  using namespace std;
  using namespace std::chrono;
  #endif

  // Four 12 bit values total 6 bytes
  struct LedValue {
    unsigned int r : 12;
    unsigned int g : 12;
    unsigned int b : 12;
    unsigned int w : 12;
  };

  #ifndef BASIC

  enum AnimationType {
    Linear = 0
  };

  struct KeyFrame {
    milliseconds duration;
    AnimationType type;
    vector<LedValue> frame;
  };

  #endif 

  struct ILedBoardChain {

    virtual ~ILedBoardChain() noexcept = default;

    #ifndef BASIC
    virtual void addKeyframe(KeyFrame keyframe) = 0;
    #endif

    virtual void setAllLeds(LedValue color) = 0;

    // TODO
    // ====
    //  * Set startup animation
    //  * Global brightness adjust
    //  * Get LED status
    //  * Get firmware version
    //  * Firmware upgrade

  };

}