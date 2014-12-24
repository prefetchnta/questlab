/*
***************************************
*   Asylum3D @ 2014-12-23
***************************************
*/

#ifndef __CRH3D9_GRAPHICS_HPP__
#define __CRH3D9_GRAPHICS_HPP__

/******************/
/* Effect Factory */
/******************/
CR_API asy::IEffect* create_crh3d9_eff_wf_fixed (D3DCOLOR ambient, D3DLIGHT9** light,
            BOOL* on_off, DWORD count, BOOL specular, const asy::crh3d9_main* main);

/*********************/
/* Attribute Factory */
/*********************/
CR_API asy::IAttrib* create_crh3d9_attr_wf (const sWAVEFRONT_M* mtrl,
    const asy::map_acs<asy::crh3d9_texr>* texpool, const asy::crh3d9_main* main);

#endif  /* __CRH3D9_GRAPHICS_HPP__ */
