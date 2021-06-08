
/*
---------------------------------------
    HelloWorld
---------------------------------------
*/
static bool_t
quest64_helloworld (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE  dest;
    ansi_t* mess;

    /* 图片反色 */
    quest64_set_image(&dest, image);
    fill.dx = fill.dy = 0;
    fill.dw = dest.position.ww;
    fill.dh = dest.position.hh;
    clrs.val = 0x00FFFFFF;
    fill_xor32_c(&dest, &fill, clrs, NULL);
    quest64_return_string(netw, "quest64_helloworld()");

    /* 信息提示 */
    mess = xml_attr_stringU("message", param);
    if (mess != NULL) {
        MessageBoxA(NULL, mess, "HelloWorld", MB_OK | MB_ICONINFORMATION);
        mem_free(mess);
    }
    return (TRUE);
}
