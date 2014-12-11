/*
***************************************
*   Asylum3D @ 2014-12-10
***************************************
*/

#ifndef __CRH3D9_TEXTURE_HPP__
#define __CRH3D9_TEXTURE_HPP__

/* Asylum Namespace */
namespace asy {

/******************/
/* Texture Object */
/******************/
class crh3d9_texr : public asylum
{
private:
    int32u              m_flag;
    D3DPOOL             m_pool;
    sD3D9_MAIN*         m_main;
    sD3D9_TEXR*         m_texr;
    const sD3D9_CALL*   m_call;

public:
    /* ============================================================================== */
    bool init (sD3D9_MAIN* main, const char* name, uint_t face = 1, cl32_t keycolor = 0)
    {
        m_call = d3d9call_get();
        if (face == 1)
            m_texr = m_call->create_tex2_fileA(main, name, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        else
        if (face == 6)
            m_texr = m_call->create_tex3_fileA(main, name, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        else
        if (face == 3)
            m_texr = m_call->create_texv_fileA(main, name, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        else
            return (false);
        if (m_texr == NULL)
            return (false);
        m_flag = 0;
        m_pool = D3DPOOL_MANAGED;
        m_main = main;
        return (true);
    }

    /* =========================================================================================== */
    bool init (sD3D9_MAIN* main, const void* data, size_t size, uint_t face = 1, cl32_t keycolor = 0)
    {
        m_call = d3d9call_get();
        if (face == 1)
            m_texr = m_call->create_tex2_mem(main, data, size, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        else
        if (face == 6)
            m_texr = m_call->create_tex3_mem(main, data, size, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        else
        if (face == 3)
            m_texr = m_call->create_texv_mem(main, data, size, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        else
            return (false);
        if (m_texr == NULL)
            return (false);
        m_flag = 0;
        m_pool = D3DPOOL_MANAGED;
        m_main = main;
        return (true);
    }

    /* ===================================================================================================================================== */
    bool init (sD3D9_MAIN* main, D3DFORMAT format, D3DPOOL pool, int32u usage, uint_t level, uint_t width, uint_t height = 0, uint_t depth = 0)
    {
        m_call = d3d9call_get();
        if (depth == 0) {
            if (height == 0)
                m_texr = m_call->create_tex3(main, width, format, pool, usage, level);
            else
                m_texr = m_call->create_tex2(main, width, height, format, pool, usage, level);
        }
        else {
            m_texr = m_call->create_texv(main, width, height, depth, format, pool, usage, level);
        }
        if (m_texr == NULL)
            return (false);
        m_flag = usage;
        m_pool = pool;
        m_main = main;
        return (true);
    }

    /* =========================================== */
    bool init (sD3D9_MAIN* main, const sIMAGE *image)
    {
        m_call = d3d9call_get();
        m_texr = m_call->create_tex2_crh(main, image, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT);
        if (m_texr == NULL)
            return (false);
        m_flag = 0;
        m_pool = D3DPOOL_MANAGED;
        m_main = main;
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_call->release_texr(m_texr);
    }

public:
    /* ======= */
    bool reset ()
    {
        sD3D9_TEXR* texr;

        if (m_pool == D3DPOOL_MANAGED)
            return (true);
        if (m_texr->face == 1)
            texr = m_call->create_tex2(m_main, m_texr->info.Width, m_texr->info.Height, m_texr->info.Format, m_pool, m_flag, m_texr->info.MipLevels);
        else
        if (m_texr->face == 6)
            texr = m_call->create_tex3(m_main, m_texr->info.Width, m_texr->info.Format, m_pool, m_flag, m_texr->info.MipLevels);
        else
        if (m_texr->face == 3)
            texr = m_call->create_texv(m_main, m_texr->info.Width, m_texr->info.Height, m_texr->info.Depth, m_texr->info.Format, m_pool, m_flag, m_texr->info.MipLevels);
        else
            return (false);
        m_call->release_texr(m_texr);
        m_texr = texr;
        return (true);
    }

    /* =========================== */
    void apply (int32u sampler) const
    {
        m_main->dev->SetTexture(sampler, m_texr->obj.base);
    }

    /* ======================================================== */
    void mipmap (D3DTEXTUREFILTERTYPE type = D3DTEXF_LINEAR) const
    {
        m_texr->obj.base->SetAutoGenFilterType(type);
        m_texr->obj.base->GenerateMipSubLevels();
    }

    /* ======================= */
    sD3D9_TEXR* get_texr () const
    {
        return (m_texr);
    }
};

}   /* namespace */

#endif  /* __CRH3D9_TEXTURE_HPP__ */
