/*
 * mf2t
 * 
 * Convert a MIDI file to text.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if defined _WIN32 || defined MSDOS
  #include <io.h>
  #include <fcntl.h>
  #include "getopt.h"
#elif defined ATARIST
  #include <getopt.h>
#endif

#include <errno.h>
#include "midifile.h"
#include "version.h"

static int TrkNr;
static int TrksToDo = 1;
static int Measure, M0, Beat, Clicks;
static long T0;

/* options */

static int fold = 0;		/* fold long lines */
static int notes = 0;		/* print notes as a–g */
static int on0_to_off = 0;   /* replace On vol=0 with Off or vv*/
static int times = 0;		/* print times as Measure/beat/click */

static char *Onmsg  = "On ch=%d n=%s v=%d\n";
static char *Offmsg = "Off ch=%d n=%s v=%d\n";
static char *PoPrmsg = "PoPr ch=%d n=%s v=%d\n";
static char *Parmsg = "Par ch=%d c=%d v=%d\n";
static char *Pbmsg  = "Pb ch=%d v=%d\n";
static char *PrChmsg = "PrCh ch=%d p=%d\n";
static char *ChPrmsg = "ChPr ch=%d v=%d\n";

static void myerror(char *s)
{
    if (TrksToDo <= 0)
        fprintf(stderr, "Error: Garbage at end\n");
    else
        fprintf(stderr, "Error: %s\n", s);
}

#ifdef ATARIST
static int read_from_tty = 0;
#endif

int mygetchar() {
    int c = getc(infile);
#ifdef ATARIST
    if (read_from_tty) {
        /* On Atari interpret CTRL-C and CTRL-Z if reading from keyboard */
        if (c == 3) exit(2);     /* CTRL-C = interrupt */
        if (c == 26) return EOF; /* CTRL-Z = end of file */
    }
#endif
    return c;
}

static void prtime(void)
{
    if (times) {
        long m = (Mf_currtime-T0)/Beat;
        fprintf(outfile, "%ld:%ld:%ld ",
                m/Measure+M0, m%Measure, (Mf_currtime-T0)%Beat);
    } else
        fprintf(outfile, "%ld ",Mf_currtime);
}

static void prtext(unsigned char *p, int leng)
{
    int n, c;
    int pos = 25;

    fprintf(outfile, "\"");
    for (n = 0; n < leng; n++) {
        c = *p++;
        if (fold && pos >= fold) {
            fprintf(outfile, "\\\n\t");
            pos = 13;	/* tab + \xab + \ */
            if (c == ' ' || c == '\t') {
                putc('\\', outfile);
                ++pos;
            }
        }
        switch (c) {
            case '\\':
            case '"':
                fprintf(outfile, "\\%c", c);
                pos += 2;
                break;
            case '\r':
                fprintf(outfile, "\\r");
                pos += 2;
                break;
            case '\n':
                fprintf(outfile, "\\n");
                pos += 2;
                break;
            case '\0':
                fprintf(outfile, "\\0");
                pos += 2;
                break;
            default:
                if (c >= 0x20) {
                    putc(c, outfile);
                    ++pos;
                } else {
                    fprintf(outfile, "\\x%02x" , c);
                    pos += 4;
                }
        }
    }
    fprintf(outfile, "\"\n");
}

static void prhex(unsigned char *p,  int leng)
{
    int n;
    int pos = 25;

    for (n = 0; n < leng; n++, p++) {
        if (fold && pos >= fold) {
            fprintf(outfile, "\\\n\t%02x", *p);
            pos = 14;	/* tab + ab + " ab" + \ */
        } else {
            fprintf(outfile, " %02x" , *p);
            pos += 3;
        }
    }
    fprintf(outfile, "\n");
}

static char *mknote(int pitch)
{
    static char *Notes[] =
        { "c", "c#", "d", "d#", "e", "f", "f#", "g",
          "g#", "a", "a#", "b" };
    static char buf[20];
    if (notes)
        sprintf(buf, "%s%d", Notes[pitch % 12], pitch/12);
    else
        sprintf(buf, "%d", pitch);
    return buf;
}

static void myheader(int format, int ntrks, int division)
{
    if (division & 0x8000) { /* SMPTE */
        times = 0; /* Can’t do beats */
        fprintf(outfile, "MFile %d %d %d %d\n",format,ntrks,
                -((-(division>>8))&0xff), division&0xff);
    } else
        fprintf(outfile, "MFile %d %d %d\n",format,ntrks,division);
    if (format > 2) {
        fprintf(stderr, "Can't deal with format %d files\n", format);
        exit (1);
    }
    Beat = Clicks = division;
    TrksToDo = ntrks;
}

static void mytrstart(void)
{
    fprintf(outfile, "MTrk\n");
    TrkNr ++;
}

static void mytrend(void)
{
    fprintf(outfile, "TrkEnd\n");
    --TrksToDo;
}

static void mynoteoff(int, int, int);

static void mynoteon(int chan, int pitch, int vol)
{
    if ((on0_to_off > 0) && vol == 0) {
        mynoteoff(chan, pitch, vol);
        return;
    }
    prtime();
    fprintf(outfile, Onmsg, chan+1, mknote(pitch), vol);
}

static void mynoteoff(int chan, int pitch, int vol)
{
    if (on0_to_off < 0) {
        mynoteon(chan, pitch, 0);
        return;
    }
    prtime();
    fprintf(outfile, Offmsg, chan+1, mknote(pitch), vol);
}

static void mypressure(int chan, int pitch, int press)
{
    prtime();
    fprintf(outfile, PoPrmsg, chan+1, mknote(pitch), press);
}

static void myparameter(int chan, int control, int value)
{
    prtime();
    fprintf(outfile, Parmsg, chan+1, control, value);
}

static void mypitchbend(int chan, int lsb, int msb)
{
    prtime();
    fprintf(outfile, Pbmsg, chan+1, 128*msb+lsb);
}

static void myprogram(int chan, int program)
{
    prtime();
    fprintf(outfile, PrChmsg, chan+1, program);
}

static void mychanpressure(int chan, int press)
{
    prtime();
    fprintf(outfile, ChPrmsg, chan+1, press);
}

static void mysysex(int leng, char *mess)
{
    prtime();
    fprintf(outfile, "SysEx");
    prhex((unsigned char *)mess, leng);
}

static void mymmisc(int type, int leng, char *mess)
{
    prtime();
    fprintf(outfile, "Meta 0x%02x",type);
    prhex((unsigned char *)mess, leng);
}

static void mymspecial(int leng, char *mess)
{
    prtime();
    fprintf(outfile, "SeqSpec");
    prhex((unsigned char *)mess, leng);
}

static void mymtext(int type, int leng, char *mess)
{
    static char *ttype[] = {
        NULL,
        "Text",         /* type=0x01 */
        "Copyright",    /* type=0x02 */
        "TrkName",
        "InstrName",    /* ...       */
        "Lyric",
        "Marker",
        "Cue",          /* type=0x07 */
        "Unrec"
    };
    int unrecognized = (sizeof(ttype)/sizeof(char *)) - 1;

    prtime();
    if (type < 1 || type > unrecognized)
        fprintf(outfile, "Meta 0x%02x ",type);
    else if (type == 3 && TrkNr == 1)
        fprintf(outfile, "Meta SeqName ");
    else
        fprintf(outfile, "Meta %s ",ttype[type]);
    prtext((unsigned char *)mess, leng);
}

static void mymseq(int num)
{
    prtime();
    fprintf(outfile, "SeqNr %d\n",num);
}

static void mymeot(void)
{
    prtime();
    fprintf(outfile, "Meta TrkEnd\n");
}

static void mykeysig(int sf, int mi)
{
    prtime();
    fprintf(outfile, "KeySig %d %s\n", (sf>127?sf-256:sf), (mi?"minor":"major"));
}

static void mytempo(long tempo)
{
    prtime();
    fprintf(outfile, "Tempo %ld\n",tempo);
}

static void mytimesig(int nn, int dd, int cc, int bb)
{
    int denom = 1;
    while (dd-- > 0)
        denom *= 2;
    prtime();
    fprintf(outfile, "TimeSig %d/%d %d %d\n", nn,denom,cc,bb);
    M0 += (Mf_currtime-T0)/(Beat*Measure);
    T0 = Mf_currtime;
    Measure = nn;
    Beat = 4 * Clicks / denom;
}

static void mysmpte(int hr, int mn, int se, int fr, int ff)
{
    prtime();
    fprintf(outfile, "SMPTE %d %d %d %d %d\n", hr, mn, se, fr, ff);
}

static void myarbitrary(int leng, char *mess)
{
    prtime();
    fprintf(outfile, "Arb");
    prhex ((unsigned char *)mess, leng);
}

static void initfuncs(void)
{
    Mf_error = myerror;
    Mf_getc = mygetchar;
    Mf_header =  myheader;
    Mf_starttrack =  mytrstart;
    Mf_endtrack =  mytrend;
    Mf_on =  mynoteon;
    Mf_off =  mynoteoff;
    Mf_pressure =  mypressure;
    Mf_parameter =  myparameter;
    Mf_pitchbend =  mypitchbend;
    Mf_program =  myprogram;
    Mf_chanpressure =  mychanpressure;
    Mf_sysex =  mysysex;
    Mf_metamisc =  mymmisc;
    Mf_seqnum =  mymseq;
    Mf_eot =  mymeot;
    Mf_timesig =  mytimesig;
    Mf_smpte =  mysmpte;
    Mf_tempo =  mytempo;
    Mf_keysig =  mykeysig;
    Mf_sqspecific =  mymspecial;
    Mf_text =  mymtext;
    Mf_arbitrary =  myarbitrary;
}

static void usage(void)
{
    fprintf(stderr,
"mf2t v%s\n"
"Usage: mf2t [-mnbtv] [-on|-off] [-f n] [midifile [textfile]]\n\n"
"Options:\n"
"  -m      merge partial sysex into a single sysex message\n"
"  -n      write notes in symbolic form\n"
"  -on     replace Note On with vol=0 by Note Off\n"
"  -off    replace Note Off by Note On with vol=0\n"
"  -b|-t   write event times as bar:beat:click\n"
"  -v      use slightly more verbose output\n"
"  -f n    fold long text and hex entries at n characters\n", VERSION);
    exit(1);
}

int main(int argc, char **argv)
{
    int c;

    Mf_nomerge = 1;
    while ((c = getopt(argc, argv, "mno:btvf:h")) != -1) {
        switch (c) {
            case 'm':
                Mf_nomerge = 0;
                break;
            case 'n':
                notes++;
                break;
            case 'o':
                if (*optarg == 'n')
                    on0_to_off = 1;
                else if (*optarg == 'f')
                    on0_to_off = -1;
                else {
                    fprintf(stderr, "illegal option -o%s\n", optarg);
                }
                break;
            case 'b':
            case 't':
                times++;
                break;
            case 'v':
                Onmsg  = "On ch=%d note=%s vol=%d\n";
                Offmsg = "Off ch=%d note=%s vol=%d\n";
                PoPrmsg = "PolyPr ch=%d note=%s val=%d\n";
                Parmsg = "Param ch=%d con=%d val=%d\n";
                Pbmsg  = "Pb ch=%d val=%d\n";
                PrChmsg = "ProgCh ch=%d prog=%d\n";
                ChPrmsg = "ChanPr ch=%d val=%d\n";
                break;
            case 'f':
                fold = atoi(optarg);
                break;
            case 'h':
            case '?':
            default:
                usage();
        }
    }

    infile = stdin;
    /* Set infile to input file argument (midi file) if given
       Filename "-" indicates stdin
       Midi files are binary
    */
    if (optind < argc && strcmp(argv[optind++], "-") != 0) {
        if (!(infile = fopen(argv[optind - 1], "rb"))) {
            perror(argv[optind - 1]);
            exit(1);
        }
    } else {
        /* Set stdin to binary on platforms where it matters */
#if defined _WIN32 || defined MSDOS
        setmode(fileno(infile),O_BINARY);
#endif
#ifdef ATARIST
        if (!(infile = fdopen(fileno(stdin), "rb"))) {
            perror("Can't set stdin to binary mode\n");
        }
#endif
    }

#ifdef ATARIST
    read_from_tty = isatty(fileno(infile));
#endif

    outfile = stdout;
    /* Set outfile to output file argument (text file) if given
       Filename "-" indicates stdout
    */
    if (optind < argc && strcmp(argv[optind], "-") != 0
                      && !(outfile = fopen(argv[optind], "w"))) {
        perror(argv[optind]);
        exit(1);
    }

    initfuncs();
    TrkNr = 0;
    Measure = 4;
    Beat = 96;
    Clicks = 96;
    T0 = 0;
    M0 = 0;
    mfread();

    fclose(infile);
    fclose(outfile);

    return 0;
}
