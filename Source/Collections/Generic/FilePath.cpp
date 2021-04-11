#include <Collections/FilePath.hpp>

CFilePath::CFilePath(const CFilePath &t_from) = default;

CFilePath::CFilePath(static_string t_from) : CFixedString<256>(t_from) {}
