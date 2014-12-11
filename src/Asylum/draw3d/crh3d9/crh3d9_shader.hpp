/*
***************************************
*   Asylum3D @ 2014-12-11
***************************************
*/

#ifndef __CRH3D9_SHADER_HPP__
#define __CRH3D9_SHADER_HPP__

/* Asylum Namespace */
namespace asy {

/*****************/
/* Vertex Shader */
/*****************/
class crh3d9_vshader : public asylum
{
private:
    sD3D9_VSH*          m_exec;
    sD3D9_MAIN*         m_main;
    const sD3D9_CALL*   m_call;

public:
    /* ============================================================================================================================================ */
    bool init (sD3D9_MAIN *main, LPD3DVERTEXELEMENT9 decl, const char* name, int32u flags = 0, const char* entry = "main", const char* profile = NULL)
    {
        m_call = d3d9call_get();
        m_exec = m_call->create_vs_fileA(main, decl, name, flags, entry, profile);
        if (m_exec == NULL)
            return (false);
        m_main = main;
        return (true);
    }

    /* ================================================================================================================================================ */
    bool init_txt (sD3D9_MAIN *main, LPD3DVERTEXELEMENT9 decl, const char* text, int32u flags = 0, const char* entry = "main", const char* profile = NULL)
    {
        m_call = d3d9call_get();
        m_exec = m_call->create_vs_text(main, decl, name, flags, entry, profile);
        if (m_exec == NULL)
            return (false);
        m_main = main;
        return (true);
    }

    /* ====================================================================== */
    bool init_bin (sD3D9_MAIN* main, LPD3DVERTEXELEMENT9 decl, const void* data)
    {
        m_call = d3d9call_get();
        m_exec = m_call->create_vs_data(main, decl, data);
        if (m_exec == NULL)
            return (false);
        m_main = main;
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_call->release_vs(m_main, m_exec);
    }

public:
    /* ============= */
    void apply () const
    {
        m_main->dev->SetVertexDeclaration(m_exec->decl);
        m_main->dev->SetVertexShader(m_exec->obj);
    }

    /* ====================== */
    sD3D9_VSH* get_exec () const
    {
        return (m_exec);
    }
};

/****************/
/* Pixel Shader */
/****************/
class crh3d9_pshader : public asylum
{
private:
    sD3D9_PSH*          m_exec;
    sD3D9_MAIN*         m_main;
    const sD3D9_CALL*   m_call;

public:
    /* ================================================================================================================== */
    bool init (sD3D9_MAIN *main, const char* name, int32u flags = 0, const char* entry = "main", const char* profile = NULL)
    {
        m_call = d3d9call_get();
        m_exec = m_call->create_ps_fileA(main, name, flags, entry, profile);
        if (m_exec == NULL)
            return (false);
        m_main = main;
        return (true);
    }

    /* ====================================================================================================================== */
    bool init_txt (sD3D9_MAIN *main, const char* text, int32u flags = 0, const char* entry = "main", const char* profile = NULL)
    {
        m_call = d3d9call_get();
        m_exec = m_call->create_ps_text(main, decl, name, flags, entry, profile);
        if (m_exec == NULL)
            return (false);
        m_main = main;
        return (true);
    }

    /* ============================================ */
    bool init_bin (sD3D9_MAIN* main, const void* data)
    {
        m_call = d3d9call_get();
        m_exec = m_call->create_ps_data(main, data);
        if (m_exec == NULL)
            return (false);
        m_main = main;
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_call->release_ps(m_main, m_exec);
    }

public:
    /* ============= */
    void apply () const
    {
        m_main->dev->SetPixelShader(m_exec->obj);
    }

    /* ====================== */
    sD3D9_PSH* get_exec () const
    {
        return (m_exec);
    }
};

}   /* namespace */

#endif  /* __CRH3D9_SHADER_HPP__ */
