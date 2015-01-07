/*
***************************************
*   Asylum3D @ 2014-12-23
***************************************
*/

#ifndef __CRH3D9_GRAPH_HPP__
#define __CRH3D9_GRAPH_HPP__

/****************/
/* Factory Type */
/****************/
typedef asy::IAttrib* (*create_crh3d9_attr_wf_t) (const sWAVEFRONT_M* mtl,
    const asy::map_acs<asy::crh3d9_texr>* texpool, const asy::crh3d9_main* main);

typedef asy::IMesh* (*create_crh3d9_mesh_wf_t) (const sWAVEFRONT* obj, leng_t idx,
                                                const asy::crh3d9_main* main);
/******************/
/* Effect Factory */
/******************/
CR_API asy::IEffect* create_crh3d9_ffct_wf_fixed (uint_t fvf, bool_t tex,
                                                  const asy::crh3d9_main* main);

CR_API asy::IEffect* create_crh3d9_ffct_light_fixed (D3DCOLOR* ambient, D3DLIGHT9* light,
                                BOOL* onoff, DWORD count, const asy::crh3d9_main* main);

CR_API asy::IEffect* create_crh3d9_ffct_alpha_fixed (DWORD value, const asy::crh3d9_main* main);

#define EFFECT_STT_RS   0
#define EFFECT_STT_SS   1
#define EFFECT_STT_TSS  2
CR_API asy::IEffect* create_crh3d9_ffct_state_fixed (const int32u* list, uint_t ninp, uint_t nout,
                                                     uint_t type, const asy::crh3d9_main* main);

/*********************/
/* Attribute Factory */
/*********************/
CR_API asy::IAttrib* create_crh3d9_attr_wf_fixed (const sWAVEFRONT_M* mtl,
    const asy::map_acs<asy::crh3d9_texr>* texpool, const asy::crh3d9_main* main);

/****************/
/* Mesh Factory */
/****************/
CR_API asy::IMesh* create_crh3d9_mesh_wf_ss (const sWAVEFRONT* obj, leng_t idx,
                                             const asy::crh3d9_main* main);
/***********************/
/* Object Base Factory */
/***********************/
CR_API bool create_crh3d9_base_wf (asy::object_base* base, const sWAVEFRONT* obj,
                            create_crh3d9_attr_wf_t fattr, create_crh3d9_mesh_wf_t fmesh,
                                    const asy::map_acs<asy::crh3d9_texr>* texpool,
                                            const asy::crh3d9_main* main);
/************************/
/* CrHack3D9 Render API */
/************************/
typedef void*   crh3d9_t;
CR_API crh3d9_t crhack3d9_init (HWND hwnd);
CR_API void crhack3d9_kill (crh3d9_t render);
CR_API bool crhack3d9_reset (crh3d9_t render);
CR_API void crhack3d9_commit (crh3d9_t render);
CR_API void crhack3d9_clean (crh3d9_t render);
CR_API bool crhack3d9_effect (crh3d9_t render, const char* name,
                              asy::IEffect* effect);
CR_API bool crhack3d9_instance (crh3d9_t render, const char* name,
                                const char* base, const vec3d_t* rote,
                                const vec3d_t* move, const vec3d_t* scale);
CR_API bool crhack3d9_wavefront (crh3d9_t render, const char* name,
                    const ansi_t* obj, const ansi_t* mtl, bool_t swap_yz,
                                bool_t neg_z, const char* type);

#endif  /* __CRH3D9_GRAPH_HPP__ */
