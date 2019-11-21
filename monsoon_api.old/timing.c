/* Code extracted from ttcp.c */
#include <sys/time.h>

#if defined(SYSV)
#include <sys/times.h>
#include <sys/param.h>
struct rusage {
    struct timeval ru_utime, ru_stime;
};
#define RUSAGE_SELF 0
#else
#include <sys/resource.h>
#endif

static struct   timeval time0;  /* Time at which timing started */
static struct   rusage ru0;     /* Resource utilization at the start */

double cput, realt;

static void prusage();
static void tvadd();
static void tvsub();
static void psecs();

/*
 *                      P R E P _ T I M E R
 */
void
prep_timer()
{
        gettimeofday(&time0, (struct timezone *)0);
        getrusage(RUSAGE_SELF, &ru0);
}
                                                                                
/*
 *                      R E A D _ T I M E R
 *
 */
double
read_timer(str,len)
char *str;
{
        struct timeval timedol;
        struct rusage ru1;
        struct timeval td;
        struct timeval tend, tstart;
        char line[132];
                                                                                
        getrusage(RUSAGE_SELF, &ru1);
        gettimeofday(&timedol, (struct timezone *)0);
        prusage(&ru0, &ru1, &timedol, &time0, line);
        (void)strncpy( str, line, len );
                                                                                
        /* Get real time */
        tvsub( &td, &timedol, &time0 );
        realt = td.tv_sec + ((double)td.tv_usec) / 1000000;
                                                                                
        /* Get CPU time (user+sys) */
        tvadd( &tend, &ru1.ru_utime, &ru1.ru_stime );
        tvadd( &tstart, &ru0.ru_utime, &ru0.ru_stime );
        tvsub( &td, &tend, &tstart );
        cput = td.tv_sec + ((double)td.tv_usec) / 1000000;
        if( cput < 0.00001 )  cput = 0.00001;
        return( cput );
}

static void
prusage(r0, r1, e, b, outp)
        register struct rusage *r0, *r1;
        struct timeval *e, *b;
        char *outp;
{
        struct timeval tdiff;
        register time_t t;
        register char *cp;
        register int i;
        int ms;
                                                                                
        t = (r1->ru_utime.tv_sec-r0->ru_utime.tv_sec)*100+
            (r1->ru_utime.tv_usec-r0->ru_utime.tv_usec)/10000+
            (r1->ru_stime.tv_sec-r0->ru_stime.tv_sec)*100+
            (r1->ru_stime.tv_usec-r0->ru_stime.tv_usec)/10000;
        ms =  (e->tv_sec-b->tv_sec)*100 + (e->tv_usec-b->tv_usec)/10000;
                                                                                
#define END(x)  {while(*x) x++;}
#if defined(SYSV)
        cp = "%Uuser %Ssys %Ereal %P";
#else
#if defined(sgi)                /* IRIX 3.3 will show 0 for %M,%F,%R,%C */
        cp = "%Uuser %Ssys %Ereal %P %Mmaxrss %F+%Rpf %Ccsw";
#else
        cp = "%Uuser %Ssys %Ereal %P %Xi+%Dd %Mmaxrss %F+%Rpf %Ccsw";
#endif
#endif
        for (; *cp; cp++)  {
                if (*cp != '%')
                        *outp++ = *cp;
                else if (cp[1]) switch(*++cp) {
                                                                                
                case 'U':
                        tvsub(&tdiff, &r1->ru_utime, &r0->ru_utime);
                        sprintf(outp,"%d.%01d", tdiff.tv_sec, tdiff.tv_usec/100000);
                        END(outp);
                        break;
                                                                                
                case 'S':
                        tvsub(&tdiff, &r1->ru_stime, &r0->ru_stime);
                        sprintf(outp,"%d.%01d", tdiff.tv_sec, tdiff.tv_usec/100000);
                        END(outp);
                        break;
                                                                                
                case 'E':
                        psecs(ms / 100, outp);
                        END(outp);
                        break;
                                                                                
                case 'P':
                        sprintf(outp,"%d%%", (int) (t*100 / ((ms ? ms : 1))));
                        END(outp);
                        break;
                                                                                
#if !defined(SYSV)
                case 'W':
                        i = r1->ru_nswap - r0->ru_nswap;
                        sprintf(outp,"%d", i);
                        END(outp);
                        break;
                                                                                
                case 'X':
                        sprintf(outp,"%d", t == 0 ? 0 : (r1->ru_ixrss-r0->ru_ixrss)/t);
                        END(outp);
                        break;
                                                                                
                case 'D':
                        sprintf(outp,"%d", t == 0 ? 0 :
                            (r1->ru_idrss+r1->ru_isrss-(r0->ru_idrss+r0->ru_isrss))/t);
                        END(outp);
                        break;
                                                                                
                case 'K':
                        sprintf(outp,"%d", t == 0 ? 0 :
                            ((r1->ru_ixrss+r1->ru_isrss+r1->ru_idrss) -
                            (r0->ru_ixrss+r0->ru_idrss+r0->ru_isrss))/t);
                        END(outp);
                        break;
                                                                                
                case 'M':
                        sprintf(outp,"%d", r1->ru_maxrss/2);
                        END(outp);
                        break;
                                                                                
                case 'F':
                        sprintf(outp,"%d", r1->ru_majflt-r0->ru_majflt);
                        END(outp);
                        break;
                                                                                
                case 'R':
                        sprintf(outp,"%d", r1->ru_minflt-r0->ru_minflt);
                        END(outp);
                        break;
                                                                                
                case 'I':
                        sprintf(outp,"%d", r1->ru_inblock-r0->ru_inblock);
                        END(outp);
                        break;
                                                                                
                case 'O':
                        sprintf(outp,"%d", r1->ru_oublock-r0->ru_oublock);
                        END(outp);
                        break;
                case 'C':
                        sprintf(outp,"%d+%d", r1->ru_nvcsw-r0->ru_nvcsw,
                                r1->ru_nivcsw-r0->ru_nivcsw );
                        END(outp);
                        break;
#endif /* !SYSV */
                }
        }
        *outp = '\0';
}
                                                                                
static void
tvadd(tsum, t0, t1)
        struct timeval *tsum, *t0, *t1;
{
                                                                                
        tsum->tv_sec = t0->tv_sec + t1->tv_sec;
        tsum->tv_usec = t0->tv_usec + t1->tv_usec;
        if (tsum->tv_usec > 1000000)
                tsum->tv_sec++, tsum->tv_usec -= 1000000;
}
                                                                                
static void
tvsub(tdiff, t1, t0)
        struct timeval *tdiff, *t1, *t0;
{
                                                                                
        tdiff->tv_sec = t1->tv_sec - t0->tv_sec;
        tdiff->tv_usec = t1->tv_usec - t0->tv_usec;
        if (tdiff->tv_usec < 0)
                tdiff->tv_sec--, tdiff->tv_usec += 1000000;
}
                                                                                
static void
psecs(l,cp)
long l;
register char *cp;
{
        register int i;
                                                                                
        i = l / 3600;
        if (i) {
                sprintf(cp,"%d:", i);
                END(cp);
                i = l % 3600;
                sprintf(cp,"%d%d", (i/60) / 10, (i/60) % 10);
                END(cp);
        } else {
                i = l;
                sprintf(cp,"%d", i / 60);
                END(cp);
        }
        i %= 60;
        *cp++ = ':';
        sprintf(cp,"%d%d", i / 10, i % 10);
}
                                                                                

