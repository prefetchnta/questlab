/*
***************************************
*   Asylum3D @ 2014-12-10
***************************************
*/

#ifndef __CRH3D9_TEXR_HPP__
#define __CRH3D9_TEXR_HPP__

/* Asylum Namespace */
namespace asy {

/******************/
/* Texture Object */
/******************/
class crh3d9_texr : public asylum
{
public:
    sD3D9_MAIN*         m_main;
    sD3D9_TEXR*         m_texr;
    const sD3D9_CALL*   m_call;

public:
    /* ========================================================== */
    bool init (const char* name, const char* type, sD3D9_MAIN *main)
    {
        cl32_t  keycolor;

        m_call = d3d9call_get();
        if (str_strA(type, "tex2:") == type) {
            keycolor = str2intx32A(type + 5);
            m_texr = m_call->create_tex2_fileA(main, name, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        }
        else
        if (str_strA(type, "tex3:") == type) {
            keycolor = str2intx32A(type + 5);
            m_texr = m_call->create_tex3_fileA(main, name, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        }
        else
        if (str_strA(type, "texv:") == type) {
            keycolor = str2intx32A(type + 5);
            m_texr = m_call->create_texv_fileA(main, name, D3DPOOL_MANAGED, 0, 0, D3DX_DEFAULT, keycolor);
        }
        else {
            return (false);
        }
        if (m_texr == NULL)
            return (false);
        m_main = main;
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_call->release_texr(m_texr);
    }
};

}   /* namespace */

#endif  /* __CRH3D9_TEXR_HPP__ */
