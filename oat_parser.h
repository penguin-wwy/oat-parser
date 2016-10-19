#include <string>
#include <memory>

class OatHeader;

class OatParser
{
public:
	OatParser();
	~OatParser();

private:
	std::unique_ptr<OatHeader> _oat_header;
	std::string _oat_file;

	const char *_begin;
	const char *_end;
};