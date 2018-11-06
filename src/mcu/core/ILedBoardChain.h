#pragma once

#include <cstdint>

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

  struct ChainInfo {   
    uint8_t boards;    // 30
    uint8_t drivers;   // 60
    uint8_t ledgroups; // 240
    uint16_t channels; // 960
    uint16_t bytes;    // 1440

    ChainInfo(uint8_t ledboards) {
      boards = ledboards;
      drivers = boards * 2;
      ledgroups = drivers * 4;
      channels = drivers * 16;
      bytes = drivers * 24;
    }

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
    // Todo: Set startup animation
    // Todo: Get LED status
    // Todo: Get firmware version
    // Todo: Firmware upgrade
    #endif

    virtual void setAllLeds(LedValue color) = 0;
    virtual void setAllLeds(LedValue values[]) = 0;
    virtual void setBrightness(uint8_t brightness) = 0;
    virtual void setBrightness(uint8_t values[]) = 0;
    virtual ChainInfo* getChainInfo() = 0;

  };

}