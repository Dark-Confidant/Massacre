#include "Universe.h"
#include "gfx/Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>

#include "SmallObject.h"

using namespace mcr;
using namespace gfx;

struct Font::Impl: public SmallObject
{
    FT_Face font;
};

rcptr<Font> Font::createFromFile(IFile* file)
{
    return new Font;
}

Font::Font(): m_impl(new Impl)
{
    m_impl->font = nullptr;
}

Font::~Font()
{
    if (m_impl->font)
        FT_Done_Face(m_impl->font);

    delete m_impl;
}

void Font::load(IFile* file)
{
    static FT_Library ft = nullptr;

    if (!ft)
        FT_Init_FreeType(&ft);

    FT_New_Face(ft, file->filename(), 0, &m_impl->font);
    FT_Set_Char_Size(m_impl->font, 0, 20 * 64, 0, 0);

    for (wchar_t c = 0; c < 32768; ++c)
    {
        FT_Load_Char(m_impl->font, c, FT_LOAD_RENDER);

        //m_impl->font->glyph->bitmap.
    }
}
