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
    /* ======================================================================================================================================================== */
    bool init (const crh3d9_main* main, const D3DVERTEXELEMENT9* decl, const char* name, int32u flags = 0, const char* entry = "main", const char* profile = NULL)
    {
        m_main = main->get_main();
        m_call = main->get_call();
        m_exec = m_call->create_vs_fileA(m_main, decl, name, flags, entry, profile);
        return ((m_exec == NULL) ? false : true);
    }

    /* ============================================================================================================================================================ */
    bool init_txt (const crh3d9_main* main, const D3DVERTEXELEMENT9* decl, const char* text, int32u flags = 0, const char* entry = "main", const char* profile = NULL)
    {
        m_main = main->get_main();
        m_call = main->get_call();
        m_exec = m_call->create_vs_text(m_main, decl, name, flags, entry, profile);
        return ((m_exec == NULL) ? false : true);
    }

    /* ================================================================================== */
    bool init_bin (const crh3d9_main* main, const D3DVERTEXELEMENT9* decl, const void* data)
    {
        m_main = main->get_main();
        m_call = main->get_call();
        m_exec = m_call->create_vs_data(m_main, decl, data);
        return ((m_exec == NULL) ? false : true);
    }

    /* ====== */
    void free ()
    {
        m_call->release_vs(m_main, m_exec);
    }

public:
    /* ============================================ */
    void same_vertex (const crh3d9_vshader* vsh) const
    {
        if (m_exec->copy)
            m_exec->decl = vsh->get_exec()->decl;
    }

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
    /* ========================================================================================================================= */
    bool init (const crh3d9_main* main, const char* name, int32u flags = 0, const char* entry = "main", const char* profile = NULL)
    {
        m_main = main->get_main();
        m_call = main->get_call();
        m_exec = m_call->create_ps_fileA(m_main, name, flags, entry, profile);
        return ((m_exec == NULL) ? false : true);
    }

    /* ============================================================================================================================= */
    bool init_txt (const crh3d9_main* main, const char* text, int32u flags = 0, const char* entry = "main", const char* profile = NULL)
    {
        m_main = main->get_main();
        m_call = main->get_call();
        m_exec = m_call->create_ps_text(m_main, decl, name, flags, entry, profile);
        return ((m_exec == NULL) ? false : true);
    }

    /* =================================================== */
    bool init_bin (const crh3d9_main* main, const void* data)
    {
        m_main = main->get_main();
        m_call = main->get_call();
        m_exec = m_call->create_ps_data(m_main, data);
        return ((m_exec == NULL) ? false : true);
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
