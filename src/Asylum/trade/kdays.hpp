/*
***************************************
*   Trade System @ 2022-01-20
***************************************
*/

#ifndef __KDAYS_HPP__
#define __KDAYS_HPP__

/* Asylum Namespace */
namespace asy {

/**************/
/* K Day Draw */
/**************/
struct kday_draw
{
    uint_t  width;      // box pixel width
    uint_t  height;     // total pixel height
    double  min_val;    // minimum value
    double  max_val;    // maximum value

    /* ================== */
    void make_axis_mirror ()
    {
        double  amin = DABS(this->min_val);
        double  amax = DABS(this->max_val);

        if (amin > amax) {
            this->max_val =  amin;
            this->min_val = -amin;
        }
        else {
            this->max_val =  amax;
            this->min_val = -amax;
        }
    }

    /* ====================================================== */
    sint_t get_positive_y (double value, bool clip = true) const
    {
        sint_t  hhh = this->height - 1;
        double  dlt = this->max_val - this->min_val;
        double  tmp = ((value - this->min_val) / dlt) * hhh;
        sint_t  top = hhh - (sint_t)tmp;

        if (clip) {
            if (top < 0) return (0);
            return ((top >= hhh + 1) ? hhh : top);
        }
        return (top);
    }

    /* ====================================================== */
    sint_t get_complete_y (double value, bool clip = true) const
    {
        sint_t  hhh = this->height - 1;
        sint_t  top = this->height / 2;
        double  bar = (DABS(value) / this->max_val) * top;

        if (value >= 0)
            top -= (sint_t)bar;
        else
            top += (sint_t)bar;

        if (clip) {
            if (top < 0) return (0);
            return ((top >= hhh + 1) ? hhh : top);
        }
        return (top);
    }
};

/**************/
/* K Day Data */
/**************/
struct kday_data
{
    double  TCLOSE;
    double  HIGH;
    double  LOW;
    double  TOPEN;
    double  LCLOSE;
    double  CHG;
    double  PCHG;
    double  VOTURNOVER;
    double  VATURNOVER;

    /* ================================================================== */
    void price_box (sRECT* box, sint_t start_x, const kday_draw* info) const
    {
        sint_t  y1 = info->get_positive_y(this->TCLOSE);
        sint_t  y2 = info->get_positive_y(this->TOPEN);

        if (y1 >= y2)
            rect_set_wh(box, start_x, y2, info->width, y1 - y2 + 1);
        else
            rect_set_wh(box, start_x, y1, info->width, y2 - y1 + 1);
    }

    /* =============================================================================== */
    void halo_box (sRECT* box, sint_t start_x, const kday_draw* info, uint_t wline) const
    {
        sint_t  y1 = info->get_positive_y(this->HIGH);
        sint_t  y2 = info->get_positive_y(this->LOW);

        rect_set_wh(box, start_x + (info->width - wline) / 2, y1, wline, y2 - y1 + 1);
    }

    /* ============================================================================== */
    void value_box (sRECT* box, sint_t start_x, const kday_draw* info, bool count) const
    {
        sint_t  top;

        if (count)
            top = info->get_positive_y(this->VOTURNOVER);
        else
            top = info->get_positive_y(this->VATURNOVER);

        rect_set_wh(box, start_x, top, info->width, info->height - top);
    }
};

/**************/
/* K Day Node */
/**************/
struct kday_node
{
    // timestamp
    int64u  time;

    // day tdata
    union {
        double      dt[9];
        kday_data   detail;
    } data;

    /* ========= */
    void free () {}

    /* =========================== */
    void date (sDATETIME* date) const
    {
        datetime_from_unx(date, this->time);
    }
};

/**************/
/* K Day Line */
/**************/
struct kday_line
{
    double* data;

    /* ====== */
    void free ()
    {
        mem_free(this->data);
    }
};

/*********/
/* KDays */
/*********/
class kdays : public asylum
{
private:
    size_t              m_cnts;
    const char*         m_name;
    const char*         m_path;
    const kday_node*    m_list;
    map_acs<kday_line>  m_line;

public:
    /* ======================================== */
    bool init (const char* file, const char* name)
    {
        char*       tmp1;
        char*       tmp2;
        leng_t      size;
        kday_node*  list;

        size = sizeof(kday_node);
        if (size != 80)
            return (false);
        tmp1 = str_dupA(name);
        if (tmp1 == NULL)
            return (false);
        list = (kday_node*)file_load_as_binA(file, &size);
        if (list == NULL) {
            mem_free(tmp1);
            return (false);
        }
        if (size < sizeof(kday_node) ||
            size % sizeof(kday_node) != 0) {
            mem_free(list);
            mem_free(tmp1);
            return (false);
        }
        tmp2 = str_allocA(str_sizeA(file) + 1);
        if (tmp2 == NULL) {
            mem_free(list);
            mem_free(tmp1);
            return (false);
        }
        str_cpyA(tmp2, file);
        filext_removeA(tmp2);
        str_catA(tmp2, "\\");
        if (!m_line.init()) {
            mem_free(list);
            mem_free(tmp2);
            mem_free(tmp1);
            return (false);
        }
        m_name = tmp1;
        m_path = tmp2;
        m_list = list;
        m_cnts = size / sizeof(kday_node);
        return (true);
    }

    /* ====== */
    void free ()
    {
        m_line.free();
        mem_free(m_list);
        mem_free(m_name);
        mem_free(m_path);
    }

public:
    /* ============== */
    size_t size () const
    {
        return (m_cnts);
    }

    /* =================== */
    const char* name () const
    {
        return (m_name);
    }

    /* =================== */
    const char* path () const
    {
        return (m_path);
    }

    /* ======================= */
    const kday_node* get () const
    {
        return (m_list);
    }

    /* ================================= */
    const kday_node* get (size_t idx) const
    {
        return ((idx >= m_cnts) ? NULL : &m_list[idx]);
    }

    /* =============================== */
    double* line (const char* name) const
    {
        kday_line*  obj = m_line.get(name);

        return ((obj == NULL) ? NULL : obj->data);
    }

private:
    /* ================================================================================ */
    bool find_index (int64u tango, size_t beg, size_t end, size_t* index, bool next) const
    {
        size_t  mid = (beg + end) / 2;

        if (tango == m_list[mid].time) {
            *index = mid;
            return (true);
        }

        if (mid == beg) {
            *index = next ? beg : end;
            return (true);
        }

        if (tango > m_list[mid].time)
            return (this->find_index(tango, beg, mid, index, next));
        return (this->find_index(tango, mid, end, index, next));
    }

public:
    /* ==================================================================== */
    bool index_by_date (const sDATETIME* date, size_t* index, bool next) const
    {
        int64u  tick;

        if (!datetime_to_unx(&tick, date))
            return (false);

        if (tick >= m_list[0].time) {
            *index = 0;
            return (true);
        }
        if (tick <= m_list[m_cnts - 1].time) {
            *index = m_cnts - 1;
            return (true);
        }
        return (this->find_index(tick, 0, m_cnts - 1, index, next));
    }

    /* ============================================================= */
    bool make_range (kday_data* output, size_t start, size_t end) const
    {
        if (start >= m_cnts || end >= m_cnts)
            return (false);

        output->TCLOSE = m_list[start].data.detail.TCLOSE;
        if (start > end) {
            output->HIGH = 0;
            output->LOW = 0;
            output->TOPEN = output->TCLOSE;
            output->LCLOSE = output->TCLOSE;
            output->VOTURNOVER = 0;
            output->VATURNOVER = 0;
        }
        else {
            output->HIGH = m_list[start].data.detail.HIGH;
            output->LOW = m_list[start].data.detail.LOW;
            output->TOPEN = m_list[end].data.detail.TOPEN;
            output->LCLOSE = m_list[end].data.detail.LCLOSE;
            output->VOTURNOVER = m_list[start].data.detail.VOTURNOVER;
            output->VATURNOVER = m_list[start].data.detail.VATURNOVER;
            for (size_t idx = start + 1; idx <= end; idx++) {
                if (output->HIGH < m_list[idx].data.detail.HIGH)
                    output->HIGH = m_list[idx].data.detail.HIGH;
                if (output->LOW > m_list[idx].data.detail.LOW)
                    output->LOW = m_list[idx].data.detail.LOW;
                output->VOTURNOVER += m_list[idx].data.detail.VOTURNOVER;
                output->VATURNOVER += m_list[idx].data.detail.VATURNOVER;
            }
        }
        output->CHG = output->TCLOSE - output->LCLOSE;
        output->PCHG = output->CHG / output->LCLOSE;
        return (true);
    }

    /* ======================================================================= */
    bool get_range_week (const sDATETIME* date, size_t* start, size_t* end) const
    {
        sDATETIME   temp;

        temp.year = date->year;
        temp.month = date->month;
        temp.day = date->day;
        temp.hour = 0;
        temp.minute = 0;
        temp.second = 0;
        date_set_week(&temp);
        date_dec(&temp, temp.week);
        if (!index_by_date(&temp, end, true))
            return (false);
        date_inc(&temp, 6);
        if (!index_by_date(&temp, start, false))
            return (false);
        return (true);
    }

    /* ======================================================================== */
    bool get_range_month (const sDATETIME* date, size_t* start, size_t* end) const
    {
        sDATETIME   temp;

        temp.year = date->year;
        temp.month = date->month;
        temp.day = 1;
        temp.hour = 0;
        temp.minute = 0;
        temp.second = 0;
        if (!index_by_date(&temp, end, true))
            return (false);
        if (temp.month == 12) {
            temp.month = 1;
            temp.year++;
        }
        else {
            temp.month++;
        }
        date_dec(&temp, 1);
        if (!index_by_date(&temp, start, false))
            return (false);
        return (true);
    }

    /* ======================================================================= */
    bool get_range_year (const sDATETIME* date, size_t* start, size_t* end) const
    {
        sDATETIME   temp;

        temp.year = date->year;
        temp.month = 1;
        temp.day = 1;
        temp.hour = 0;
        temp.minute = 0;
        temp.second = 0;
        if (!index_by_date(&temp, end, true))
            return (false);
        temp.year++;
        date_dec(&temp, 1);
        if (!index_by_date(&temp, start, false))
            return (false);
        return (true);
    }

protected:
    /* ======================================================== */
    size_t load_line_file (kday_line* obj, const char* name) const
    {
        char*   file;
        leng_t  size;
        size_t  cnts;
        double* data;

        file = str_fmtA("%s\\%s", m_path, name);
        if (file == NULL)
            return (0);
        data = (double*)file_load_as_binA(file, &size);
        mem_free(file);
        if (data == NULL)
            return (0);
        if (size < sizeof(double) ||
            size % sizeof(double) != 0) {
            mem_free(data);
            return (0);
        }
        cnts = (size_t)(size / sizeof(double));
        if (cnts > m_cnts) {
            mem_free(data);
            return (0);
        }
        if (cnts == m_cnts) {
            obj->data = data;
            return (cnts);
        }
        obj->data = mem_talloc(m_cnts, double);
        if (obj->data == NULL) {
            mem_free(data);
            return (0);
        }
        mem_cpy(obj->data + m_cnts - cnts, data, size);
        mem_free(data);
        return (cnts);
    }

    /* ====================================================== */
    bool save_line_file (kday_line* obj, const char* name) const
    {
        char*   file;
        bool_t  retc;

        file = str_fmtA("%s\\%s", m_path, name);
        if (file == NULL)
            return (false);
        retc = file_saveA(file, obj->data, m_cnts * sizeof(double));
        mem_free(file);
        return (retc ? true : false);
    }

private:
    /* ====================================== */
    double WR (size_t start, uint_t count) const
    {
        size_t  ii, end = start + count;
        double  max = m_list[start].data.detail.HIGH;
        double  min = m_list[start].data.detail.LOW;
        double  now = m_list[start].data.detail.TCLOSE;

        for (ii = start + 1; ii < end; ii++) {
            if (ii >= m_cnts)
                break;
            if (max < m_list[ii].data.detail.HIGH)
                max = m_list[ii].data.detail.HIGH;
            if (min > m_list[ii].data.detail.LOW)
                min = m_list[ii].data.detail.LOW;
        }
        return ((max - now) / (max - min) * 100);
    }

    /* ======================================= */
    double RSV (size_t start, uint_t count) const
    {
        size_t  ii, end = start + count;
        double  max = m_list[start].data.detail.HIGH;
        double  min = m_list[start].data.detail.LOW;
        double  now = m_list[start].data.detail.TCLOSE;

        for (ii = start + 1; ii < end; ii++) {
            if (ii >= m_cnts)
                break;
            if (max < m_list[ii].data.detail.HIGH)
                max = m_list[ii].data.detail.HIGH;
            if (min > m_list[ii].data.detail.LOW)
                min = m_list[ii].data.detail.LOW;
        }
        return ((now - min) / (max - min) * 100);
    }

private:
    /* =============================================================================================== */
    double MA (size_t start, uint_t count, const double* data = NULL, size_t step = sizeof(double)) const
    {
        double  avg = 0;
        size_t  ii, jj, end = start + count;

        if (data == NULL) {
            data = (double*)(&m_list[0].data.detail.TCLOSE);
            step = sizeof(kday_node);
        }
        for (ii = start; ii < end; ii++) {
            jj = (ii >= m_cnts) ? m_cnts - 1 : ii;
            avg += *(double*)((byte_t*)data + jj * step);
        }
        return (avg / count);
    }

    /* =========================================================================================================== */
    double MD (size_t start, uint_t count, double avg, const double* data = NULL, size_t step = sizeof(double)) const
    {
        double  delta, sum = 0;
        size_t  ii, jj, end = start + count;

        if (data == NULL) {
            data = (double*)(&m_list[0].data.detail.TCLOSE);
            step = sizeof(kday_node);
        }
        for (ii = start; ii < end; ii++) {
            jj = (ii >= m_cnts) ? m_cnts - 1 : ii;
            delta = *(double*)((byte_t*)data + jj * step) - avg;
            sum += DABS(delta);
        }
        return (sum / count);
    }

    /* ===================================================================================================================================== */
    double BOLL (size_t start, uint_t count, double avg, uint_t mul, sint_t fix, const double* data = NULL, size_t step = sizeof(double)) const
    {
        double  delta, sd2 = 0;
        size_t  ii, jj, end = start + count;

        if (data == NULL) {
            data = (double*)(&m_list[0].data.detail.TCLOSE);
            step = sizeof(kday_node);
        }
        for (ii = start; ii < end; ii++) {
            jj = (ii >= m_cnts) ? m_cnts - 1 : ii;
            delta = *(double*)((byte_t*)data + jj * step) - avg;
            sd2 += delta * delta;
        }
        return (mul * DSQRT(sd2 / (count + fix)));
    }

public:
    /* ======================================================================================================= */
    bool make_MA (uint_t days, const char* alias = NULL, const double* data = NULL, size_t step = sizeof(double))
    {
        size_t      strt;
        size_t      cnts;
        kday_line   line;
        /* ----------- */
        char    name[64];

        if (alias == NULL) {
            sprintf(name, "MA%u", days);
            alias = name;
        }
        if (this->line(alias) != NULL)
            return (true);
        cnts = this->load_line_file(&line, alias);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++)
                line.data[idx] = this->MA(idx, days, data, step);

            // save line data
            this->save_line_file(&line, alias);
        }

        // insert line data
        if (m_line.insert(alias, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* ======================================================================================================== */
    bool make_EMA (uint_t days, const char* alias = NULL, const double* data = NULL, size_t step = sizeof(double))
    {
        size_t      strt;
        size_t      cnts;
        double      vals;
        kday_line   line;
        /* ----------- */
        char    name[64];

        if (alias == NULL) {
            sprintf(name, "EMA%u", days);
            alias = name;
        }
        if (this->line(alias) != NULL)
            return (true);
        cnts = this->load_line_file(&line, alias);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            if (data == NULL) {
                data = (double*)(&m_list[0].data.detail.TCLOSE);
                step = sizeof(kday_node);
            }
            strt = m_cnts - cnts - 1;
            if (strt == m_cnts - 1) {
                vals = *(double*)((byte_t*)data + strt * step);
                line.data[strt] = vals;
                if (strt-- == 0) {
                    this->save_line_file(&line, alias);
                    return (true);
                }
            }

            do {
                vals = *(double*)((byte_t*)data + strt * step);
                line.data[strt] = ((2 * vals) + (days - 1) * line.data[strt + 1]) / (days + 1);
            } while (strt-- != 0);

            // save line data
            this->save_line_file(&line, alias);
        }

        // insert line data
        if (m_line.insert(alias, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* ======================================================================================================================= */
    bool make_SMA (uint_t days, uint_t weight, const char* alias = NULL, const double* data = NULL, size_t step = sizeof(double))
    {
        size_t      strt;
        size_t      cnts;
        double      vals;
        kday_line   line;
        /* ----------- */
        char    name[64];

        if (weight > days)
            return (false);
        if (alias == NULL) {
            sprintf(name, "SMA(%u,%u)", days, weight);
            alias = name;
        }
        if (this->line(alias) != NULL)
            return (true);
        cnts = this->load_line_file(&line, alias);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            if (data == NULL) {
                data = (double*)(&m_list[0].data.detail.TCLOSE);
                step = sizeof(kday_node);
            }
            strt = m_cnts - cnts - 1;
            if (strt == m_cnts - 1) {
                vals = *(double*)((byte_t*)data + strt * step);
                line.data[strt] = vals;
                if (strt-- == 0) {
                    this->save_line_file(&line, alias);
                    return (true);
                }
            }

            do {
                vals = *(double*)((byte_t*)data + strt * step);
                line.data[strt] = ((weight * vals) + (days - weight) * line.data[strt + 1]) / days;
            } while (strt-- != 0);

            // save line data
            this->save_line_file(&line, alias);
        }

        // insert line data
        if (m_line.insert(alias, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* ================================================================================================== */
    bool make_DMA (double weight, const char* name, const double* data = NULL, size_t step = sizeof(double))
    {
        size_t      strt;
        size_t      cnts;
        double      vals;
        kday_line   line;

        if (weight > 1.0)
            return (false);
        if (this->line(name) != NULL)
            return (true);
        cnts = this->load_line_file(&line, name);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            if (data == NULL) {
                data = (double*)(&m_list[0].data.detail.TCLOSE);
                step = sizeof(kday_node);
            }
            strt = m_cnts - cnts - 1;
            if (strt == m_cnts - 1) {
                vals = *(double*)((byte_t*)data + strt * step);
                line.data[strt] = vals;
                if (strt-- == 0) {
                    this->save_line_file(&line, name);
                    return (true);
                }
            }

            do {
                vals = *(double*)((byte_t*)data + strt * step);
                line.data[strt] = (weight * vals) + (1 - weight) * line.data[strt + 1];
            } while (strt-- != 0);

            // save line data
            this->save_line_file(&line, name);
        }

        // insert line data
        if (m_line.insert(name, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* =========================================================================================================================== */
    bool make_DIFF (double mul1, double mul2, const char* name, const double* dat1, const double* dat2, size_t step = sizeof(double))
    {
        size_t      strt;
        size_t      cnts;
        kday_line   line;

        if (this->line(name) != NULL)
            return (true);
        cnts = this->load_line_file(&line, name);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++) {
                line.data[idx] = (*dat1) * mul1 - (*dat2) * mul2;
                dat1 = (double*)((byte_t*)dat1 + step);
                dat2 = (double*)((byte_t*)dat2 + step);
            }

            // save line data
            this->save_line_file(&line, name);
        }

        // insert line data
        if (m_line.insert(name, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

public:
    /* ========= */
    bool make_TP ()
    {
        size_t      strt;
        size_t      cnts;
        kday_line   line;

        if (this->line("TP") != NULL)
            return (true);
        cnts = this->load_line_file(&line, "TP");
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++) {
                line.data[idx]  = m_list[idx].data.detail.HIGH;
                line.data[idx] += m_list[idx].data.detail.LOW;
                line.data[idx] += m_list[idx].data.detail.TCLOSE;
                line.data[idx] /= 3;
            }

            // save line data
            this->save_line_file(&line, "TP");
        }

        // insert line data
        if (m_line.insert("TP", &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* =========== */
    bool make_GAIN ()
    {
        size_t      strt;
        size_t      cnts;
        kday_line   line;

        if (this->line("GAIN") != NULL)
            return (true);
        cnts = this->load_line_file(&line, "GAIN");
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++)
            {
                double  delta;

                if (idx == m_cnts - 1 &&
                    m_list[idx].data.detail.LCLOSE == 0) {
                    line.data[idx] = 0;
                }
                else {
                    delta  = m_list[idx].data.detail.TCLOSE;
                    delta -= m_list[idx].data.detail.LCLOSE;
                    line.data[idx] = (delta > 0) ? delta : 0;
                }
            }

            // save line data
            this->save_line_file(&line, "GAIN");
        }

        // insert line data
        if (m_line.insert("GAIN", &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* =========== */
    bool make_LOSS ()
    {
        size_t      strt;
        size_t      cnts;
        kday_line   line;

        if (this->line("LOSS") != NULL)
            return (true);
        cnts = this->load_line_file(&line, "LOSS");
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++)
            {
                double  delta;

                if (idx == m_cnts - 1 &&
                    m_list[idx].data.detail.LCLOSE == 0) {
                    line.data[idx] = 0;
                }
                else {
                    delta  = m_list[idx].data.detail.TCLOSE;
                    delta -= m_list[idx].data.detail.LCLOSE;
                    line.data[idx] = (delta < 0) ? -delta : 0;
                }
            }

            // save line data
            this->save_line_file(&line, "LOSS");
        }

        // insert line data
        if (m_line.insert("LOSS", &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

public:
    /* ============================================================== */
    bool make_MACD (uint_t fast = 12, uint_t slow = 26, uint_t diff = 9)
    {
        char    name[64];
        double* line_dif;
        double* line_dea;
        double* line_fast;
        double* line_slow;

        // fast EMA
        if (!this->make_EMA(fast))
            return (false);
        sprintf(name, "EMA%u", fast);
        line_fast = this->line(name);
        if (line_fast == NULL)
            return (false);

        // slow EMA
        if (!this->make_EMA(slow))
            return (false);
        sprintf(name, "EMA%u", slow);
        line_slow = this->line(name);
        if (line_slow == NULL)
            return (false);

        // DIF
        sprintf(name, "DIF(%u,%u)", fast, slow);
        if (!this->make_DIFF(1, 1, name, line_fast, line_slow))
            return (false);
        line_dif = this->line(name);
        if (line_dif == NULL)
            return (false);

        // DEA
        sprintf(name, "DEA(%u,%u,%u)", fast, slow, diff);
        if (!this->make_EMA(diff, name, line_dif))
            return (false);
        line_dea = this->line(name);
        if (line_dea == NULL)
            return (false);

        // MACD
        sprintf(name, "MACD(%u,%u,%u)", fast, slow, diff);
        if (!this->make_DIFF(2, 2, name, line_dif, line_dea))
            return (false);
        return (true);
    }

    /* =============================================== */
    bool make_RSV (uint_t days, const char* alias = NULL)
    {
        size_t      strt;
        size_t      cnts;
        kday_line   line;
        /* ----------- */
        char    name[64];

        if (alias == NULL) {
            sprintf(name, "RSV%u", days);
            alias = name;
        }
        if (this->line(alias) != NULL)
            return (true);
        cnts = this->load_line_file(&line, alias);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++)
                line.data[idx] = this->RSV(idx, days);

            // save line data
            this->save_line_file(&line, alias);
        }

        // insert line data
        if (m_line.insert(alias, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* =========================================================== */
    bool make_KDJ (uint_t days = 9, uint_t fast = 3, uint_t slow = 3)
    {
        char    name[64];
        double* line_rsv;
        double* line_fast;
        double* line_slow;

        // RSV
        if (!this->make_RSV(days))
            return (false);
        sprintf(name, "RSV%u", days);
        line_rsv = this->line(name);
        if (line_rsv == NULL)
            return (false);

        // K
        sprintf(name, "K(%u,%u)", days, fast);
        if (!this->make_SMA(fast, 1, name, line_rsv))
            return (false);
        line_fast = this->line(name);
        if (line_fast == NULL)
            return (false);

        // D
        sprintf(name, "D(%u,%u,%u)", days, fast, slow);
        if (!this->make_SMA(slow, 1, name, line_fast))
            return (false);
        line_slow = this->line(name);
        if (line_slow == NULL)
            return (false);

        // J = 3 * K - 2 * D
        sprintf(name, "J(%u,%u,%u)", days, fast, slow);
        if (!this->make_DIFF(3, 2, name, line_fast, line_slow))
            return (false);
        return (true);
    }

    /* ===================== */
    bool make_RSI (uint_t days)
    {
        char    name[64];
        double* line_gain;
        double* line_loss;

        // GAIN
        if (!this->make_GAIN())
            return (false);
        line_gain = this->line("GAIN");
        if (line_gain == NULL)
            return (false);

        // LOSS
        if (!this->make_LOSS())
            return (false);
        line_loss = this->line("LOSS");
        if (line_loss == NULL)
            return (false);

        // GAIN SMA
        sprintf(name, "SMA(G,%u)", days);
        if (!this->make_SMA(days, 1, name, line_gain))
            return (false);
        line_gain = this->line(name);
        if (line_gain == NULL)
            return (false);

        // LOSS SMA
        sprintf(name, "SMA(L,%u)", days);
        if (!this->make_SMA(days, 1, name, line_loss))
            return (false);
        line_loss = this->line(name);
        if (line_loss == NULL)
            return (false);

        size_t      strt;
        size_t      cnts;
        kday_line   line;

        // RSI
        sprintf(name, "RSI%u", days);
        if (this->line(name) != NULL)
            return (true);
        cnts = this->load_line_file(&line, name);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++) {
                if (line_loss[idx] < CR_ABIT64)
                    line.data[idx] = (line_gain[idx] < CR_ABIT64) ? 0 : 100;
                else
                    line.data[idx] = 100 - 100 / (1 + line_gain[idx] / line_loss[idx]);
            }

            // save line data
            this->save_line_file(&line, name);
        }

        // insert line data
        if (m_line.insert(name, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* ================================================================ */
    bool make_RSI3 (uint_t fast = 6, uint_t normal = 12, uint_t slow = 24)
    {
        if (!this->make_RSI(fast))
            return (false);
        if (!this->make_RSI(normal))
            return (false);
        return (this->make_RSI(slow));
    }

    /* ============================================================ */
    bool make_BOLL (uint_t days = 20, uint_t mul = 2, sint_t fix = -1)
    {
        char    name[64];
        double* line_avg;

        // MA
        if (!this->make_MA(days))
            return (false);
        sprintf(name, "MA%u", days);
        line_avg = this->line(name);
        if (line_avg == NULL)
            return (false);

        size_t      strt;
        size_t      cnts;
        kday_line   line;

        // SD with FIX
        sprintf(name, "SD(%u,%u,%d)", days, mul, fix);
        if (this->line(name) != NULL)
            return (true);
        cnts = this->load_line_file(&line, name);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++)
                line.data[idx] = this->BOLL(idx, days, line_avg[idx], mul, fix);

            // save line data
            this->save_line_file(&line, name);
        }

        // insert line data
        if (m_line.insert(name, &line) == NULL) {
            line.free();
            return (false);
        }

        // BOLL UP
        sprintf(name, "BOLL(UP,%u,%u,%d)", days, mul, fix);
        if (!this->make_DIFF(1, -1, name, line_avg, line.data))
            return (false);

        // BOLL DOWN
        sprintf(name, "BOLL(DN,%u,%u,%d)", days, mul, fix);
        if (!this->make_DIFF(1,  1, name, line_avg, line.data))
            return (false);
        return (true);
    }

    /* ====================================================================================== */
    bool make_BBI (uint_t level1 = 3, uint_t level2 = 6, uint_t level3 = 12, uint_t level4 = 24)
    {
        char    name[64];
        double* line_lv1;
        double* line_lv2;
        double* line_lv3;
        double* line_lv4;

        // MA Level1
        if (!this->make_MA(level1))
            return (false);
        sprintf(name, "MA%u", level1);
        line_lv1 = this->line(name);
        if (line_lv1 == NULL)
            return (false);

        // MA Level2
        if (!this->make_MA(level2))
            return (false);
        sprintf(name, "MA%u", level2);
        line_lv2 = this->line(name);
        if (line_lv2 == NULL)
            return (false);

        // MA Level3
        if (!this->make_MA(level3))
            return (false);
        sprintf(name, "MA%u", level3);
        line_lv3 = this->line(name);
        if (line_lv3 == NULL)
            return (false);

        // MA Level4
        if (!this->make_MA(level4))
            return (false);
        sprintf(name, "MA%u", level4);
        line_lv4 = this->line(name);
        if (line_lv4 == NULL)
            return (false);

        size_t      strt;
        size_t      cnts;
        kday_line   line;

        // BBI
        sprintf(name, "BBI(%u,%u,%u,%u)", level1, level2, level3, level4);
        if (this->line(name) != NULL)
            return (true);
        cnts = this->load_line_file(&line, name);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++) {
                line.data[idx]  = line_lv1[idx];
                line.data[idx] += line_lv2[idx];
                line.data[idx] += line_lv3[idx];
                line.data[idx] += line_lv4[idx];
                line.data[idx] /= 4;
            }

            // save line data
            this->save_line_file(&line, name);
        }

        // insert line data
        if (m_line.insert(name, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* ========================= */
    bool make_WR (uint_t days = 10)
    {
        size_t      strt;
        size_t      cnts;
        kday_line   line;
        /* ----------- */
        char    name[64];

        sprintf(name, "WR%u", days);
        if (this->line(name) != NULL)
            return (true);
        cnts = this->load_line_file(&line, name);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++)
                line.data[idx] = this->WR(idx, days);

            // save line data
            this->save_line_file(&line, name);
        }

        // insert line data
        if (m_line.insert(name, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

    /* ========================== */
    bool make_CCI (uint_t days = 14)
    {
        double* line_pt;
        double* line_ma;
        double* line_md;

        // TP
        if (!this->make_TP())
            return (false);
        line_pt = this->line("TP");
        if (line_pt == NULL)
            return (false);

        char    name[64];

        // TP MA
        sprintf(name, "MA(TP,%u)", days);
        if (!this->make_MA(days, name, line_pt))
            return (false);
        line_ma = this->line(name);
        if (line_ma == NULL)
            return (false);

        size_t      strt;
        size_t      cnts;
        kday_line   line;

        // TP MD
        sprintf(name, "MD(TP,%u)", days);
        if (this->line(name) != NULL)
            return (true);
        cnts = this->load_line_file(&line, name);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++)
                line.data[idx] = this->MD(idx, days, line_ma[idx], line_pt);

            // save line data
            this->save_line_file(&line, name);
        }

        // insert line data
        if (m_line.insert(name, &line) == NULL) {
            line.free();
            return (false);
        }
        line_md = line.data;

        // CCI
        sprintf(name, "CCI%u", days);
        if (this->line(name) != NULL)
            return (true);
        cnts = this->load_line_file(&line, name);
        if (cnts < m_cnts)
        {
            // complete data
            if (cnts == 0) {
                line.data = mem_talloc(m_cnts, double);
                if (line.data == NULL)
                    return (false);
            }
            strt = m_cnts - cnts;
            for (size_t idx = 0; idx < strt; idx++) {
                line.data[idx] = line_pt[idx] - line_ma[idx];
                line.data[idx] /= 0.015 * line_md[idx];
            }

            // save line data
            this->save_line_file(&line, name);
        }

        // insert line data
        if (m_line.insert(name, &line) == NULL) {
            line.free();
            return (false);
        }
        return (true);
    }

public:
    /* ============================================== */
    bool make_VO (uint_t level1 = 5, uint_t level2 = 10)
    {
        char    name[64];

        // VO Level1
        sprintf(name, "VO%u", level1);
        if (!this->make_MA(level1, name, (double*)(&m_list[0].data.detail.VOTURNOVER), sizeof(kday_data)))
            return (false);

        // VO Level2
        sprintf(name, "VO%u", level2);
        if (!this->make_MA(level2, name, (double*)(&m_list[0].data.detail.VOTURNOVER), sizeof(kday_data)))
            return (false);
        return (true);
    }

    /* ============================================== */
    bool make_VA (uint_t level1 = 5, uint_t level2 = 10)
    {
        char    name[64];

        // VA Level1
        sprintf(name, "VA%u", level1);
        if (!this->make_MA(level1, name, (double*)(&m_list[0].data.detail.VATURNOVER), sizeof(kday_data)))
            return (false);

        // VA Level2
        sprintf(name, "VA%u", level2);
        if (!this->make_MA(level2, name, (double*)(&m_list[0].data.detail.VATURNOVER), sizeof(kday_data)))
            return (false);
        return (true);
    }
};

}   /* namespace */

#endif  /* __KDAYS_HPP__ */
