#include "oat_header.h"

template<typename T>
static inline bool IsAligned(T x) {
	//static_assert((n & (n - 1)) == 0, "n is not a power of two");
	//return (x & (n - 1)) == 0;
	return x;
}

const char OatHeader::kOatMagic[] = { 0x6F, 0x61, 0x74, 0x0A };
const OatHeader::uint32_t OatHeader::defSize = 4;

OatHeader::OatHeader(void *data, int64_t len)
{
	if (data != nullptr && len > 0) {
		file = data;
		length = len;
	}
	else {
		return;
	}

	 adler32_checksum_ = 0;

	InstructionSet instruction_set_; /*size of 4*/
	switch (char2uint((char *)file, 3))
	{
	case kNone: instruction_set_ = kNone; break;
	case kArm: instruction_set_ = kArm; break;
	case kArm64: instruction_set_ = kArm64; break;
	case kThumb2: instruction_set_ = kThumb2; break;
	case kX86: instruction_set_ = kX86; break;
	case kX86_64: instruction_set_ = kX86_64; break;
	case kMips: instruction_set_ = kMips; break;
	case kMips64: instruction_set_ = kMips64; break;
	default:
		fprintf(stderr, "error InstructionSet\n");
		return;
	}
	instruction_set_features_ = new InstructionSetFeatures(char2uint((char *)file, 4));
	 dex_file_count_ = 0;
	 executable_offset_ = 0;
	 interpreter_to_interpreter_bridge_offset_ = 0;
	 interpreter_to_compiled_code_bridge_offset_ = 0;
	 jni_dlsym_lookup_offset_ = 0;
	 portable_imt_conflict_trampoline_offset_ = 0;
	 portable_resolution_trampoline_offset_ = 0;
	 portable_to_interpreter_bridge_offset_ = 0;
	 quick_generic_jni_trampoline_offset_ = 0;
	 quick_imt_conflict_trampoline_offset_ = 0;
	 quick_resolution_trampoline_offset_ = 0;
	 quick_to_interpreter_bridge_offset_ = 0;

	// The amount that the image this oat is associated with has been patched.
	 image_patch_delta_ = 0;

	 image_file_location_oat_checksum_ = 0;
	 image_file_location_oat_data_begin_ = 0;

	 key_value_store_size_ = 0;
	 key_value_store_ = nullptr;
	
}

OatHeader::~OatHeader()
{

}

bool OatHeader::initHeader()
{
	char *buf = (char *)file;
	
	for (int i = 0; i < 8; i++) {
		if (i < 4) {
			magic_[i] = *(buf++);
		}
		else {
			version_[i - 4] = *(buf++);
		}
	}

	if (!strcmp(magic_, kOatMagic)) {
		fprintf(stderr, "err magic\n");
		return false;
	}

	adler32_checksum_ = char2uint(buf, 0 * defSize);

	dex_file_count_ = char2uint(buf, 3 * defSize);
	executable_offset_ = char2uint(buf, 4 * defSize);

	image_file_location_oat_checksum_ = char2uint(buf, 16 * defSize);
	image_file_location_oat_data_begin_ = char2uint(buf, 17 * defSize);

	key_value_store_size_ = char2uint(buf, 18 * defSize);

	key_value_store_ = (char *)malloc((size_t)key_value_store_);
	key_value_store_ = buf + 19;


	return true;
}

bool OatHeader::IsValid() const
{
	if (!IsAligned<OatHeader::uint32_t>(executable_offset_)) {
		return false;
	}

	if (!IsAligned<OatHeader::uint32_t>(image_patch_delta_)) {
		return false;
	}

	return true;
}


const char* OatHeader::GetMagic() const {
	return reinterpret_cast<const char*>(magic_);
}

OatHeader::uint32_t OatHeader::GetChecksum() const{
	return adler32_checksum_;
}