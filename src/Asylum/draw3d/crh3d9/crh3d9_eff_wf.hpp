/*
***************************************
*   Asylum3D @ 2014-12-23
***************************************
*/

#ifndef __CRH3D9_EFF_WF_HPP__
#define __CRH3D9_EFF_WF_HPP__

/***************/
/* Factory API */
/***************/
CR_API asy::effect_i* create_crh3d9_eff_wf_fixed (cl32_t ambient, D3DLIGHT9** light,
                        BOOL* on_off, DWORD count, const asy::crh3d9_main* main);

#endif  /* __CRH3D9_EFF_WF_HPP__ */
