#define ceu_out_assert(v)
#define ceu_out_log(m,s)

#include "_ceu_app.src"

int notmain ( void )
{
    byte CEU_DATA[sizeof(CEU_Main)];
    tceu_app app;
        app.data = (tceu_org*) &CEU_DATA;
        app.init = &ceu_app_init;

    int ret = ceu_go_all(&app);
    return ret;
}
