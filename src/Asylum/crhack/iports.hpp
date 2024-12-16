/*
***************************************
*   CrHack C++ Wrapper @ 2013-12-20
***************************************
*/

#ifndef __IPORTS_HPP__
#define __IPORTS_HPP__

/* Asylum Namespace */
namespace asy {

/******************/
/* iDATIN Wrapper */
/******************/
class IDatIN : public asylum
{
private:
    iDATIN* m_that;

public:
    /* =============== */
    IDatIN (iDATIN* that)
    {
        m_that = that;
    }
    ~IDatIN ()
    {
    }

public:
    /* ====== */
    void free ()
    {
        this->release();
    }

public:
    /* ======================= */
    void_t release (void_t) const
    {
        CR_VCALL(m_that)->release(m_that);
    }

    /* ======================================== */
    template<class T>T* getMore (port_t iid) const
    {
        return ((T*)(CR_VCALL(m_that)->getMore(m_that, iid)));
    }

    /* ===================== */
    fsize_t tell (void_t) const
    {
        return (CR_VCALL(m_that)->tell(m_that));
    }

    /* =========================================== */
    bool_t seek (fdist_t offset, uint_t whence) const
    {
        return (CR_VCALL(m_that)->seek(m_that, offset, whence));
    }

    /* ============================================ */
    bool_t seek64 (int64s offset, uint_t whence) const
    {
        return (CR_VCALL(m_that)->seek64(m_that, offset, whence));
    }

    /* ===================== */
    bool_t isEOF (void_t) const
    {
        return (CR_VCALL(m_that)->isEOF(m_that));
    }

    /* ====================== */
    bool_t rewind (void_t) const
    {
        return (CR_VCALL(m_that)->rewind(m_that));
    }

    /* ========================= */
    bool_t getb (byte_t* val) const
    {
        return (CR_VCALL(m_that)->getb_no(m_that, val));
    }

    /* ========================= */
    bool_t getw (int16u* val) const
    {
        return (CR_VCALL(m_that)->getw_no(m_that, val));
    }

    /* ========================= */
    bool_t getd (int32u* val) const
    {
        return (CR_VCALL(m_that)->getd_no(m_that, val));
    }

    /* ========================= */
    bool_t getq (int64u* val) const
    {
        return (CR_VCALL(m_that)->getq_no(m_that, val));
    }

    /* ============================ */
    bool_t getw_le (int16u* val) const
    {
        return (CR_VCALL(m_that)->getw_le(m_that, val));
    }

    /* ============================ */
    bool_t getd_le (int32u* val) const
    {
        return (CR_VCALL(m_that)->getd_le(m_that, val));
    }

    /* ============================ */
    bool_t getq_le (int64u* val) const
    {
        return (CR_VCALL(m_that)->getq_le(m_that, val));
    }

    /* ============================ */
    bool_t getw_be (int16u* val) const
    {
        return (CR_VCALL(m_that)->getw_be(m_that, val));
    }

    /* ============================ */
    bool_t getd_be (int32u* val) const
    {
        return (CR_VCALL(m_that)->getd_be(m_that, val));
    }

    /* ============================ */
    bool_t getq_be (int64u* val) const
    {
        return (CR_VCALL(m_that)->getq_be(m_that, val));
    }

    /* ========================== */
    byte_t* get (leng_t* size) const
    {
        return (CR_VCALL(m_that)->get(m_that, size, FALSE));
    }

    /* ============================================= */
    template<class T>bool_t getype (void_t* data) const
    {
        return (this->read(data, sizeof(T)) == sizeof(T));
    }

    /* ======================================= */
    leng_t read (void_t* data, leng_t size) const
    {
        return (CR_VCALL(m_that)->read(m_that, data, size));
    }

    /* =========================================================== */
    bool_t check (void_t* temp, const ansi_t* tag, uint_t size) const
    {
        return (CR_VCALL(m_that)->check(m_that, temp, tag, size));
    }

    /* =========================================== */
    bool_t setArea (fdist_t head, fdist_t tail) const
    {
        return (CR_VCALL(m_that)->setArea(m_that, head, tail));
    }

    /* ======================== */
    fsize_t getHead (void_t) const
    {
        return (m_that->__head__);
    }

    /* ======================== */
    fsize_t getTail (void_t) const
    {
        return (m_that->__tail__);
    }

    /* ======================== */
    fsize_t getSize (void_t) const
    {
        return (m_that->__size__);
    }
};

/******************/
/* iDATOT Wrapper */
/******************/
class IDatOT : public asylum
{
private:
    iDATOT* m_that;

public:
    /* =============== */
    IDatOT (iDATOT* that)
    {
        m_that = that;
    }
    ~IDatOT ()
    {
    }

public:
    /* ====== */
    void free ()
    {
        this->release();
    }

public:
    /* ======================= */
    void_t release (void_t) const
    {
        CR_VCALL(m_that)->release(m_that);
    }

    /* ======================================== */
    template<class T>T* getMore (port_t iid) const
    {
        return ((T*)(CR_VCALL(m_that)->getMore(m_that, iid)));
    }

    /* ======================== */
    bool_t putb (byte_t val) const
    {
        return (CR_VCALL(m_that)->putb_no(m_that, val));
    }

    /* ======================== */
    bool_t putw (int16u val) const
    {
        return (CR_VCALL(m_that)->putw_no(m_that, val));
    }

    /* ======================== */
    bool_t putd (int32u val) const
    {
        return (CR_VCALL(m_that)->putd_no(m_that, val));
    }

    /* ======================== */
    bool_t putq (int64u val) const
    {
        return (CR_VCALL(m_that)->putq_no(m_that, val));
    }

    /* =========================== */
    bool_t putw_le (int16u val) const
    {
        return (CR_VCALL(m_that)->putw_le(m_that, val));
    }

    /* =========================== */
    bool_t putd_le (int32u val) const
    {
        return (CR_VCALL(m_that)->putd_le(m_that, val));
    }

    /* =========================== */
    bool_t putq_le (int64u val) const
    {
        return (CR_VCALL(m_that)->putq_le(m_that, val));
    }

    /* =========================== */
    bool_t putw_be (int16u val) const
    {
        return (CR_VCALL(m_that)->putw_be(m_that, val));
    }

    /* =========================== */
    bool_t putd_be (int32u val) const
    {
        return (CR_VCALL(m_that)->putd_be(m_that, val));
    }

    /* =========================== */
    bool_t putq_be (int64u val) const
    {
        return (CR_VCALL(m_that)->putq_be(m_that, val));
    }

    /* ========================= */
    bool_t zero (leng_t size) const
    {
        return (CR_VCALL(m_that)->zero(m_that, size));
    }

    /* ============================================= */
    template<class T>bool_t putype (void_t* data) const
    {
        return (this->write(data, sizeof(T)) == sizeof(T));
    }

    /* ============================================== */
    leng_t write (const void_t* data, leng_t size) const
    {
        return (CR_VCALL(m_that)->write(m_that, data, size));
    }

    /* ============================================ */
    bool_t putsA (const ansi_t* str, leng_t len) const
    {
        return (CR_VCALL(m_that)->putsA(m_that, str, len));
    }

    /* ============================================ */
    bool_t putsW (const wide_t* str, leng_t len) const
    {
        return (CR_VCALL(m_that)->putsW(m_that, str, len));
    }

    /* ====================== */
    void_t* flush (void_t) const
    {
        return (CR_VCALL(m_that)->flush(m_that));
    }

    /* ============================= */
    bool_t reput (fsize_t offset) const
    {
        return (CR_VCALL(m_that)->reput(m_that, offset));
    }

    /* ======================== */
    fsize_t getSize (void_t) const
    {
        return (m_that->__size__);
    }
};

/********************/
/* iPACKAGE Wrapper */
/********************/
class IPackage : public asylum
{
private:
    iPACKAGE*   m_that;

public:
    /* =================== */
    IPackage (iPACKAGE* that)
    {
        m_that = that;
    }
    ~IPackage ()
    {
    }

public:
    /* ====== */
    void free ()
    {
        this->release();
    }

public:
    /* ======================= */
    void_t release (void_t) const
    {
        CR_VCALL(m_that)->release(m_that);
    }

    /* ======================================== */
    template<class T>T* getMore (port_t iid) const
    {
        return ((T*)(CR_VCALL(m_that)->getMore(m_that, iid)));
    }

    /* ===================== */
    int64u count (void_t) const
    {
        return (pack_file_num(m_that));
    }

    /* ================================================ */
    bool_t findU (int64u* index, const ansi_t* name) const
    {
        return (pack_find_fileU(m_that, index, name));
    }

    /* ================================================ */
    bool_t findW (int64u* index, const wide_t* name) const
    {
        return (pack_find_fileW(m_that, index, name));
    }

    /* ========================================================================= */
    bool_t findA (int64u* index, const ansi_t* name, uint_t cpage = CR_LOCAL) const
    {
        return (pack_find_fileA(m_that, index, name, cpage));
    }

    /* ============================================================= */
    bool_t data (sBUFFER* buff, int64u index, bool_t hash = TRUE) const
    {
        return (pack_file_data(m_that, buff, index, hash));
    }

    /* ============================================ */
    bool_t info (sPAK_FILE** info, int64u index) const
    {
        return (pack_file_info(m_that, info, index));
    }

    /* ==================================================================== */
    bool_t dataU (sBUFFER* buff, const ansi_t* name, bool_t hash = TRUE) const
    {
        return (pack_file_dataU(m_that, buff, name, hash));
    }

    /* ==================================================================== */
    bool_t dataW (sBUFFER* buff, const wide_t* name, bool_t hash = TRUE) const
    {
        return (pack_file_dataW(m_that, buff, name, hash));
    }

    /* ============================================================================================= */
    bool_t dataA (sBUFFER* buff, const ansi_t* name, uint_t cpage = CR_LOCAL, bool_t hash = TRUE) const
    {
        return (pack_file_dataA(m_that, buff, name, cpage, hash));
    }

    /* =================================================== */
    bool_t infoU (sPAK_FILE** info, const ansi_t* name) const
    {
        return (pack_file_infoU(m_that, info, name));
    }

    /* =================================================== */
    bool_t infoW (sPAK_FILE** info, const wide_t* name) const
    {
        return (pack_file_infoW(m_that, info, name));
    }

    /* ============================================================================ */
    bool_t infoA (sPAK_FILE** info, const ansi_t* name, uint_t cpage = CR_LOCAL) const
    {
        return (pack_file_infoA(m_that, info, name, cpage));
    }
};

/********************/
/* iXMMEDIA Wrapper */
/********************/
class IXMMedia : public asylum
{
private:
    iXMMEDIA*   m_that;

public:
    /* =================== */
    IXMMedia (iXMMEDIA* that)
    {
        m_that = that;
    }
    ~IXMMedia ()
    {
    }

public:
    /* ====== */
    void free ()
    {
        this->release();
    }

public:
    /* ======================= */
    void_t release (void_t) const
    {
        CR_VCALL(m_that)->release(m_that);
    }

    /* ======================================== */
    template<class T>T* getMore (port_t iid) const
    {
        return ((T*)(CR_VCALL(m_that)->getMore(m_that, iid)));
    }

    /* ==================== */
    bool_t play (void_t) const
    {
        return (CR_VCALL(m_that)->play(m_that));
    }

    /* ===================== */
    bool_t pause (void_t) const
    {
        return (CR_VCALL(m_that)->pause(m_that));
    }

    /* ==================== */
    bool_t stop (void_t) const
    {
        return (CR_VCALL(m_that)->stop(m_that));
    }

    /* ====================== */
    bool_t replay (void_t) const
    {
        if (this->stop())
            return (this->play());
        return (FALSE);
    }

    /* ============================ */
    bool_t set_pos (int64u curt) const
    {
        return (CR_VCALL(m_that)->set_pos(m_that, curt));
    }

    /* ============================================ */
    bool_t get_pos (int64u* curt, int64u* total) const
    {
        return (CR_VCALL(m_that)->get_pos(m_that, curt, total));
    }

    /* ============================= */
    bool_t is_over (bool_t* over) const
    {
        return (CR_VCALL(m_that)->is_over(m_that, over));
    }

    /* ================================== */
    bool_t set_volume (sint_t percent) const
    {
        return (CR_VCALL(m_that)->set_volume(m_that, percent));
    }

    /* =================================== */
    bool_t get_volume (sint_t* percent) const
    {
        return (CR_VCALL(m_that)->get_volume(m_that, percent));
    }

    /* ================================ */
    bool_t adj_volume (sint_t value) const
    {
        return (this->set_volume(value + m_that->__volume__));
    }

    /* ======================== */
    bool_t is_video (void_t) const
    {
        return (CR_VCALL(m_that)->is_video(m_that));
    }

    /* ================================================= */
    bool_t video_size (uint_t* width, uint_t* height) const
    {
        return (CR_VCALL(m_that)->video_size(m_that, width, height));
    }

    /* ============================================== */
    bool_t video_setwin (hwnd_t hwnd, bool_t full) const
    {
        return (CR_VCALL(m_that)->video_setwin(m_that, hwnd, full));
    }

    /* ====================== */
    hwnd_t window (void_t) const
    {
        return (m_that->__hwnd__);
    }

    /* ======================= */
    bool_t is_full (void_t) const
    {
        return (m_that->__full__);
    }

    /* ====================== */
    sint_t volume (void_t) const
    {
        return (m_that->__volume__);
    }
};

/********************/
/* iPICTURE Wrapper */
/********************/
class IPicture : public asylum
{
private:
    iPICTURE*   m_that;

public:
    /* =================== */
    IPicture (iPICTURE* that)
    {
        m_that = that;
    }
    ~IPicture ()
    {
    }

public:
    /* ====== */
    void free ()
    {
        this->release();
    }

public:
    /* ======================= */
    void_t release (void_t) const
    {
        CR_VCALL(m_that)->release(m_that);
    }

    /* ======================================== */
    template<class T>T* getMore (port_t iid) const
    {
        return ((T*)(CR_VCALL(m_that)->getMore(m_that, iid)));
    }

    /* ============================ */
    sFMT_PIC* get (int32u index) const
    {
        return (CR_VCALL(m_that)->get(m_that, index));
    }

    /* ===================== */
    int32u count (void_t) const
    {
        return (m_that->__count__);
    }
};

/*****************/
/* iGFX2 Wrapper */
/*****************/
class IGfx2 : public asylum
{
private:
    iGFX2*  m_that;

public:
    /* ============= */
    IGfx2 (iGFX2* that)
    {
        m_that = that;
    }
    ~IGfx2 ()
    {
    }

public:
    /* ====== */
    void free ()
    {
        this->release();
    }

public:
    /* ======================= */
    void_t release (void_t) const
    {
        CR_VCALL(m_that)->release(m_that);
    }

    /* ======================================== */
    template<class T>T* getMore (port_t iid) const
    {
        return ((T*)(CR_VCALL(m_that)->getMore(m_that, iid)));
    }

    /* ===================== */
    bool_t reset (void_t) const
    {
        return (CR_VCALL(m_that)->reset(m_that));
    }

    /* ===================== */
    sIMAGE* lock (void_t) const
    {
        return (CR_VCALL(m_that)->lock(m_that));
    }

    /* ====================== */
    void_t unlock (void_t) const
    {
        CR_VCALL(m_that)->unlock(m_that);
    }

    /* ========================= */
    bool_t flip (bool_t sync) const
    {
        return (CR_VCALL(m_that)->flip(m_that, sync));
    }

    /* ========================================= */
    bool_t clear (cl32_t color, int32u param) const
    {
        return (CR_VCALL(m_that)->clear(m_that, color, param));
    }

    /* ========================================== */
    bool_t setPal (uint_t start, uint_t count) const
    {
        return (CR_VCALL(m_that)->setPal(m_that, start, count));
    }

    /* ===================================== */
    template<class T>T* get_port (void_t) const
    {
        return ((T*)m_that);
    }
};

/*****************/
/* iFONT Wrapper */
/*****************/
class IFont : public asylum
{
private:
    iFONT*  m_that;

public:
    /* ============= */
    IFont (iFONT* that)
    {
        m_that = that;
    }
    ~IFont ()
    {
    }

public:
    /* ====== */
    void free ()
    {
        this->release();
    }

public:
    /* ======================= */
    void_t release (void_t) const
    {
        CR_VCALL(m_that)->release(m_that);
    }

    /* ======================================== */
    template<class T>T* getMore (port_t iid) const
    {
        return ((T*)(CR_VCALL(m_that)->getMore(m_that, iid)));
    }

    /* ===================== */
    void_t enter (void_t) const
    {
        CR_VCALL(m_that)->enter(m_that);
    }

    /* ===================== */
    void_t leave (void_t) const
    {
        CR_VCALL(m_that)->leave(m_that);
    }

    /* ========================= */
    bool_t bind (iGFX2* gfx2) const
    {
        return (CR_VCALL(m_that)->bind(m_that, gfx2));
    }

    /* ============================ */
    bool_t setMode (int32u mode) const
    {
        return (CR_VCALL(m_that)->setMode(m_that, mode));
    }

    /* ============================== */
    bool_t setColor (cl32_t color) const
    {
        return (CR_VCALL(m_that)->setColor(m_that, color));
    }

    /* ================================ */
    bool_t setBkColor (cl32_t color) const
    {
        return (CR_VCALL(m_that)->setBkColor(m_that, color));
    }

    /* ============================================================================================================ */
    bool_t draw_tran (const void_t* text, const sRECT* rect, uint_t type = EGUI_ALN_CM, uint_t cpage = CR_LOCAL) const
    {
        return (egui_draw_tran(m_that, text, rect, type, cpage));
    }

    /* =================================================================================== */
    bool_t draw_tran2 (const void_t* text, sint_t x, sint_t y, uint_t cpage = CR_LOCAL) const
    {
        return (egui_draw_tran2(m_that, text, x, y, cpage));
    }

    /* ============================================================================================================ */
    bool_t draw_text (const void_t* text, const sRECT* rect, uint_t type = EGUI_ALN_CM, uint_t cpage = CR_LOCAL) const
    {
        return (egui_draw_text(m_that, text, rect, type, cpage));
    }

    /* =================================================================================== */
    bool_t draw_text2 (const void_t* text, sint_t x, sint_t y, uint_t cpage = CR_LOCAL) const
    {
        return (egui_draw_text2(m_that, text, x, y, cpage));
    }

    /* =========================================================================== */
    bool_t calc_rect (const void_t* text, sRECT* rect, uint_t cpage = CR_LOCAL) const
    {
        return (CR_VCALL(m_that)->calc_rect(m_that, text, rect, cpage));
    }

    /* ===================================== */
    template<class T>T* get_port (void_t) const
    {
        return ((T*)m_that);
    }
};

}   /* namespace */

#endif  /* __IPORTS_HPP__ */
