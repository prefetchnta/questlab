/*
---------------------------------------
    输出调试日志
---------------------------------------
*/
static void logit (char *message)
{
    FILE*           fp;
    int32u          now, dlt;
    static int32u   base = 0;

    fp = fopen(QST_PATH_OUTPUT EXE_XNAME ".log", "a");
    if (fp != NULL)
    {
        sDATETIME   dttm;

        if (base == 0)
            base = GetTickCount();
        now = GetTickCount();
        if (now < base)
            dlt = 0xFFFFFFFFUL - base + now + 1;
        else
            dlt = now - base;
        base = now;
        if (base == 0)
            base++;
        datetime_get(&dttm);
        fprintf(fp, "%04u-%02u-%02u@%02u:%02u:%02u [+%5u] %s\n",
                    dttm.year, dttm.month, dttm.day, dttm.hour,
                    dttm.minute, dttm.second, dlt, message);
        fclose(fp);
    }
}
