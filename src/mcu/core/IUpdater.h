#pragma once

#include <string>
#include <cstdint>
#include <vector>

using namespace std;

namespace lamp {

  struct IUpdater {

    virtual ~IUpdater() noexcept = default;

	virtual string getRunningVersion() = 0;
	virtual string getRunningFpgaHash() = 0;
	virtual string getInstalledFpgaHash() = 0;

	virtual bool beginUpgrade() = 0;
	virtual bool writeChunk(vector<uint8_t> chunk) = 0;
	virtual bool completeUpgrade() = 0;

	virtual bool flashFpga() = 0;

  };

}
