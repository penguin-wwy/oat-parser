#include <string>

enum InstructionSet {
	kNone,
	kArm,
	kArm64,
	kThumb2,
	kX86,
	kX86_64,
	kMips,
	kMips64
};

enum InstructionFeatures {
	kHwDiv = 0x1,              // Supports hardware divide.
	kHwLpae = 0x2,              // Supports Large Physical Address Extension.
};

class InstructionSetFeatures{
public:
	InstructionSetFeatures() : mask_(0) {}
	explicit InstructionSetFeatures(unsigned int mask) : mask_(mask) {}

	static InstructionSetFeatures GuessInstructionSetFeatures();

	bool HasDivideInstruction() const {
		return (mask_ & kHwDiv) != 0;
	}

	void SetHasDivideInstruction(bool v) {
		mask_ = (mask_ & ~kHwDiv) | (v ? kHwDiv : 0);
	}

	bool HasLpae() const {
		return (mask_ & kHwLpae) != 0;
	}

	void SetHasLpae(bool v) {
		mask_ = (mask_ & ~kHwLpae) | (v ? kHwLpae : 0);
	}

	std::string GetFeatureString() const;

	// Other features in here.

	bool operator==(const InstructionSetFeatures &peer) const {
		return mask_ == peer.mask_;
	}

	bool operator!=(const InstructionSetFeatures &peer) const {
		return mask_ != peer.mask_;
	}

	bool operator<=(const InstructionSetFeatures &peer) const {
		return (mask_ & peer.mask_) == mask_;
	}

private:
	unsigned int mask_;
};