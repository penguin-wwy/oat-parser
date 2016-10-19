#include "instruction_set.h"

class OatHeader
{
	typedef unsigned char uint8_t;
	typedef unsigned int uint32_t;
	typedef long long int64_t;

public:
	OatHeader(void *, int64_t);
	~OatHeader();

	bool initHeader();

	bool IsValid() const;
	const char* GetMagic() const;
	uint32_t GetChecksum() const;

	uint32_t GetDexFileCount() const {
		return dex_file_count_;
	}

	uint32_t GetExecutableOffset() const {
		return executable_offset_;
	}

	uint32_t GetKeyValueStoreSize() const {
		return key_value_store_size_;
	}

	static inline uint32_t char2uint(char *data, int64_t pos) {
		uint32_t tmp = 0;
		for (int i = 0; i < 4; i++) {
			uint8_t byte = (uint8_t)(*(data + pos + 3 - i));
			tmp ^= byte;
			tmp = tmp << 8;
		}
		return tmp;
	}

	static const char kOatMagic[4];
	static const uint32_t defSize;

private:
	void *file;
	int64_t length;

	char magic_[4];
	char version_[4];
	uint32_t adler32_checksum_;

	InstructionSet instruction_set_; /*size of 4*/
	InstructionSetFeatures *instruction_set_features_; /*size of 4*/
	uint32_t dex_file_count_;
	uint32_t executable_offset_;
	uint32_t interpreter_to_interpreter_bridge_offset_;
	uint32_t interpreter_to_compiled_code_bridge_offset_;
	uint32_t jni_dlsym_lookup_offset_;
	uint32_t portable_imt_conflict_trampoline_offset_;
	uint32_t portable_resolution_trampoline_offset_;
	uint32_t portable_to_interpreter_bridge_offset_;
	uint32_t quick_generic_jni_trampoline_offset_;
	uint32_t quick_imt_conflict_trampoline_offset_;
	uint32_t quick_resolution_trampoline_offset_;
	uint32_t quick_to_interpreter_bridge_offset_;

	// The amount that the image this oat is associated with has been patched.
	int image_patch_delta_;

	uint32_t image_file_location_oat_checksum_;
	uint32_t image_file_location_oat_data_begin_;

	uint32_t key_value_store_size_;
	char *key_value_store_;  // note variable width data at end
};