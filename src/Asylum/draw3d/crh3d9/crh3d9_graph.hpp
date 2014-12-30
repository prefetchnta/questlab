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
CR_API asy::IEffect* create_crh3d9_ffct_wf_fixed (uint_t fvf, int64u flags,
                                                  const asy::crh3d9_main* main);

CR_API asy::IEffect* create_crh3d9_ffct_light_fixed (D3DCOLOR* ambient, D3DLIGHT9* light,
                                BOOL* onoff, DWORD count, const asy::crh3d9_main* main);

CR_API asy::IEffect* create_crh3d9_ffct_alpha_fixed (DWORD value, const asy::crh3d9_main* main);

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

#endif  /* __CRH3D9_GRAPH_HPP__ */
