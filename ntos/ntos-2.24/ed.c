/*
 *  ed - standard editor
 *  ~~
 *      Authors: Brian Beattie, Kees Bot, and others
 *
 * Copyright 1987 Brian Beattie Rights Reserved.
 * Permission to copy or distribute granted under the following conditions:
 * 1). No charge may be made other than reasonable charges for reproduction.
 * 2). This notice must remain intact.
 * 3). No further restrictions may be added.
 * 4). Except meaningless ones.
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *  TurboC mods and cleanup 8/17/88 RAMontante.
 *  Further information (posting headers, etc.) at end of file.
 *  RE stuff replaced with Spencerian version, sundry other bugfix+speedups
 *  Ian Phillipps. Version incremented to "5".
 * _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
 *  Changed the program to use more of the #define's that are at the top of
 *  this file.  Modified prntln() to print out a string instead of each
 *  individual character to make it easier to see if snooping a wizard who
 *  is in the editor.  Got rid of putcntl() because of the change to strings
 *  instead of characters, and made a define with the name of putcntl()
 *  Incremented version to "6".
 *  Scott Grosch / Belgarath    08/10/91
 * _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
 *
 *  ********--->> INDENTATION ONLINE !!!! <<----------****************
 *  Indentation added by Ted Gaunt (aka Qixx) paradox@mcs.anl.gov
 *  help files added by Ted Gaunt
 *  '^' added by Ted Gaunt
 *  Note: this version compatible with v.3.0.34  (and probably all others too)
 *  but i've only tested it on v.3 please mail me if it works on your mud
 *  and if you like it!
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 * Dworkin rewrote the Indentation algorithm, originally for Amylaar's driver,
 * around 5/20/1992.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 * Inspiral grabbed indentation code from Amylaar, hacked it into MudOS version
 * 0.9.18.17, around 12/11/1993.  Added some hacks to provide proper support
 * for "//" commenting, and took out a PROMPT define.
 *
 * -------------------------------------------------------------------------
 * Beek fixed Inspirals hacks, then later added the object_ed_* stuff.
 * ------------------------------------------------------------------------
 * Isaac Charles (Hamlet) did some reworks 2008/March
 * 1) Many ED_OUTPUTV() things combined for less calls to the output stuff.
 *    (this will be important with future changes and shouldn't hurt now)
 * 2) added an APPLY_RECEIVE_ED to send output within the user ob before
 *    outputting to the user.
 *    Prototype:  mixed receive_ed(string msg, string fname);
 *    If return is 0, it is a "pass" and normal output occurs.  If return
 *    is 1, we assume receive_ed() called receive() or something else
 *    clever and the text is taken care of.  If return is a string,
 *    that string is sent to the user rather than the original.
 *    If fname is 0, the incoming string is help text.  In this case,
 *    an array of strings may also be returned, which will be separated by
 *    "more" statements.  As above, if 1 is returned, no output, and if 0
 *    is returned, normal output.
 * 3) ed_start() and ed() now take an extra final argument telling
 *    how many lines on the user's screen.  'z' commands will then scroll
 *    that many lines (less 1 for the prompt).  'Z' commands scroll that many
 *    plus 5.  Why?  I dunno.  Why was it double before?  f_ed() and
 *    f_ed_start() do not require this arg.
 * 5) Minor fixes: fixed indentation for foreach(), and for lines with only
 *    //.
 * 6) Minor enhancements: z++ and z-- now work for double-forward or back.
 *    Added ED_INDENT_CASE (should 'case' be indented after switch?) and
 *    ED_INDENT_SPACES (how many spaces to indent with I) as defines in
 *    options.h.  Their presence is optional.  All additions to options.h
 *    are optional.  Default behavior is mostly the same as the original ed.
 */

#include "std.h"
#include "ed.h"
#include "comm.h"
#include "file.h"
#include "master.h"

#ifndef ED_INDENT_SPACES
#define ED_INDENT_SPACES 4
#endif

#ifndef ED_TAB_WIDTH
#define ED_TAB_WIDTH 8
#endif

/* Regexp is Henry Spencer's package. WARNING: regsub is modified to return
 * a pointer to the \0 after the destination string, and this program refers
 * to the "private" reganch field in the struct regexp.
 */

/* if you don't want ed, define OLD_ED and remove ed() from func_spec.c */
#if defined(F_ED) || !defined(OLD_ED)

/**  Global variables  **/

static int version = 601;

static int EdErr = 0;

static int append (int, int);
static int more_append (const char *);
static int ckglob (void);
static int deflt (int, int);
static int del (int, int);
static int dolst (int, int);
/* This seems completely unused; -Beek
static int esc (char **);
*/

static void do_output(char *);
static int doprnt (int, int);
static void prntln (char *, int, int);
static int egets (char *, int, FILE *);
static int doread (int, const char *);
static int dowrite (int, int, const char *, int);
static int find (regexp *, int);
static char *getfn (int);
static int getnum (int);
static int getone (void);
static int getlst (void);
static ed_line_t *getptr (int);
static int getrhs (char *);
static int ins (const char *);
static int join (int, int);
static int move (int);
static int transfer (int);
static regexp *optpat (void);
static int set (void);
static void set_ed_buf (void);
static int subst (regexp *, char *, int, int);
static int docmd (int);
static int doglob (void);
static void free_ed_buffer (object_t *);
static void shift (char *);
static void indent (char *);
static int indent_code (void);
static void report_status (int);

#ifndef OLD_ED
static char *object_ed_results (void);
static ed_buffer_t *add_ed_buffer (object_t *);
static void object_free_ed_buffer (void);
#endif

static void print_help (int arg);
static void print_help2 (void);
static void count_blanks (int line);
static void _count_blanks (char *str, int blanks);

static ed_buffer_t *current_ed_buffer;
static object_t *current_editor;        /* the object responsible */
outbuffer_t current_ed_results;

#define ED_BUFFER       (current_ed_buffer)
#ifdef OLD_ED
#define P_NET_DEAD      (command_giver->interactive->iflags & NET_DEAD)
#else
#define P_NET_DEAD      0 /* objects are never net dead :) */
#endif
#define P_NONASCII      (ED_BUFFER->nonascii)
#define P_NULLCHAR      (ED_BUFFER->nullchar)
#define P_TRUNCATED     (ED_BUFFER->truncated)
#define P_FNAME         (ED_BUFFER->fname)
#define P_FCHANGED      (ED_BUFFER->fchanged)
#define P_NOFNAME       (ED_BUFFER->nofname)
#define P_MARK          (ED_BUFFER->mark)
#define P_OLDPAT        (ED_BUFFER->oldpat)
#define P_LINE0         (ED_BUFFER->Line0)
#define P_CURLN         (ED_BUFFER->CurLn)
#define P_CURPTR        (ED_BUFFER->CurPtr)
#define P_LASTLN        (ED_BUFFER->LastLn)
#define P_LINE1         (ED_BUFFER->Line1)
#define P_LINE2         (ED_BUFFER->Line2)
#define P_NLINES        (ED_BUFFER->nlines)
#define P_HELPOUT	(ED_BUFFER->helpout)
/* shiftwidth is meant to be a 4-bit-value that can be packed into an int
   along with flags, therefore masks 0x1 ... 0x8 are reserved.           */
#define P_SHIFTWIDTH    (ED_BUFFER->shiftwidth)
#define P_FLAGS         (ED_BUFFER->flags)
#define NFLG_MASK       0x0010
#define P_NFLG          ( P_FLAGS & NFLG_MASK )
#define LFLG_MASK       0x0020
#define P_LFLG          ( P_FLAGS & LFLG_MASK )
#define PFLG_MASK       0x0040
#define P_PFLG          ( P_FLAGS & PFLG_MASK )
#define EIGHTBIT_MASK   0x0080
#define P_EIGHTBIT      ( P_FLAGS & EIGHTBIT_MASK )
#define AUTOINDFLG_MASK 0x0100
#define P_AUTOINDFLG    ( P_FLAGS & AUTOINDFLG_MASK )
#define EXCOMPAT_MASK   0x0200
#define P_EXCOMPAT      ( P_FLAGS & EXCOMPAT_MASK )
#define DPRINT_MASK     0x0400
#define P_DPRINT        ( P_FLAGS & DPRINT_MASK )
#define VERBOSE_MASK    0x0800
#define P_VERBOSE       ( P_FLAGS & VERBOSE_MASK )
#define SHIFTWIDTH_MASK 0x000f
#define ALL_FLAGS_MASK  0x0ff0
#define P_APPENDING     (ED_BUFFER->appending)
#define P_MORE          (ED_BUFFER->moring)
#define P_LEADBLANKS    (ED_BUFFER->leading_blanks)
#define P_CUR_AUTOIND   (ED_BUFFER->cur_autoindent)
#define P_RESTRICT      (ED_BUFFER->restricted)


static char inlin[ED_MAXLINE];
static char *inptr;             /* tty input buffer */

static struct tbl {
    const char *t_str;
    int t_and_mask;
    int t_or_mask;
}  *t, tbl[] = {
    { "number",               ~FALSE,                 NFLG_MASK       },
    { "nonumber",             ~NFLG_MASK,             FALSE           },
    { "list",                 ~FALSE,                 LFLG_MASK       },
    { "nolist",               ~LFLG_MASK,             FALSE           },
    { "print",                ~FALSE,                 PFLG_MASK       },
    { "noprint",              ~PFLG_MASK,             FALSE           },
    { "eightbit",             ~FALSE,                 EIGHTBIT_MASK   },
    { "noeightbit",           ~EIGHTBIT_MASK,         FALSE           },
    { "autoindent",           ~FALSE,                 AUTOINDFLG_MASK },
    { "noautoindent",         ~AUTOINDFLG_MASK,       FALSE           },
    { "excompatible",         ~FALSE,                 EXCOMPAT_MASK   },
    { "noexcompatible",       ~EXCOMPAT_MASK,         FALSE           },
    { "dprint",               ~FALSE,                 DPRINT_MASK     },
    { "nodprint",             ~DPRINT_MASK,           FALSE           },
    { "verbose",              ~FALSE,                 VERBOSE_MASK    },
    { "noverbose",            ~VERBOSE_MASK,          FALSE           },
};


/*________  Macros  ________________________________________________________*/

#ifndef max
#define max(a,b)        ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)        ((a) < (b) ? (a) : (b))
#endif

#define nextln(l)       ((l)+1 > P_LASTLN ? 0 : (l)+1)
#define prevln(l)       ((l)-1 < 0 ? P_LASTLN : (l)-1)

#define gettxtl(lin)    ((lin)->l_buff)
#define gettxt(num)     (gettxtl( getptr(num) ))

#define getnextptr(p)   ((p)->l_next)
#define getprevptr(p)   ((p)->l_prev)

#define setCurLn( lin ) ( P_CURPTR = getptr( P_CURLN = (lin) ) )
#define nextCurLn()     ( P_CURLN = nextln(P_CURLN), P_CURPTR = getnextptr( P_CURPTR ) )
#define prevCurLn()     ( P_CURLN = prevln(P_CURLN), P_CURPTR = getprevptr( P_CURPTR ) )

#define clrbuf()        del(1, P_LASTLN)

#define Skip_White_Space        {while (*inptr==SP || *inptr==HT) inptr++;}

#define relink(a, x, y, b) { (x)->l_prev = (a); (y)->l_next = (b); }


/*________  functions  ______________________________________________________*/


/*      append.c        */


static int append (int line, int glob)
{
    if (glob)
        return SYNTAX_ERROR;
    setCurLn(line);
    P_APPENDING = 1;
    if (P_NFLG)
        ED_OUTPUTV(ED_DEST, "%6d. ", P_CURLN + 1);
    if (P_CUR_AUTOIND)
        ED_OUTPUTV(ED_DEST, "%*s", P_LEADBLANKS, "");
#ifdef OLD_ED
    set_prompt("*\b");
#endif
    return 0;
}

static int more_append (const char *str)
{
    if (str[0] == '.' && str[1] == '\0') {
        P_APPENDING = 0;
#ifdef OLD_ED
        set_prompt(":");
#endif
        return (0);
    }
    if (P_NFLG)
        ED_OUTPUTV(ED_DEST, "%6d. ", P_CURLN + 2);
    if (P_CUR_AUTOIND) {
        int i;
        int less_indent_flag = 0;

        while (*str == '\004' || *str == '\013') {
            str++;
            P_LEADBLANKS -= P_SHIFTWIDTH;
            if (P_LEADBLANKS < 0)
                P_LEADBLANKS = 0;
            less_indent_flag = 1;
        }
        for (i = 0; i < P_LEADBLANKS;)
            inlin[i++] = ' ';
        strncpy(inlin + P_LEADBLANKS, str, ED_MAXLINE - P_LEADBLANKS);
        inlin[ED_MAXLINE - 1] = '\0';
        _count_blanks(inlin, 0);
        ED_OUTPUTV(ED_DEST, "%*s", P_LEADBLANKS, "");
        if (!*str && less_indent_flag)
            return 0;
        str = inlin;
    }
    if (ins(str) < 0)
        return (MEM_FAIL);
    return 0;
}

static void count_blanks (int line)
{
    _count_blanks(gettxtl(getptr(line)), 0);
}

static void _count_blanks (char * str, int blanks)
{
    for (; *str; str++) {
        if (*str == ' ')
            blanks++;
        else if (*str == '\t')
            blanks += ED_TAB_WIDTH - blanks % ED_TAB_WIDTH;
        else
            break;
    }
    P_LEADBLANKS = blanks < ED_MAXLINE ? blanks : ED_MAXLINE;
}

/*      ckglob.c        */

static int ckglob()
{
    regexp *glbpat;
    char c, delim, *lin;
    int num;
    ed_line_t *ptr;

    c = *inptr;

    if (c != 'g' && c != 'v')
        return (0);
    if (deflt(1, P_LASTLN) < 0)
        return (BAD_LINE_RANGE);

    delim = *++inptr;
    if (delim <= ' ')
        return (ERR);

    glbpat = optpat();
    if (*inptr == delim)
        inptr++;
    ptr = getptr(1);
    for (num = 1; num <= P_LASTLN; num++) {
        ptr->l_stat &= ~LGLOB;
        if (P_LINE1 <= num && num <= P_LINE2) {
            /*
             * we might have got a NULL pointer if the supplied pattern was
             * invalid
             */
            if (glbpat) {
                lin = gettxtl(ptr);
                if (regexec(glbpat, lin)) {
                    if (c == 'g')
                        ptr->l_stat |= LGLOB;
                } else {
                    if (c == 'v')
                        ptr->l_stat |= LGLOB;
                }
            }
        }
        ptr = getnextptr(ptr);
    }
    return (1);
}


/*  deflt.c
 *      Set P_LINE1 & P_LINE2 (the command-range delimiters) if the file is
 *      empty; Test whether they have valid values.
 */

static int deflt (int def1, int def2)
{
    if (P_NLINES == 0) {
        P_LINE1 = def1;
        P_LINE2 = def2;
    }
    return ((P_LINE1 > P_LINE2 || P_LINE1 <= 0) ? ERR : 0);
}


/*      del.c   */

/* One of the calls to this function tests its return value for an error
 * condition.  But del doesn't return any error value, and it isn't obvious
 * to me what errors might be detectable/reportable.  To silence a warning
 * message, I've added a constant return statement. -- RAM
 * ... It could check to<=P_LASTLN ... igp
 * ... Ok...and it corrects from (or to) > P_LASTLN also -- robo
 */

static int del (int from, int to)
{
    ed_line_t *first, *last, *next, *tmp;

    if (P_LASTLN == 0)
        return (0);             /* nothing to delete */
    if (from > P_LASTLN)
        from = P_LASTLN;
    if (to > P_LASTLN)
        to = P_LASTLN;
    if (from < 1)
        from = 1;
    if (to < 1)
        to = 1;
    first = getprevptr(getptr(from));
    last = getnextptr(getptr(to));
    next = first->l_next;
    while (next != last && next != &P_LINE0) {
        tmp = next->l_next;
        FREE((char *) next);
        next = tmp;
    }
    relink(first, last, first, last);
    P_LASTLN -= (to - from) + 1;
    setCurLn(prevln(from));
    return (0);
}


static int dolst (int line1, int line2)
{
    int oldflags = P_FLAGS, p;

    P_FLAGS |= LFLG_MASK;
    p = doprnt(line1, line2);
    P_FLAGS = oldflags;
    return p;
}


/*      esc.c
 * Map escape sequences into their equivalent symbols.  Returns the
 * correct ASCII character.  If no escape prefix is present then s
 * is untouched and *s is returned, otherwise **s is advanced to point
 * at the escaped character and the translated character is returned.
 */
/* UNUSED ----
static int esc (char ** s)
{
    register int rval;

    if (**s != ESCAPE) {
        rval = **s;
    } else {
        (*s)++;
        switch (islower(**s) ? toupper(**s) : **s) {
        case '\000':
            rval = ESCAPE;
            break;
        case 'S':
            rval = ' ';
            break;
        case 'N':
            rval = '\n';
            break;
        case 'T':
            rval = '\t';
            break;
        case 'B':
            rval = '\b';
            break;
        case 'R':
            rval = '\r';
            break;
        default:
            rval = **s;
            break;
        }
    }
    return (rval);
}
*/

/*      doprnt.c        */
static int doprnt (int from, int to)
{
    from = (from < 1) ? 1 : from;
    to = (to > P_LASTLN) ? P_LASTLN : to;

    if (to != 0) {
	setCurLn(from);
	while (P_CURLN <= to) {
	    prntln(gettxtl(P_CURPTR), P_LFLG, (P_NFLG ? P_CURLN : 0));
	    if (P_NET_DEAD || (P_CURLN == to))
		break;
	    nextCurLn();
	}
    }
    return (0);
}

static void do_output(char *str) {
#ifdef RECEIVE_ED
    svalue_t *ret;

    copy_and_push_string(str);
    copy_and_push_string(P_FNAME);
    // One could argue that this should be safe_apply()
    // Pro: ed continues to work with a runtiming receive_ed()
    // Con: "I wrote a receive_ed()!  Why does the driver ignore it??"
    ret = apply(APPLY_RECEIVE_ED, ED_DEST, 2, ORIGIN_DRIVER);
    if(!ret)
      ED_OUTPUTV(ED_DEST, "%s\n", str);
    else if(ret->type == T_NUMBER) {
      // if 0, output ourselves, else they handled it, do nothing.
      if(ret->u.number == 0) // "pass"
        ED_OUTPUTV(ED_DEST, "%s\n", str);
    }
    else if(ret->type == T_STRING) {
        ED_OUTPUTV(ED_DEST, "%s\n", ret->u.string);
    }
#else
    ED_OUTPUTV(ED_DEST, "%s\n", str);
#endif
}

static void free_ed_buffer (object_t * who)
{
    clrbuf();
#ifdef OLD_ED
    if (ED_BUFFER->write_fn) {
	object_t *exit_ob = ED_BUFFER->exit_ob;
        FREE(ED_BUFFER->write_fn);
        free_object(&exit_ob, "ed EOF");
    }
    if (ED_BUFFER->exit_fn) {
        char *exit_fn = ED_BUFFER->exit_fn;
        object_t *exit_ob = ED_BUFFER->exit_ob;

        if (P_OLDPAT)
            FREE((char *) P_OLDPAT);
        FREE((char *) ED_BUFFER);
        who->interactive->ed_buffer = 0;
        set_prompt("> ");

        /* make this "safe" */
        safe_apply(exit_fn, exit_ob, 0, ORIGIN_INTERNAL);
        FREE(exit_fn);
        free_object(&exit_ob, "ed EOF");
        return;
    }
#endif

    if (P_OLDPAT)
        FREE((char *) P_OLDPAT);
#ifdef OLD_ED
    FREE((char *) ED_BUFFER);
    who->interactive->ed_buffer = 0;
    set_prompt("> ");
#else
    object_free_ed_buffer();
#endif
    return;
}

#define putcntl(X) *line++ = '^'; *line++ = (X) ? ((*str&31)|'@') : '?'

static void prntln (char * str, int vflg, int lineno)
{
    char *line, start[ED_MAXLINE + 2]; // by Lonely.
    line = start;

    if (lineno) {
        ED_OUTPUTV(ED_DEST, "%3d  ", lineno);
    }
    while (*str && *str != NL) {
        if ((line - start) > ED_MAXLINE) {
            free_ed_buffer(current_editor);
            error("错误的档案格式。Ed 无法处理比 %d 还长的字串。\n", ED_MAXLINE);
        }
        //if (*str < ' ' || *str >= DEL) {
        if((*str < ' ') && (*str >='\0')) { // by Lonely
            switch (*str) {
            case '\t':
                /* have to be smart about this or the indentor will fail */
                *line++ = ' ';
                while ((line - start) % ED_TAB_WIDTH)
                    *line++ = ' ';
                break;
            case DEL:
                putcntl(0);
                break;
            default:
                putcntl(1);
                break;
            }
        } else
            *line++ = *str;
        str++;
    }
#ifndef NO_END_DOLLAR_SIGN
    if (vflg)
        *line++ = '$';
#endif
    *line = EOS;

    ED_OUTPUTV(ED_DEST, "%s\n", start);
}

/*      egets.c */

static int egets (char * str, int size, FILE * stream)
{
    int c = 0, count;
    char *cp;

    for (count = 0, cp = str; size > count;) {
        c = getc(stream);
        if (c == EOF) {
            *cp = EOS;
            if (count)
                ED_OUTPUT(ED_DEST, "[最后一行未完成]\n");
            return (count);
        } else if (c == NL) {
            *cp = EOS;
            return (++count);
        } else if (c == 0)
            P_NULLCHAR++;       /* count nulls */
        else {
            if (c > 127) {
                if (!P_EIGHTBIT)/* if not saving eighth bit */
                    c = c & 127;/* strip eigth bit */
                P_NONASCII++;   /* count it */
            }
            *cp++ = c;          /* not null, keep it */
            count++;
        }
    }
    str[count - 1] = EOS;
    if (c != NL) {
        ED_OUTPUT(ED_DEST, "删节该行\n");
        P_TRUNCATED++;
        while ((c = getc(stream)) != EOF)
            if (c == NL)
                break;
    }
    return (count);
}                               /* egets */

static int doread (int lin, const char * fname)
{
    FILE *fp;
    int err;
    unsigned int bytes;
    unsigned int lines;
    static char str[ED_MAXLINE];

    err = 0;
    P_NONASCII = P_NULLCHAR = P_TRUNCATED = 0;

    if (P_VERBOSE)
        ED_OUTPUTV(ED_DEST, "\"%s\" ", fname);
    if ((fp = fopen(fname, "r")) == NULL) {
        ED_OUTPUT(ED_DEST, " 不可读取。\n");
        return ERR;
    }
    setCurLn(lin);
    for (lines = 0, bytes = 0; (err = egets(str, ED_MAXLINE, fp)) > 0;) {
        bytes += err;
        if (ins(str) < 0) {
            err = MEM_FAIL;
            break;
        }
        lines++;
    }
    fclose(fp);
    if (err < 0)
        return (err);
    if (P_VERBOSE) {
        ED_OUTPUTV(ED_DEST, "共 %u 行 %u 位元组", lines, bytes);
        if (P_NONASCII)
            ED_OUTPUTV(ED_DEST, " [共 %d 非 ASCII 字元]", P_NONASCII);
        if (P_NULLCHAR)
            ED_OUTPUTV(ED_DEST, " [共 %d 空字串]", P_NULLCHAR);
        if (P_TRUNCATED)
            ED_OUTPUTV(ED_DEST, " [共截短 %d 行]", P_TRUNCATED);
        ED_OUTPUT(ED_DEST, "\n");
    }
    return (err);
}                               /* doread */

static int dowrite (int from, int to, const char * fname, int apflg)
{
    FILE *fp;
    int lin, err;
    unsigned int lines;
    unsigned int bytes;
    char *str;
    ed_line_t *lptr;

    err = 0;
    lines = bytes = 0;

#ifdef OLD_ED
    if (ED_BUFFER->write_fn) {
        svalue_t *res;

        push_malloced_string(add_slash(fname));
        push_number(0);
        res = safe_apply(ED_BUFFER->write_fn, ED_BUFFER->exit_ob, 2, ORIGIN_INTERNAL);
        if (IS_ZERO(res))
            return (ERR);
    }
#endif

    if (!P_RESTRICT)
        ED_OUTPUTV(ED_DEST, "\"/%s\" ", fname);
    if ((fp = fopen(fname, (apflg ? "a" : "w"))) == NULL) {
        if (!P_RESTRICT)
            ED_OUTPUT(ED_DEST, " 此档无法被开启来编辑。\n");
        else
            ED_OUTPUT(ED_DEST, "无法开启档案来编辑。\n");
        return ERR;
    }
    lptr = getptr(from);
    for (lin = from; lin <= to; lin++) {
        str = lptr->l_buff;
        lines++;
        bytes += strlen(str) + 1;       /* str + '\n' */
        if (fputs(str, fp) == EOF) {
            ED_OUTPUT(ED_DEST, "档案写入错误\n");
            err++;
            break;
        }
        fputc('\n', fp);
        lptr = lptr->l_next;
    }

    if (!P_RESTRICT)
        ED_OUTPUTV(ED_DEST, "共 %u 行 %lu 位元组\n", lines, bytes);
    fclose(fp);

#ifdef OLD_ED
    if (ED_BUFFER->write_fn) {
        push_malloced_string(add_slash(fname));
        push_number(1);
        safe_apply(ED_BUFFER->write_fn, ED_BUFFER->exit_ob, 2, ORIGIN_INTERNAL);
    }
#endif

    return (err);
}                               /* dowrite */

/*      find.c  */

static int find (regexp * pat, int dir)
{
    int i, num;
    ed_line_t *lin;

    if (!pat)
        return (ERR);
    dir ? nextCurLn() : prevCurLn();
    num = P_CURLN;
    lin = P_CURPTR;
    for (i = 0; i < P_LASTLN; i++) {
        if (regexec(pat, gettxtl(lin)))
            return (num);
        if (EdErr) {
            EdErr = 0;
            break;
        }
        if (dir)
            num = nextln(num), lin = getnextptr(lin);
        else
            num = prevln(num), lin = getprevptr(lin);
    }
/* Put us back to where we came from */
    dir ? prevCurLn() : nextCurLn();
    return (SEARCH_FAILED);
}                               /* find() */

/*      getfn.c */

static char *getfn (int writeflg)
{
    static char file[MAXFNAME];
    char *cp;
    const char *file2;
    svalue_t *ret;

    if (*inptr == NL) {
        P_NOFNAME = TRUE;
        file[0] = '/';
        strcpy(file + 1, P_FNAME);
    } else {
        P_NOFNAME = FALSE;
        Skip_White_Space;

        cp = file;
        while (*inptr && *inptr != NL && *inptr != SP && *inptr != HT)
            *cp++ = *inptr++;
        *cp = '\0';

    }
    if (strlen(file) == 0) {
        ED_OUTPUT(ED_DEST, "档案名称错误。\n");
        return (0);
    }

    if (file[0] != '/') {
        copy_and_push_string(file);
        ret = apply_master_ob(APPLY_MAKE_PATH_ABSOLUTE, 1);
        if ((ret == 0) || (ret == (svalue_t *)-1) || ret->type != T_STRING)
            return NULL;
        strncpy(file, ret->u.string, sizeof file - 1);
        file[MAXFNAME - 1] = '\0';
    }

    /* valid_read/valid_write done here */
    file2 = check_valid_path(file, current_editor, "ed_start", writeflg);
    if (!file2)
        return (NULL);
    strncpy(file, file2, MAXFNAME - 1);
    file[MAXFNAME - 1] = 0;

    if (*file == 0) {
        ED_OUTPUT(ED_DEST, "没有档案名称。\n");
        return (NULL);
    }
    return (file);
}                               /* getfn */

static int getnum (int first)
{
    regexp *srchpat;
    int num;
    char c;

    Skip_White_Space;

    if (*inptr >= '0' && *inptr <= '9') {       /* line number */
        for (num = 0; *inptr >= '0' && *inptr <= '9'; ++inptr) {
            num = (num * 10) + (*inptr - '0');
        }
        return num;
    }
    switch (c = *inptr) {
    case '.':
        inptr++;
        return (P_CURLN);

    case '$':
        inptr++;
        return (P_LASTLN);

    case '/':
    case '?':
        srchpat = optpat();
        if (*inptr == c)
            inptr++;
        return (find(srchpat, c == '/' ? 1 : 0));

    case '-':
    case '+':
        return (first ? P_CURLN : 1);

    case '\'':
        inptr++;
        if (*inptr < 'a' || *inptr > 'z')
            return MARK_A_TO_Z;
        return P_MARK[*inptr++ - 'a'];

    default:
        return (first ? NO_LINE_RANGE : 1);     /* unknown address */
    }
}                               /* getnum */


/*  getone.c
 *      Parse a number (or arithmetic expression) off the command line.
 */
#define FIRST 1
#define NOTFIRST 0

static int getone()
{
    int c, i, num;

    if ((num = getnum(FIRST)) >= 0) {
        for (;;) {
            Skip_White_Space;
            if (*inptr != '+' && *inptr != '-')
                break;          /* exit infinite loop */

            c = *inptr++;
            if ((i = getnum(NOTFIRST)) < 0)
                return (i);
            if (c == '+')
                num += i;
            else {
                num -= i;
                if (num < 1) num = 1;
            }
        }
    }
    return (num > P_LASTLN ? BAD_LINE_NUMBER : num);
}                               /* getone */


static int getlst()
{
    int num;

    P_LINE2 = 0;
    for (P_NLINES = 0; (num = getone()) >= 0 || (num == BAD_LINE_NUMBER);) {
        /* if it's out of bounds, go to the end of the file. */
        if (num == BAD_LINE_NUMBER)
            num = P_LASTLN;

        P_LINE1 = P_LINE2;
        P_LINE2 = num;
        P_NLINES++;
        if (*inptr != ',' && *inptr != ';')
            break;
        if (*inptr == ';')
            setCurLn(num);
        inptr++;
    }
    P_NLINES = min(P_NLINES, 2);
    if (P_NLINES == 0)
        P_LINE2 = P_CURLN;
    if (P_NLINES <= 1)
        P_LINE1 = P_LINE2;

    return ((num < 0) ? num : P_NLINES);
}                               /* getlst */


/*      getptr.c        */

static ed_line_t *getptr (int num)
{
    ed_line_t *ptr;
    int j;

    if (2 * num > P_LASTLN && num <= P_LASTLN) {        /* high line numbers */
        ptr = P_LINE0.l_prev;
        for (j = P_LASTLN; j > num; j--)
            ptr = ptr->l_prev;
    } else {                    /* low line numbers */
        ptr = &P_LINE0;
        for (j = 0; j < num; j++)
            ptr = ptr->l_next;
    }
    return (ptr);
}


/*      getrhs.c        */

static int getrhs (char * sub)
{
    char delim = *inptr++;
    char *outmax = sub + MAXPAT;

    if (delim == NL || *inptr == NL)    /* check for eol */
        return (ERR);
    while (*inptr != delim && *inptr != NL) {
        if (sub > outmax)
            return ERR;
        if (*inptr == ESCAPE) {
            switch (*++inptr) {
            case 'r':
                *sub++ = '\r';
                inptr++;
                break;
            case 'n':
                *sub++ = '\n';
                inptr++;
                break;
            case 'b':
                *sub++ = '\b';
                inptr++;
                break;
            case 't':
                *sub++ = '\t';
                inptr++;
                break;
            case '0':{
                    int i = 3;

                    *sub = 0;
                    do {
                        if (*++inptr < '0' || *inptr > '7')
                            break;
                        *sub = (*sub << 3) | (*inptr - '0');
                    } while (--i != 0);
                    sub++;
                } break;
            case '&':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '\\':
                *sub++ = ESCAPE;/* fall through */
            default:
                *sub++ = *inptr;
                if (*inptr != NL)
                    inptr++;
            }
        } else
            *sub++ = *inptr++;
    }
    *sub = '\0';

    inptr++;                    /* skip over delimter */
    Skip_White_Space;
    if (*inptr == 'g') {
        inptr++;
        return (1);
    }
    return (0);
}

/*      ins.c   */

static int ins (const char *str)
{
    const char *cp;
    ed_line_t *newl, *nxt;
    int len;

    do {
        for (cp = str; *cp && *cp != NL; cp++);
        len = cp - str;
        /* cp now points to end of first or only line */

        if ((newl = (ed_line_t *) DXALLOC(sizeof(ed_line_t) + len,
                                         TAG_ED, "ins: new")) == NULL)
            return (MEM_FAIL);  /* no memory */

        newl->l_stat = 0;
        strncpy(newl->l_buff, str, len); /* build new line */
        newl->l_buff[len] = EOS;
        nxt = getnextptr(P_CURPTR);     /* get next line */
        relink(P_CURPTR, newl, newl, nxt);        /* add to linked list */
        relink(newl, nxt, P_CURPTR, newl);
        P_LASTLN++;
        P_CURLN++;
        P_CURPTR = newl;
        str = cp + 1;
    }
    while (*cp != EOS);
    return 1;
}


/*      join.c  */

static int join (int first, int last)
{
    char buf[ED_MAXLINE];
    char *cp = buf, *str;
    ed_line_t *lin;
    int num;

    if (first <= 0 || first > last || last > P_LASTLN)
        return BAD_LINE_RANGE;
    if (first == last) {
        setCurLn(first);
        return 0;
    }
    lin = getptr(first);
    for (num = first; num <= last; num++) {
        str = gettxtl(lin);
        while (*str) {
            if (cp >= buf + ED_MAXLINE - 1) {
                ED_OUTPUT(ED_DEST, "此行过长\n");
                return ERR;
            }
            *cp++ = *str++;
        }
        lin = getnextptr(lin);
    }
    *cp = EOS;
    del(first, last);
    if (ins(buf) < 0)
        return MEM_FAIL;
    P_FCHANGED = TRUE;
    return 0;
}

/*  move.c
 *      Unlink the block of lines from P_LINE1 to P_LINE2, and relink them
 *      after line "num".
 */

static int move (int num)
{
    int range;
    ed_line_t *before, *first, *last, *after;

    if (P_LINE1 <= num && num <= P_LINE2)
        return BAD_LINE_RANGE;
    range = P_LINE2 - P_LINE1 + 1;
    before = getptr(prevln(P_LINE1));
    first = getptr(P_LINE1);
    last = getptr(P_LINE2);
    after = getptr(nextln(P_LINE2));

    relink(before, after, before, after);
    P_LASTLN -= range;          /* per AST's posted patch 2/2/88 */
    if (num > P_LINE1)
        num -= range;

    before = getptr(num);
    after = getptr(nextln(num));
    relink(before, first, last, after);
    relink(last, after, before, first);
    P_LASTLN += range;          /* per AST's posted patch 2/2/88 */
    setCurLn(num + range);
    return (1);
}


static int transfer (int num)
{
    int mid, lin, ntrans;

    if (P_LINE1 <= 0 || P_LINE1 > P_LINE2)
        return (ERR);

    mid = num < P_LINE2 ? num : P_LINE2;

    setCurLn(num);
    ntrans = 0;

    for (lin = P_LINE1; lin <= mid; lin++) {
        if (ins(gettxt(lin)) < 0)
            return MEM_FAIL;
        ntrans++;
    }
    lin += ntrans;
    P_LINE2 += ntrans;

    for (; lin <= P_LINE2; lin += 2) {
        if (ins(gettxt(lin)) < 0)
            return MEM_FAIL;
        P_LINE2++;
    }
    return (1);
}


/*      optpat.c        */

static regexp *optpat()
{
    char delim, str[MAXPAT], *cp;

    delim = *inptr++;
    if (delim == NL)
        return P_OLDPAT;
    cp = str;
    while (*inptr != delim && *inptr != NL && *inptr != EOS && cp < str + MAXPAT - 1) {
        if (*inptr == ESCAPE && inptr[1] != NL)
            *cp++ = *inptr++;
        *cp++ = *inptr++;
    }

    *cp = EOS;
    if (*str == EOS)
        return (P_OLDPAT);
    if (P_OLDPAT)
        FREE((char *) P_OLDPAT);
    return P_OLDPAT = regcomp((unsigned char *)str, P_EXCOMPAT);
}

static int set()
{
    char word[16];
    int i;
    struct tbl *limit;

    if (*(++inptr) != 't') {
        if (*inptr != SP && *inptr != HT && *inptr != NL)
            return SYNTAX_ERROR;
    } else
        inptr++;

    if ((*inptr == NL)) {
        char out[512];
        char *pos = out;

        pos += sprintf(pos, "ED 版本 %d.%d\n", version / 100,
                                                  version % 100);
        limit = tbl + (sizeof(tbl) / sizeof(struct tbl));
        for (t = tbl; t < limit; t += 2) {
            pos += sprintf(pos, "%s:%s ", t->t_str,
                        P_FLAGS & t->t_or_mask ? "开" : "关");
        }
        pos += sprintf(pos, "\n缩排空格(shiftwidth):%d\n", P_SHIFTWIDTH);

        ED_OUTPUT(ED_DEST, out);
        return (0);
    }
    Skip_White_Space;
    for (i = 0; *inptr != SP && *inptr != HT && *inptr != NL;) {
        if (i == sizeof word - 1) {
            ED_OUTPUT(ED_DEST, "你的参数过长，不符合 'set' 指令的要求。\n");
            return 0;
        }
        word[i++] = *inptr++;
    }
    word[i] = EOS;
    limit = tbl + (sizeof(tbl) / sizeof(struct tbl));
    for (t = tbl; t < limit; t++) {
        if (strcmp(word, t->t_str) == 0) {
            P_FLAGS = (P_FLAGS & t->t_and_mask) | t->t_or_mask;
            return (0);
        }
    }
    if (!strcmp(word, "save")) {
        svalue_t *ret;

        if (P_RESTRICT)
            return (SET_FAIL);
        push_object(current_editor);
        push_number(P_SHIFTWIDTH | P_FLAGS);
        ret = apply_master_ob(APPLY_SAVE_ED_SETUP, 2);
        if (MASTER_APPROVED(ret))
            return 0;
    }
    if (!strcmp(word, "shiftwidth")) {
        Skip_White_Space;
        if (uisdigit(*inptr)) {
            P_SHIFTWIDTH = *inptr - '0';
            return 0;
        }
    }
    return SET_FAIL;
}

#ifndef relink
void relink (LINE * a, LINE * x, LINE * y, LINE * b)
{
    x->l_prev = a;
    y->l_next = b;
}
#endif


static void set_ed_buf()
{
    relink(&P_LINE0, &P_LINE0, &P_LINE0, &P_LINE0);
    P_CURLN = P_LASTLN = 0;
    P_CURPTR = &P_LINE0;
}


/*      subst.c */

static int subst (regexp * pat, char * sub, int gflg, int pflag)
{
    int nchngd = 0;
    const char *txtptr;
    char *newc, *old, buf[ED_MAXLINE];
    int space;                  /* amylaar */
    int still_running = 1;
    ed_line_t *lastline = getptr(P_LINE2);

    if (P_LINE1 <= 0)
        return (SUB_FAIL);
    nchngd = 0;                 /* reset count of lines changed */

    for (setCurLn(prevln(P_LINE1)); still_running;) {
        nextCurLn();
        newc = buf;
        space = ED_MAXLINE;     /* amylaar */
        if (P_CURPTR == lastline)
            still_running = 0;
        if (regexec(pat, txtptr = gettxtl(P_CURPTR))) {
            do {
                /* Copy leading text */
                int diff = pat->startp[0] - txtptr;

                if ((space -= diff) < 0)        /* amylaar */
                    return SUB_FAIL;
                strncpy(newc, txtptr, diff);
                newc += diff;
                /* Do substitution */
                old = newc;
                newc = regsub(pat, sub, newc, space);
                if (!newc || (space -= newc - old) < 0)   /* amylaar */
                    return SUB_FAIL;
                if (txtptr == pat->endp[0]) {   /* amylaar : prevent infinite
                                                 * loop */
                    if (!*txtptr)
                        break;
                    if (--space < 0)
                        return SUB_FAIL;
                    *newc++ = *txtptr++;
                } else
                    txtptr = pat->endp[0];
            }
            while (gflg && !pat->reganch && regexec(pat, txtptr));

            /* Copy trailing chars */
            /*
             * amylaar : always check for enough space left BEFORE altering
             * memory
             */
            if ((space -= strlen(txtptr) + 1) < 0)
                return SUB_FAIL;
            strcpy(newc, txtptr);
            del(P_CURLN, P_CURLN);
            if (ins(buf) < 0)
                return MEM_FAIL;
            nchngd++;
            if (pflag)
                doprnt(P_CURLN, P_CURLN);
        }
    }
    return ((nchngd == 0 && !gflg) ? SUB_FAIL : nchngd);
}


/*
 * Indent code from DGD editor (v0.1), adapted.  No attempt has been made to
 * optimize for this editor.   Dworkin 920510
 */
#define error(s)               { ED_OUTPUTV(ED_DEST, s, lineno); errs++; return; }
#define bool char
static int lineno, errs;
static int shi;                 /* the current shift (negative for left
                                 * shift) */

/*
 * NAME:        shift(char*)
 * ACTION:      Shift a line left or right according to "shi".
 */
/* amylaar: don't use identifier index, this is reserved in SYS V compatible
 *              environments.
 */
static void shift (register char * text)
{
    register int indent_index;

    /* first determine the number of leading spaces */
    indent_index = 0;
    while (*text == ' ' || *text == '\t') {
        if (*text++ == ' ') {
            indent_index++;
        } else {
            // indent_index = (indent_index + 8) & ~7;
            indent_index = indent_index + ED_TAB_WIDTH -
                           (indent_index % ED_TAB_WIDTH);
        }
    }

    if (*text != '\0') {        /* don't shift lines with ws only */
        indent_index += shi;
        if (indent_index < ED_MAXLINE) {
            char buffer[ED_MAXLINE];
            register char *p;

            p = buffer;
            /* fill with leading ws */
#ifdef USE_TABS
            while (indent_index >= ED_TAB_WIDTH) {
                *p++ = '\t';
                indent_index -= ED_TAB_WIDTH;
            }
#endif
            while (indent_index > 0) {
                *p++ = ' ';
                --indent_index;
            }
            if (p - buffer + strlen(text) < ED_MAXLINE) {
                strcpy(p, text);
                del(lineno, lineno);
                ins(buffer);
                return;
            }
        }
        error("缩排后的结果太长，发生于第 %d 行\n");
    }
}

#define STACKSZ        1024     /* size of indent stack */

/* token definitions in indent */
#define SEMICOLON      0
#define LBRACKET       1
#define RBRACKET       2
#define LOPERATOR      3
#define ROPERATOR      4
#define LHOOK          5
#define LHOOK2         6
#define RHOOK          7
#define TOKEN          8
#define ELSE           9
#define IF             10
#define FOR            11
#define WHILE          12
#define XDO             13
#define XEOT           14

static char *stack, *stackbot;  /* token stack */
static int *ind, *indbot;       /* indent stack */
static char quote;              /* ' or " */
static bool in_ppcontrol, after_keyword_t, in_mblock;   /* status */
int in_comment;
static char last_term[ED_MAXLINE+5];
static int last_term_len;

/*
 * NAME:        indent(char*)
 * ACTION:      Parse and indent a line of text. This isn't perfect, as
 *              keyword_ts could be defined as macros, comments are very hard to
 *              handle properly, (, [ and ({ will match any of ), ] and }),
 *              and last but not least everyone has his own taste of
 *              indentation.
 */
static void indent (char * buf)
{
    static char f[] =
    {7, 1, 7, 1, 2, 1, 1, 6, 4, 2, 6, 7, 7, 2, 0,};
    static char g[] =
    {2, 2, 1, 7, 1, 5, 5, 1, 3, 6, 2, 2, 2, 2, 0,};
    char text[ED_MAXLINE], ident[ED_MAXLINE];
    char *p, *sp;
    register int *ip;
    register long indent_index;
    register int top, token;
    char *start;
    bool do_indent;

    /*
     * Problem: in this editor memory for deleted lines is reclaimed. So we
     * cannot shift the line and then continue processing it, as in DGD ed.
     * Instead make a copy of the line, and process the copy. Dworkin 920510
     */
    strcpy(text, buf);

    do_indent = FALSE;
    indent_index = 0;
    p = text;

    /* process status vars */
    if (quote != '\0') {
        shi = 0;                /* in case a comment starts on this line */
    } else if (in_ppcontrol || *p == '#') {
        while (*p != '\0') {
            if (*p == '\\' && *++p == '\0') {
                in_ppcontrol = TRUE;
                return;
            }
            p++;
        }
        in_ppcontrol = FALSE;
        return;
    } else if (in_mblock) {
        if (!strncmp(p, last_term, last_term_len)) {
            in_mblock = FALSE;
            p+=last_term_len;
        }
        else return;
    } else {
        /* count leading ws */
        while (*p == ' ' || *p == '\t') {
            if (*p++ == ' ') {
                indent_index++;
            } else {
                // indent_index = (indent_index + 8) & ~7;
                indent_index = indent_index + ED_TAB_WIDTH -
                               (indent_index % ED_TAB_WIDTH);
            }
        }
        if (*p == '\0') {
            del(lineno, lineno);
            ins(p);
            return;
        } else if (in_comment) {
            shift(text);        /* use previous shi */
        } else {
            do_indent = TRUE;
        }
    }

    /* process this line */
    start = p;
    while (*p != '\0') {

        /* lexical scanning: find the next token */
        ident[0] = '\0';
        if (in_comment) {
            /* comment */
            while (*p != '*') {
                if (*p == '\0') {
                    if (in_comment == 2) in_comment = 0;
                    return;
                }
                p++;
            }
            while (*p == '*') {
                p++;
            }
            if (*p == '/') {
                in_comment = 0;
                p++;
            }
            continue;

        } else if (quote != '\0') {
            /* string or character constant */
            for (;;) {
                if (*p == quote) {
                    quote = '\0';
                    p++;
                    break;
                } else if (*p == '\0') {
                    error("未结束的字串发现于第 %d 行\n");
                } else if (*p == '\\' && *++p == '\0') {
                    break;
                }
                p++;
            }
            token = TOKEN;

        } else {
            switch (*p++) {
            case ' ':           /* white space */
            case '\t':
                continue;

            case '\'':
            case '"':           /* start of string */
                quote = p[-1];
                continue;

            case '@': {
                int j = 0;

                if (*p == '@') p++;
                while (uisalnum(*p) || *p == '_') last_term[j++] = *p++;
                last_term[j] = '\0';
                last_term_len = j;
                in_mblock = TRUE;
                return;
            }
            case '/':
                if (*p == '*' || *p == '/') {   /* start of a comment    */
                    in_comment = (*p == '*') ? 1 : 2;
                    if (do_indent) {
                        /* this line hasn't been indented yet */
                        shi = *ind - indent_index;
                        shift(text);
                        do_indent = FALSE;
                    } else {
                        register char *q;
                        register int index2;

                        /*
                         * find how much the comment has shifted, so the same
                         * shift can be used if the coment continues on the
                         * next line
                         */
                        index2 = *ind;
                        for (q = start; q < p - 1;) {
                            if (*q++ == '\t') {
                                // indent_index = (indent_index + 8) & ~7;
                                // index2 = (index2 + 8) & ~7;
                                indent_index = indent_index + ED_TAB_WIDTH -
                                               (indent_index % ED_TAB_WIDTH);
                                index2 = index2 + ED_TAB_WIDTH -
                                         (index2 % ED_TAB_WIDTH);


                            } else {
                                indent_index++;
                                index2++;
                            }
                        }
                        shi = index2 - indent_index;
                    }
                    p++;

                    if((*p == '\0') && (in_comment == 2))
                      in_comment = 0;

                    continue;
                }
                token = TOKEN;
                break;

            case '{':
                token = LBRACKET;
                break;

            case '(':
                if (after_keyword_t) {
                    /*
                     * LOPERATOR & ROPERATOR are a kludge. The operator
                     * precedence parser that is used could not work if
                     * parenthesis after keyword_ts was not treated specially.
                     */
                    token = LOPERATOR;
                    break;
                }
                if (*p == '{' || *p == '[' || (*p == ':' && p[1] != ':')) {
                    p++;        /* ({ , ([ , (: each are one token */
                    token = LHOOK2;
                    break;
                }
            case '[':
                token = LHOOK;
                break;

            case ':':
                if (*p == ')') {
                    p++;
                    token = RHOOK;
                    break;
                }
                token = TOKEN;
                break;
            case '}':
                if (*p != ')') {
                    token = RBRACKET;
                    break;
                }
                /* }) is one token */
                p++;
                token = RHOOK;
                break;
            case ']':
                if (*p == ')' &&
                    (*stack == LHOOK2 || (*stack != XEOT &&
                                          (stack[1] == LHOOK2 ||
                         (stack[1] == ROPERATOR && stack[2] == LHOOK2))))) {
                    p++;
                }
            case ')':
                token = RHOOK;
                break;

            case ';':
                token = SEMICOLON;
                break;

            default:
                if (uisalpha(*--p) || *p == '_') {
                    char *q;

                    /* Identifier. See if it's a keyword_t. */
                    q = ident;
                    do {
                        *q++ = *p++;
                    } while (uisalnum(*p) || *p == '_');
                    *q = '\0';

                    if (strcmp(ident, "if") == 0)
                        token = IF;
                    else if (strcmp(ident, "else") == 0)
                        token = ELSE;
                    else if (strcmp(ident, "for") == 0)
                        token = FOR;
                    else if (strcmp(ident, "foreach") == 0)
                        token = FOR;
                    else if (strcmp(ident, "while") == 0)
                        token = WHILE;
                    else if (strcmp(ident, "do") == 0)
                        token = XDO;
                    else        /* not a keyword_t */
                        token = TOKEN;
                } else {
                    /* anything else is a "token" */
                    p++;
                    token = TOKEN;
                }
                break;
            }
        }

        /* parse */
        sp = stack;
        ip = ind;
        for (;;) {
            top = *sp;
            if (top == LOPERATOR && token == RHOOK) {
                /* ) after LOPERATOR is ROPERATOR */
                token = ROPERATOR;
            }
            if (f[top] <= g[token]) {   /* shift the token on the stack */
                register int i;

                if (sp == stackbot) {
                    /* out of stack */
                    error("巢状缩排过深于第 %d 行。\n");
                }
                /* handle indentation */
                i = *ip;
                /* if needed, reduce indentation prior to shift */
                if ((token == LBRACKET &&
                     (*sp == ROPERATOR || *sp == ELSE || *sp == XDO)) ||
                    token == RBRACKET ||
                    (token == IF && *sp == ELSE)) {
                    /* back up */
                    i -= P_SHIFTWIDTH;
                } else if (token == RHOOK || token == ROPERATOR) {
                    i -= P_SHIFTWIDTH / 2;
                }
                /* shift the current line, if appropriate */
                if (do_indent) {
                    shi = i - indent_index;
                    if (token == TOKEN && *sp == LBRACKET &&
                        (strcmp(ident, "case") == 0 ||
                         strcmp(ident, "default") == 0)) {
                        /* back up if this is a switch label */
#ifndef ED_INDENT_CASE
                        shi -= P_SHIFTWIDTH;
#endif
                    }
                    shift(text);
                    do_indent = FALSE;
                }
                /* change indentation after current token */
                switch (token) {
                case LBRACKET:
                case ROPERATOR:
                case ELSE:
                case XDO:
                    {
                        /* add indentation */
                        i += P_SHIFTWIDTH;
                        break;
                    }
                case LOPERATOR:
                case LHOOK:
                case LHOOK2: /* Is this right? */
                    {
                        /* half indent after ( [ ({ ([ */
                        i += P_SHIFTWIDTH / 2;
                        break;
                    }
                case SEMICOLON:
                    {
                        /* in case it is followed by a comment */
                        if (*sp == ROPERATOR || *sp == ELSE) {
                            i -= P_SHIFTWIDTH;
                        }
                        break;
                    }
                }

                *--sp = token;
                *--ip = i;
                break;
            }
            /* reduce handle */
            do {
                top = *sp++;
                ip++;
            } while (f[(int) *sp] >= g[top]);
        }
        stack = sp;
        ind = ip;
        after_keyword_t = (token >= IF);        /* but not after ELSE */
    }
}

static int indent_code()
{
    char s[STACKSZ];
    int i[STACKSZ];
    int last;

    /* setup stacks */
    stackbot = s;
    indbot = i;
    stack = stackbot + STACKSZ - 1;
    *stack = XEOT;
    ind = indbot + STACKSZ - 1;
    *ind = 0;

    quote = '\0';
    in_ppcontrol = FALSE;
    in_comment = 0;
    in_mblock = 0;

    P_FCHANGED = TRUE;
    last = P_LASTLN;
    errs = 0;

    for (lineno = 1; lineno <= last; lineno++) {
        setCurLn(lineno);
        indent(gettxtl(P_CURPTR));
        if (errs != 0) {
            return ERR;
        }
    }

    return 0;
}

#undef bool
#undef error
/* end of indent code */


/*  docmd.c
 *      Perform the command specified in the input buffer, as pointed to
 *      by inptr.  Actually, this finds the command letter first.
 */

static int docmd (int glob)
{
    static char rhs[MAXPAT];
    regexp *subpat;
    int c, err, line3;
    int apflg, pflag, gflag;
    int nchng;
    char *fptr;
    int st;
    int scr;

    pflag = FALSE;
    Skip_White_Space;

    c = *inptr++;
    switch (c) {
    case NL:
        if (P_NLINES == 0 && (P_LINE2 = nextln(P_CURLN)) == 0)
            return END_OF_FILE;
        if (P_NLINES == 2) {
            if (P_LINE1 > P_LINE2 || P_LINE1 <= 0)
                return BAD_LINE_RANGE;
            if ((st = doprnt(P_LINE1, P_LINE2)))
                return st;
            return 0;
        }
        setCurLn(P_LINE2);
        return (1);

    case '=':
        ED_OUTPUTV(ED_DEST, "%d\n", P_LINE2);
        break;

    case 'o':
    case 'a':
    case 'A':
        P_CUR_AUTOIND = c == 'A' ? !P_AUTOINDFLG : P_AUTOINDFLG;
        if (*inptr != NL)
            return SYNTAX_ERROR;
        if (P_NLINES > 1)
            return RANGE_ILLEGAL;

        if (P_CUR_AUTOIND)
            count_blanks(P_LINE1);
        if ((st = append(P_LINE1, glob)) < 0)
            return st;
        P_FCHANGED = TRUE;
        break;

    case 'c':
        if (*inptr != NL)
            return SYNTAX_ERROR;

        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;

        P_CUR_AUTOIND = P_AUTOINDFLG;
        if (P_AUTOINDFLG)
            count_blanks(P_LINE1);
        if ((st = del(P_LINE1, P_LINE2)) < 0)
            return st;
        if ((st = append(P_CURLN, glob)) < 0)
            return st;
        P_FCHANGED = TRUE;
        break;

    case 'd':
        if (*inptr != NL)
            return SYNTAX_ERROR;

        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;

        if ((st = del(P_LINE1, P_LINE2)) < 0)
            return st;
        if (nextln(P_CURLN) != 0)
            nextCurLn();
        if (P_DPRINT)
            doprnt(P_CURLN, P_CURLN);
        P_FCHANGED = TRUE;
        break;

    case 'e':
        if (P_RESTRICT)
            return IS_RESTRICTED;
        if (P_NLINES > 0)
            return LINE_OR_RANGE_ILL;
        if (P_FCHANGED)
            return CHANGED;
        /* FALL THROUGH */
    case 'E':
        if (P_RESTRICT)
            return IS_RESTRICTED;
        if (P_NLINES > 0)
            return LINE_OR_RANGE_ILL;

        if (*inptr != ' ' && *inptr != HT && *inptr != NL)
            return SYNTAX_ERROR;

        if ((fptr = getfn(0)) == NULL)
            return FILE_NAME_ERROR;

        clrbuf();
        (void) doread(0, fptr);

        strcpy(P_FNAME, fptr);
        P_FCHANGED = FALSE;
        break;

    case 'f':
        if (P_RESTRICT)
            return IS_RESTRICTED;
        if (P_NLINES > 0)
            return LINE_OR_RANGE_ILL;

        if (*inptr != ' ' && *inptr != HT && *inptr != NL)
            return SYNTAX_ERROR;

        fptr = getfn(0);

        if (P_NOFNAME)
            ED_OUTPUTV(ED_DEST, "/%s\n", P_FNAME);
        else {
            if (fptr == NULL)
                return FILE_NAME_ERROR;
            strcpy(P_FNAME, fptr);
        }
        break;

    case 'O':
    case 'i':
        if (*inptr != NL)
            return SYNTAX_ERROR;
        if (P_NLINES > 1)
            return RANGE_ILLEGAL;

        P_CUR_AUTOIND = P_AUTOINDFLG;
        if (P_AUTOINDFLG)
            count_blanks(P_LINE1);
        if ((st = append(prevln(P_LINE1), glob)) < 0)
            return st;
        P_FCHANGED = TRUE;
        break;

    case 'j':
        if (*inptr != NL || deflt(P_CURLN, P_CURLN + 1) < 0)
            return SYNTAX_ERROR;

        if ((st = join(P_LINE1, P_LINE2)) < 0)
            return st;
        break;

    case 'k':
        Skip_White_Space;

        if (*inptr < 'a' || *inptr > 'z')
            return MARK_A_TO_Z;
        c = *inptr++;

        if (*inptr != ' ' && *inptr != HT && *inptr != NL)
            return SYNTAX_ERROR;

        P_MARK[c - 'a'] = P_LINE1;
        break;

    case 'l':
        if (*inptr != NL)
            return SYNTAX_ERROR;
        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;
        if ((st = dolst(P_LINE1, P_LINE2)) < 0)
            return st;
        break;

    case 'm':
        if ((line3 = getone()) < 0)
            return line3;
        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;
        if ((st = move(line3)) < 0)
            return st;
        P_FCHANGED = TRUE;
        break;

    case 'n':
        if (*inptr != NL)
            return SYNTAX_ERROR;
        if (P_NFLG)
            P_FLAGS &= ~NFLG_MASK;
        else
            P_FLAGS |= NFLG_MASK;
        ED_OUTPUTV(ED_DEST, "行号模式 %s，特殊字元模式 %s\n",
                    P_NFLG ? "开" : "关", P_LFLG ? "开" : "关");
        break;

    case 'I':
        if (P_NLINES > 0)
            return LINE_OR_RANGE_ILL;
        if (*inptr != NL)
            return SYNTAX_ERROR;
        ED_OUTPUT(ED_DEST, "开始进行缩排(Indentation)处理¨¨\n");
        if (indent_code())
            ED_OUTPUT(ED_DEST, "停止缩排，可能有错误发生。\n");
        else
            ED_OUTPUT(ED_DEST, "缩排完毕。\n");
        break;

    case 'H':
    case 'h':
        print_help(*(inptr++));
        break;

    case 'P':
    case 'p':
        if (*inptr != NL)
            return SYNTAX_ERROR;
        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;
        if ((st = doprnt(P_LINE1, P_LINE2)) < 0)
            return st;
        break;

    case 'q':
        if (P_FCHANGED)
            return CHANGED;
        /* FALL THROUGH */
    case 'Q':
        if (*inptr != NL)
            return SYNTAX_ERROR;
        if (glob)
            return SYNTAX_ERROR;
        if (P_NLINES > 0)
            return LINE_OR_RANGE_ILL;
        clrbuf();
        return (EOF);

    case 'r':
        if (P_RESTRICT)
            return IS_RESTRICTED;
        if (P_NLINES > 1)
            return RANGE_ILLEGAL;

        if (P_NLINES == 0)      /* The original code tested */
            P_LINE2 = P_LASTLN; /* if(P_NLINES = 0)    */
        /* which looks wrong.  RAM  */

        if (*inptr != ' ' && *inptr != HT && *inptr != NL)
            return SYNTAX_ERROR;

        if ((fptr = getfn(0)) == NULL)
            return FILE_NAME_ERROR;

        if ((err = doread(P_LINE2, fptr)) < 0)
            return (err);
        P_FCHANGED = TRUE;
        break;

    case 's':
        if (*inptr == 'e')
            return (set());
        Skip_White_Space;
        if ((subpat = optpat()) == NULL)
            return SUB_BAD_PATTERN;
        if ((gflag = getrhs(rhs)) < 0)
            return SUB_BAD_REPLACEMENT;
        if (*inptr == 'p')
            pflag++;
        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;
        if ((nchng = subst(subpat, rhs, gflag, pflag)) < 0)
            return nchng;
        if (nchng)
            P_FCHANGED = TRUE;
        if (nchng == 1 && P_PFLG) {
            if ((st = doprnt(P_CURLN, P_CURLN)) < 0)
                return st;
        }
        break;

    case 't':
        if ((line3 = getone()) < 0)
            return BAD_DESTINATION;
        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;
        if ((st = transfer(line3)) < 0)
            return st;
        P_FCHANGED = TRUE;
        break;

    case 'W':
    case 'w':
        apflg = (c == 'W');

        if (*inptr != NL && P_RESTRICT)
            return IS_RESTRICTED;
        if (*inptr != ' ' && *inptr != HT && *inptr != NL)
            return SYNTAX_ERROR;

        if ((fptr = getfn(1)) == NULL)
            return FILE_NAME_ERROR;

        if (deflt(1, P_LASTLN) < 0)
            return BAD_LINE_RANGE;
        if ((st = dowrite(P_LINE1, P_LINE2, fptr, apflg)) < 0)
            return st;
        P_FCHANGED = FALSE;
        break;

    case 'x':
        if (*inptr == NL && P_NLINES == 0 && !glob) {
            if ((fptr = getfn(1)) == NULL)
                return FILE_NAME_ERROR;
            if (dowrite(1, P_LASTLN, fptr, 0) >= 0) {
                clrbuf();
                return (EOF);
            }
        }
        if (P_NLINES)
            return LINE_OR_RANGE_ILL;
        return SYNTAX_ERROR;

    case 'z':
        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;

        switch (*inptr) {
        case '-':
            scr = 0;

            while(*inptr == '-') {
              scr += ED_BUFFER->scroll_lines - 2;
              inptr++;
            }

            if(scr >= P_LINE1)
              scr = P_LINE1 - 1;

            if ((st = doprnt(P_LINE1 - (scr),
                             P_LINE1 + ED_BUFFER->scroll_lines - (scr+2))) < 0)
                return st;
            break;

        case '.':
            if ((st = doprnt(P_LINE1 - (ED_BUFFER->scroll_lines/2),
                             P_LINE1 + (ED_BUFFER->scroll_lines/2-1))) < 0)
                return st;
            break;

        case '+':
        case '\n':
            scr = 0;
            while(*(++inptr) == '+')
              scr += ED_BUFFER->scroll_lines - 2;
            if ((st = doprnt(P_LINE1 + (scr),
                P_LINE1 + ED_BUFFER->scroll_lines + (scr) - 2)) < 0)
                return st;
            break;
        }
        break;

    case 'Z':
        if (deflt(P_CURLN, P_CURLN) < 0)
            return BAD_LINE_RANGE;

        switch (*inptr) {
        case '-':
            scr = 0;

            while(*inptr == '-') {
              scr += ED_BUFFER->scroll_lines+3;
              inptr++;
            }

            if(scr >= P_LINE1)
              scr = P_LINE1 - 1;

            if ((st = doprnt(P_LINE1 - (scr),
                             P_LINE1 + ED_BUFFER->scroll_lines - (scr-3))) < 0)

                return st;
            break;

        case '.':
            if ((st = doprnt(P_LINE1 - (ED_BUFFER->scroll_lines/2+2),
                             P_LINE1 + (ED_BUFFER->scroll_lines/2+2))) < 0)
                return st;
            break;

        case '+':
        case '\n':
            scr = 0;
            while(*(++inptr) == '+')
              scr += ED_BUFFER->scroll_lines+3;
            if ((st = doprnt(P_LINE1 + (scr),
                P_LINE1 + ED_BUFFER->scroll_lines + (scr) + 3)) < 0)
                return st;
            break;
        }
        break;
    default:
        return (UNRECOG_COMMAND);
    }

    return (0);
}                               /* docmd */

/*      doglob.c        */
static int doglob()
{
    int lin, status = 0;
    char *cmd;
    ed_line_t *ptr;

    cmd = inptr;

    for (;;) {
        ptr = getptr(1);
        for (lin = 1; lin <= P_LASTLN; lin++) {
            if (ptr->l_stat & LGLOB)
                break;
            ptr = getnextptr(ptr);
        }
        if (lin > P_LASTLN)
            break;

        ptr->l_stat &= ~LGLOB;
        P_CURLN = lin;
        P_CURPTR = ptr;
        inptr = cmd;
        if ((status = getlst()) < 0 && status != NO_LINE_RANGE)
            return (status);
        if ((status = docmd(1)) < 0)
            return (status);
    }
    return (P_CURLN);
}                               /* doglob */


/*
 * Start the editor. Because several users can edit simultaneously,
 * they will each need a separate editor data block.
 *
 * If a write_fn and exit_ob is given, then call exit_ob->write_fn after
 * any time the editor contents are written out to a file. The purpose is
 * make it possible for external LPC code to do more admin logging of
 * files modified.
 *
 * If an exit_fn and exit_ob is given, then call exit_ob->exit_fn at
 * exit of editor. The purpose is to make it possible for external LPC
 * code to maintain a list of locked files.
 */
#ifdef OLD_ED
void ed_start (const char * file_arg, const char * write_fn,
               const char * exit_fn, int restricted, object_t * exit_ob,
               int scroll_lines)
{
    svalue_t *setup;

    regexp_user = ED_REGEXP;
    if (!command_giver)
        error("No current user for ed().\n");
    if (!command_giver->interactive)
        error("Tried to start an ed session on a non-interactive user.\n");
    if (command_giver->interactive->ed_buffer)
        error("Tried to start an ed session, when already active.\n");

    current_ed_buffer = command_giver->interactive->ed_buffer =
        ALLOCATE(ed_buffer_t, TAG_ED, "ed_start: ED_BUFFER");
    memset((char *) ED_BUFFER, '\0', sizeof(ed_buffer_t));

    current_editor = command_giver;

    ED_BUFFER->flags |= EIGHTBIT_MASK;
    ED_BUFFER->shiftwidth = ED_INDENT_SPACES;
    push_object(current_editor);
    setup = apply_master_ob(APPLY_RETRIEVE_ED_SETUP, 1);
    if (setup && setup != (svalue_t *)-1 &&
        setup->type == T_NUMBER && setup->u.number) {
        ED_BUFFER->flags = setup->u.number & ALL_FLAGS_MASK;
        ED_BUFFER->shiftwidth = setup->u.number & SHIFTWIDTH_MASK;
    }
    ED_BUFFER->CurPtr = &ED_BUFFER->Line0;

#if defined(RESTRICTED_ED) && !defined(NO_WIZARDS)
    if (current_editor->flags & O_IS_WIZARD) {
        P_RESTRICT = 0;
    } else {
        P_RESTRICT = 1;
    }
#endif                          /* RESTRICTED_ED */
    if (restricted) {
        P_RESTRICT = 1;
    }
    if (write_fn) {
        ED_BUFFER->write_fn = alloc_cstring(write_fn, "ed_start");
        exit_ob->ref++;
    } else {
        ED_BUFFER->write_fn = 0;
    }
    if (exit_fn) {
        ED_BUFFER->exit_fn = alloc_cstring(exit_fn, "ed_start");
        exit_ob->ref++;
    } else {
        ED_BUFFER->exit_fn = 0;
    }
    ED_BUFFER->exit_ob = exit_ob;
    if(scroll_lines)
        ED_BUFFER->scroll_lines = scroll_lines;
    else
        ED_BUFFER->scroll_lines = 22;

    set_ed_buf();

    /*
     * Check for read on startup, since the buffer is read in. But don't
     * check for write, since we may want to change the file name.
     */
    if (file_arg
        && (file_arg =
            check_valid_path(file_arg, current_editor,
                             "ed_start", 0))
        && !doread(0, file_arg)) {
        setCurLn(1);
    }
    if (file_arg) {
        strncpy(P_FNAME, file_arg, MAXFNAME - 1);
        P_FNAME[MAXFNAME - 1] = 0;
    } else {
        ED_OUTPUT(ED_DEST, "<没有指定档案>\n");
    }
    set_prompt(":");
    return;
}
#endif

#ifdef OLD_ED
void ed_cmd (char * str)
{
    int status = 0;

    regexp_user = ED_REGEXP;
    current_ed_buffer = command_giver->interactive->ed_buffer;
    current_editor = command_giver;

    if (P_MORE) {
        print_help2();
        return;
    }
    if (P_APPENDING) {
        more_append(str);
        return;
    }

    strncpy(inlin, str, ED_MAXLINE - 2);
    inlin[ED_MAXLINE - 2] = 0;
    strcat(inlin, "\n");

    inptr = inlin;
    if ((status = getlst()) >= 0 || status == NO_LINE_RANGE) {
        if ((status = ckglob()) != 0) {
            if (status >= 0 && (status = doglob()) >= 0) {
                setCurLn(status);
                return;
            }
        } else {
            if ((status = docmd(0)) >= 0) {
                if (status == 1)
                    doprnt(P_CURLN, P_CURLN);
                return;
            }
        }
    }
    report_status(status);
}
#endif

static void report_status (int status) {
    switch (status) {
    case EOF:
        ED_OUTPUT(ED_DEST, "<离开 ed>\n");
        free_ed_buffer(current_editor);
        return;
    case CHANGED:
        ED_OUTPUT(ED_DEST, "<档案已更变>\n");
        break;
    case SET_FAIL:
        ED_OUTPUT(ED_DEST, "<set 指令失败>\n");
        break;
    case SUB_FAIL:
        ED_OUTPUT(ED_DEST, "<取代字串失败>\n");
        break;
    case MEM_FAIL:
        ED_OUTPUT(ED_DEST, "<记忆体不足：资料可能流失>\n");
        break;
    case UNRECOG_COMMAND:
        ED_OUTPUT(ED_DEST, "<没有这个指令>\n");
        break;
    case BAD_LINE_RANGE:
        ED_OUTPUT(ED_DEST, "<不正确的范围>\n");
        break;
    case BAD_LINE_NUMBER:
        ED_OUTPUT(ED_DEST, "<不正确的行数>\n");
        break;
    case SYNTAX_ERROR:
        ED_OUTPUT(ED_DEST, "<不正确的指令格式>\n");
        break;
    case RANGE_ILLEGAL:
        ED_OUTPUT(ED_DEST, "<此指令无法使用范围>\n");
        break;
    case IS_RESTRICTED:
        ED_OUTPUT(ED_DEST, "<你无法使用此指令>\n");
        break;
    case LINE_OR_RANGE_ILL:
        ED_OUTPUT(ED_DEST, "<此指令无法使用行数或范围>\n");
        break;
    case FILE_NAME_ERROR:
        ED_OUTPUT(ED_DEST, "<'file' 指令错误>\n");
        break;
    case MARK_A_TO_Z:
        ED_OUTPUT(ED_DEST, "<标记范围必须由 'a 到 'z>\n");
        break;
    case SUB_BAD_PATTERN:
        ED_OUTPUT(ED_DEST, "<不正确的搜寻样式>\n");
        break;
    case SUB_BAD_REPLACEMENT:
        ED_OUTPUT(ED_DEST, "<不正确的取代样式>。\n");
        break;
    case BAD_DESTINATION:
        ED_OUTPUT(ED_DEST, "<移动或复制的目地错误>\n");
        break;
    case END_OF_FILE:
        ED_OUTPUT(ED_DEST, "<档尾>\n");
        break;
    case SEARCH_FAILED:
        ED_OUTPUT(ED_DEST, "<搜寻失败>\n");
        break;
    default:
        ED_OUTPUT(ED_DEST, "<不正确的指令>\n");
    }
}

#ifdef OLD_ED
void save_ed_buffer (object_t * who)
{
    svalue_t *stmp;
    const char *fname;

    regexp_user = ED_REGEXP;
    current_ed_buffer = who->interactive->ed_buffer;
    current_editor = who;

    push_malloced_string(add_slash(P_FNAME));
    push_object(who);
    /* must be safe; we get called by remove_interactive() */
    stmp = safe_apply_master_ob(APPLY_GET_ED_BUFFER_SAVE_FILE_NAME, 2);
    if (stmp && stmp != (svalue_t *)-1) {
        if (stmp->type == T_STRING) {
            fname = stmp->u.string;
            if (*fname == '/')
                fname++;
            dowrite(1, P_LASTLN, fname, 0);
        }
    }
    free_ed_buffer(who);
}
#endif

static void print_help (int arg)
{
    char edout[8192];
    char *brkpt;
//    char *pos = edout;
    char *outstr;
//    int n;
    struct strlst *curp;

    switch (arg) {
    case 'I':
        ED_OUTPUT(ED_DEST, "                    自动缩排 (V 1.0)\n");
        ED_OUTPUT(ED_DEST, "------------------------------------\n");
        ED_OUTPUT(ED_DEST, "                Qixx [更新: 7/10/91]\n");
        ED_OUTPUT(ED_DEST, "指令格式: I\n\n");
        ED_OUTPUT(ED_DEST, "当使用这个指令时候，本编辑程式会自动帮你将程式码做缩排的动\n");
        ED_OUTPUT(ED_DEST, "作，让程式码变的更容易阅\读和寻找错误。并且同时寻找一些像是\n");
        ED_OUTPUT(ED_DEST, "未中断的字串 (Unterminated String)、不配对的 {} 和 () 之类\n");
        ED_OUTPUT(ED_DEST, "的基本错误 (如果没有配对的话，缩排后看起来就有问题了) 。如\n");
        ED_OUTPUT(ED_DEST, "果有任何未妥善缩排的程式码，请将之寄到 gaunt@mcs.anl.gov。\n");
        break;
    case 'n':
        ED_OUTPUT(ED_DEST, "指令格式: n\n\n");
        ED_OUTPUT(ED_DEST, "这个指令会将内建的旗标设定成显示行数的模式。\n");
        break;
    case 'a':
        ED_OUTPUT(ED_DEST, "指令格式: [行号]a 或 a\n\n");
        ED_OUTPUT(ED_DEST, "这个指令另你由指定某行之后开始新增 (append) 文字，若不指定\n");
        ED_OUTPUT(ED_DEST, "则为目前所在的行数。在空白行打入 '.' 来结束本输入模式。\n");
        break;
    case 'A':
        ED_OUTPUT(ED_DEST, "指令格式: [行数]A 或 A\n\n");
        ED_OUTPUT(ED_DEST, "如同 'a' 指令，但使用反向自动缩排模式 (inverse autoindent mode)。\n");
        ED_OUTPUT(ED_DEST, "建议与 'a' 指定比较。\n");
        break;
    case 'i':
        ED_OUTPUT(ED_DEST, "指令格式: [行数]i 或 i\n\n");
        ED_OUTPUT(ED_DEST, "这个指令另你由指定某行之前开始插入 (insert) 文字，若不指定\n");
        ED_OUTPUT(ED_DEST, "则为目前所在的行数。在空白行打入 '.' 来结束本输入模式。\n");
        break;
    case 'c':
        ED_OUTPUT(ED_DEST, "指令格式: [范围]c 或 c\n\n");
        ED_OUTPUT(ED_DEST, "此指令将范围内的行数取代为新输入的文字，若是不指定范围则为\n");
        ED_OUTPUT(ED_DEST, "目前所在该行。在空白行打入 '.' 来结束本输入模式。\n");
        break;
    case 'd':
        ED_OUTPUT(ED_DEST, "指令格式: [范围]d 或 d\n\n");
        ED_OUTPUT(ED_DEST, "此指令将范围内的行数删除，若是不指定范围则为目前所在该行。\n");
        break;
#ifndef RESTRICTED_ED
    case 'e':
        ED_OUTPUT(ED_DEST, "指令格式: e [档案名称]\n\n");
        ED_OUTPUT(ED_DEST, "此指令将编辑中的档案由记忆体中清除，并重新读入该档案。\n");
        break;
    case 'E':
        ED_OUTPUT(ED_DEST, "指令格式: E [档案名称]\n\n");
        ED_OUTPUT(ED_DEST, "此指令与 'e' 相同，但会忽略尚未储存的修改。\n");
        break;
    case 'f':
        ED_OUTPUT(ED_DEST, "指令格式: f 或 f [档案]\n\n");
        ED_OUTPUT(ED_DEST, "显示目前编辑中的档案名称。若后面有指定档名，则将会把档名更\n");
        ED_OUTPUT(ED_DEST, "改为指定的新档案。\n");
        break;
#endif                          /* RESTRICTED_ED mode */
    case 'g':
        ED_OUTPUT(ED_DEST, "指令格式: [范围] g/[字串]/p 或 g/[字串]/p\n\n");
        ED_OUTPUT(ED_DEST, "在范围内寻找所有包含 [字串] 的行数，并同时列出那几行。如果\n");
        ED_OUTPUT(ED_DEST, "不指定范围则会搜寻整个档案。\n");
        ED_OUTPUT(ED_DEST, "建议与 'v' 指令比较。\n");
        break;
    case 'h':
        ED_OUTPUT(ED_DEST, "指令格式: h[指令] 或 h\n\n");
        ED_OUTPUT(ED_DEST, "显示 [指令] 的详细说明或者所有指令的简略说明。\n");
        ED_OUTPUT(ED_DEST, "本指令由 Qixx 新增。\n");
        break;
    case 'j':
        ED_OUTPUT(ED_DEST, "指令格式: [范围]j 或 j\n\n");
        ED_OUTPUT(ED_DEST, "将范围内所有换行字元移除结合成一行。如果没有指定范围则范围\n");
        ED_OUTPUT(ED_DEST, "为目前所在之行与下一行。如果范围只有一行的话，本指令则无任\n");
        ED_OUTPUT(ED_DEST, "何作用。\n");
        break;
    case 'k':
        ED_OUTPUT(ED_DEST, "指令格式: [行数]k[字母] 或 k[字母]\n\n");
        ED_OUTPUT(ED_DEST, "将指定 [行数] 标记成 [字母] ，如果不指定行数则为目前所在该\n");
        ED_OUTPUT(ED_DEST, "行。输入 '[字母] 则会列出刚刚标记过的那行。被标记过的行数\n");
        ED_OUTPUT(ED_DEST, "并不会被修改。\n");
        break;
    case 'l':
        ED_OUTPUT(ED_DEST, "指令格式: [范围]l 或 l\n\n");
        ED_OUTPUT(ED_DEST, "将范围内的那几行列出来，并且将无法显示的字元 (如换行字元) \n");
        ED_OUTPUT(ED_DEST, "用符号来表示。如果不指定范围则为目前所在该行。\n");
        break;
    case 'm':
        ED_OUTPUT(ED_DEST, "指令格式: [范围]m[行数] 或 m[行数]\n");
        ED_OUTPUT(ED_DEST, "此指令将范围内的那几行移到第 [行数] 的后面，如果不指定范围\n");
        ED_OUTPUT(ED_DEST, "则是指目前所在该行。[行数 0] 则会移到档案的最上面。\n");
        break;
    case 'p':
        ED_OUTPUT(ED_DEST, "指令格式: [范围]p 或 p\n\n");
        ED_OUTPUT(ED_DEST, "此指令将范围内的那几行列印 (print) 到荧幕上，不指定范围则\n");
        ED_OUTPUT(ED_DEST, "为目前所在该行。\n");
        break;
    case 'q':
        ED_OUTPUT(ED_DEST, "指令格式: q\n\n");
        ED_OUTPUT(ED_DEST, "离开本编辑器。如果档案有被更动过则无法离开，参阅\指令 'w'\n");
        ED_OUTPUT(ED_DEST, "来将更变写入档案。\n");
        break;
    case 'Q':
        ED_OUTPUT(ED_DEST, "指令格式: Q\n\n");
        ED_OUTPUT(ED_DEST, "强制离开本编辑器，忽略任何的未储存变动。\n");
        break;
#ifndef RESTRICTED_ED
    case 'r':
        ED_OUTPUT(ED_DEST, "指令格式: [行数]r [档案] 或 r [档案]\n\n");
        ED_OUTPUT(ED_DEST, "读入 [档案] 并插入 [行数] 之后，如果不指定行数的话则为目前\n");
        ED_OUTPUT(ED_DEST, "所在之该行。\n");
        break;
#endif                          /* RESTRICTED_ED */
    case 't':
        ED_OUTPUT(ED_DEST, "指令格式: [范围]t[行数] 或 t[行数]\n\n");
        ED_OUTPUT(ED_DEST, "此指令用法与 m 相同，其不同为此指令并非移动指定范围，而是\n");
        ED_OUTPUT(ED_DEST, "将指定范围 copy 并插入 [行数] 之后，原本 [范围] 内则不变。\n");
        ED_OUTPUT(ED_DEST, "建议参考 'm' 指令。\n");
        break;
    case 'v':
        ED_OUTPUT(ED_DEST, "指令格式: [范围]v/[字串]/p 或 v/[字串]/p\n\n");
        ED_OUTPUT(ED_DEST, "在范围内搜寻所有不包含 [字串] 的行数，并同时列出那几行。如\n");
        ED_OUTPUT(ED_DEST, "果不指定范围则搜寻整个档案。\n");
        ED_OUTPUT(ED_DEST, "建议与 'g' 指令比较。\n");
        break;
    case 'z':
        ED_OUTPUT(ED_DEST, "指令格式: z 或 z- 或 z.\n\n");
        ED_OUTPUT(ED_DEST, "'z' 显示由目前开始的下面 20 行 (并将目前所在行数 + 20)。\n");
        ED_OUTPUT(ED_DEST, "'z-' 显示由目前开始的上面 20 行 (并不会更动目前所在行数)。\n");
        ED_OUTPUT(ED_DEST, "'z.' 显示由目前开始的上 10 行到下 10 行 (目前所在行数 + 10)。\n");
        break;
    case 'Z':
        ED_OUTPUT(ED_DEST, "指令格式: Z 或 Z- 或 Z.\n\n");
        ED_OUTPUT(ED_DEST, "用法同 'z'，但范围为其 2 倍。\n");
        break;
    case 'x':
        ED_OUTPUT(ED_DEST, "指令格式: x\n\n");
        ED_OUTPUT(ED_DEST, "储存编辑中档案并离开本编辑程式。\n");
        break;
    case 's':
        if (*inptr == 'e' && *(inptr + 1) == 't') {
            ED_OUTPUT(ED_DEST, "指令格式: set [选项] 或 set no[选项] 或 set。\n\
\n\
如果没有加入 [选项] 则显示目前的设定。如果用 no[选项] 则会关闭该功\能，\n\
set save 则会储存目前的设定。\n\
选项如下:\n\
\n\
number                  显示行数 (参考指令 n)。\n\
list                    显示特殊字元 (参考指令 l)。\n\
print                   在替换 (subsitution) 后会显示目前所在该行。\n\
eightbit                << 无说明 >>\n\
autoindent              会保持原来该行的缩排。用 ^D 或 ^K 来向右移一格。\n\
excompatible            会将 \\( \\) 的意思与 ( ) 互换。\n\
dprint                  在删除行数之后显示目前所在该行。\n\
shiftwidth [数字]       设定在自动排版时候的空格数目。\n");

        }
        break;
    case '/':
        ED_OUTPUT(ED_DEST, "指令格式: /[样式]\n\n");
        ED_OUTPUT(ED_DEST, "寻找下一个符合 [样式] 的行数。直得注意的是 [样式] 在此使用\n");
        ED_OUTPUT(ED_DEST, "正规表示 (regular expression) 语法，所以像是 .*][() 这种的\n");
        ED_OUTPUT(ED_DEST, "样式前面必须加上 \\.\n");
        break;
    case '?':
        ED_OUTPUT(ED_DEST, "指令格式: ?[样式]\n\n");
        ED_OUTPUT(ED_DEST, "寻找最后一个符合 [样式] 的行数。\n");
        ED_OUTPUT(ED_DEST, "建议参考 '/' 指令。\n");
        break;
        /* someone should REALLY write help for s in the near future */
#ifndef RESTRICTED_ED
    case 'w':
    case 'W':
        ED_OUTPUT(ED_DEST, "指令格式: W [档案] 或 W\n\n");
        ED_OUTPUT(ED_DEST, "将目前在记忆体里编辑的档案新增到 [档案] 的后面，若无指定则\n");
        ED_OUTPUT(ED_DEST, "新增至正在编辑的档案。\n");
        break;
#endif                          /* RESTRICTED_ED */
    default:
        ED_OUTPUT(ED_DEST, "             Ed 使用说明  (V 2.0)\n");
        ED_OUTPUT(ED_DEST, "---------------------------------\n");
        ED_OUTPUT(ED_DEST, "            Qixx [更新: 07/10/91]\n");
        ED_OUTPUT(ED_DEST, "         中文化作者: Cwlin & Ekac\n");
        ED_OUTPUT(ED_DEST, "       [最后更新日期: 11/08/2002]\n");
        ED_OUTPUT(ED_DEST, "\n\n指令\n--------\n");
        ED_OUTPUT(ED_DEST, "/\t向前搜寻 <样式>\n");
        ED_OUTPUT(ED_DEST, "?\t向后搜寻 <样式>\n");
        ED_OUTPUT(ED_DEST, "=\t显示所在行数\n");
        ED_OUTPUT(ED_DEST, "a\t插入文字于此行之后\n");
        ED_OUTPUT(ED_DEST, "A\t类似 'a', 但使用反向缩排模式\n");
        ED_OUTPUT(ED_DEST, "c\t更改该行\n");
        ED_OUTPUT(ED_DEST, "d\t删除该行\n");
#ifdef RESTRICTED_ED
        if (!P_RESTRICT) {
                ED_OUTPUT(ED_DEST, "e\t使用其他档案取代此档\n");
                ED_OUTPUT(ED_DEST, "E\t同 'e', 但忽略尚未储存的变动\n");
                ED_OUTPUT(ED_DEST, "f\t显示/更改 目前档案名称\n");
        }
#endif                          /* restricted ed */
        ED_OUTPUT(ED_DEST, "g\t搜寻并显示符合样式的行数\n");
        ED_OUTPUT(ED_DEST, "h\t说明档案 (即此说明讯息)\n");
        ED_OUTPUT(ED_DEST, "i\t于所在行数之前插入文字\n");
        ED_OUTPUT(ED_DEST, "I\t缩排所有档案内容 (Qixx 版本 1.0)\n");
    ED_OUTPUT(ED_DEST, "j\t合并范围内的那几行成一行\n");
    ED_OUTPUT(ED_DEST, "k\t标记某行，用 '[标记] 可返回此行\n");
    ED_OUTPUT(ED_DEST, "l\t显示列中的控制字元\n");
    ED_OUTPUT(ED_DEST, "m\t移动范围内容到指定行数之后\n");
    ED_OUTPUT(ED_DEST, "n\t设定是否显示行数\n");
    ED_OUTPUT(ED_DEST, "O\t同 'i'\n");
    ED_OUTPUT(ED_DEST, "o\t同 'a'\n");
    ED_OUTPUT(ED_DEST, "p\t显范围内的所有行\n");
    ED_OUTPUT(ED_DEST, "q\t离开编辑器\n");
    ED_OUTPUT(ED_DEST, "Q\t离开编辑器，忽略尚未储存的修改\n");
    ED_OUTPUT(ED_DEST, "r\t读入档案并插入所在行之后\n");
    ED_OUTPUT(ED_DEST, "s\t搜寻与取代\n");
    ED_OUTPUT(ED_DEST, "set\t查询, 更改或储存选项设定\n");
    ED_OUTPUT(ED_DEST, "t\t复制列到指定位址\n");
    ED_OUTPUT(ED_DEST, "v\t搜寻并显示不符合样式的行数\n");
    ED_OUTPUT(ED_DEST, "x\t储存档案并离开\n");
#ifdef RESTRICTED_ED
        if (!P_RESTRICT) {
        ED_OUTPUT(ED_DEST, "w\t将目前编辑结果写入目前档案或指定档案\n");
        ED_OUTPUT(ED_DEST, "W\t将目前编辑结果新增至目前档案或指定档案的后面\n");
        }
#endif                          /* restricted ed */
    ED_OUTPUT(ED_DEST, "z\t显示 20 行, . + - 为可用的范围参数\n");
    ED_OUTPUT(ED_DEST, "Z\t显示 40 行, . + - 为可用的范围参数\n");
    ED_OUTPUT(ED_DEST, "\n欲取得某指令更详细的说明请输入 h[指令]。\n");

#ifdef RECEIVE_ED
        copy_and_push_string(edout);
        push_undefined();

        ret = apply(APPLY_RECEIVE_ED, ED_DEST, 2, ORIGIN_DRIVER);
        if(!ret)
          outstr = edout;
        else if(ret->type == T_NUMBER) {
      // if 0, output it ourselves, else they handled it, we do nothing.
          if(ret->u.number == 0) // "pass"
            outstr = edout;
          else
            outstr = (char *) 0;
        }
        else if(ret->type == T_STRING) {
          outstr = (char *) ret->u.string;
        }
        else if(ret->type == T_ARRAY) {
          outstr = (char *) 0;

          curp = P_HELPOUT = (struct strlst *) DMALLOC(sizeof(struct strlst),
                                                   TAG_TEMPORARY, "ed: help");
          curp->screen = 0;
          curp->next = 0;

          for(i=0; i < ret->u.arr->size; i++) {
            if(ret->u.arr->item[i].type != T_STRING)
              continue; // dumb.  just skip.

            curp->next = (struct strlst *) DMALLOC(sizeof(struct strlst),
                                                   TAG_TEMPORARY, "ed: help");
            curp = curp->next;

            curp->next = 0;
            curp->screen = DMALLOC(sizeof(char) *
                                   (strlen(ret->u.arr->item[i].u.string) + 1),
                                   TAG_TEMPORARY, "ed: help");
            strcpy(curp->screen, ret->u.arr->item[i].u.string);
          }

          curp = P_HELPOUT;
          P_HELPOUT = P_HELPOUT->next;
          FREE(curp);
        }
        else {
            outstr = edout; // failsafe if they sent something dumb.
        }
#else
        outstr = edout;
#endif

        if(outstr) { // We need to figure out the paging
          char tmpc;
          int lines = 0;

          curp = P_HELPOUT = (struct strlst *) DMALLOC(sizeof(struct strlst),
                                                   TAG_TEMPORARY, "ed: help");
          curp->next = 0;

          brkpt = outstr;

          /* This will miss a newline at the start of the output... wah */
          while(*brkpt != '\0') {
            brkpt++;

            if((*brkpt == '\n') || (*brkpt == '\0')) {
              lines++;

              if((lines == (ED_BUFFER->scroll_lines - 1)) ||
                 (*brkpt == '\0')) {
                if(*brkpt == '\0')
                  tmpc = '\0';
                else {
                  tmpc = *(brkpt+1); // the one past the newline
                  *(brkpt+1) = '\0';
                }

                curp->next = (struct strlst *) DMALLOC(sizeof(struct strlst),
                                                TAG_TEMPORARY, "ed: help");
                curp = curp->next;
                curp->screen = (char *)DMALLOC(sizeof(char) * (strlen(outstr) + 1),
                                   TAG_TEMPORARY, "ed: help");
                strcpy(curp->screen, outstr);

                curp->next = 0;

                if(tmpc != '\0') {
                  *(brkpt+1) = tmpc;
                  outstr = brkpt+1;
                }
                lines = 0;
              }
            }
            //else
            //  brkpt++;
          };

          /* free the dead head */
          curp = P_HELPOUT;
          P_HELPOUT = P_HELPOUT->next;
          FREE(curp);
        }

        if(P_HELPOUT)
          print_help2();
    }
}

static void print_help2()
{
    struct strlst *curp;

    ED_OUTPUT(ED_DEST, P_HELPOUT->screen);

    curp = P_HELPOUT;
    P_HELPOUT = P_HELPOUT->next;
    FREE(curp->screen);
    FREE(curp);

    if(P_HELPOUT) {
      ED_OUTPUT(ED_DEST, "--请输入 [ENTER] 继续--");
      P_MORE = 1;
    }
    else
      P_MORE = 0;
}
#endif                          /* ED */

/****************************************************************
  Stuff below here is for the new ed() interface. -Beek
 ****************************************************************/

#ifndef OLD_ED
static char *object_ed_results() {
    char *ret;

    outbuf_fix(&current_ed_results);
    ret = current_ed_results.buffer;
    outbuf_zero(&current_ed_results);

    return ret;
}

static ed_buffer_t *ed_buffer_list;

static ed_buffer_t *add_ed_buffer (object_t * ob) {
    ed_buffer_t *ret;

    ret = ALLOCATE(ed_buffer_t, TAG_ED, "ed_start: ED_BUFFER");
    memset((char *)ret, '\0', sizeof(ed_buffer_t));

    ob->flags |= O_IN_EDIT;

    ret->owner = ob;
    ret->next_ed_buf = ed_buffer_list;
    ed_buffer_list = ret;
    current_editor = ob;
    return ret;
}

ed_buffer_t *find_ed_buffer (object_t * ob) {
    ed_buffer_t *p;

    for (p = ed_buffer_list; p && p->owner != ob; p = p->next_ed_buf)
        ;
    return p;
}

static void object_free_ed_buffer() {
    ed_buffer_t **p = &ed_buffer_list;
    ed_buffer_t *tmp;

    while ((*p)->owner != current_editor)
        p = &((*p)->next_ed_buf);

    tmp = *p;
    *p = (*p)->next_ed_buf;
    FREE(tmp);

    current_editor->flags &= ~O_IN_EDIT;
}

char *object_ed_start (object_t * ob, const char *fname, int restricted,
                       int scroll_lines) {
    svalue_t *setup;

    /* ensure that the result buffer is initialized */
    outbuf_zero(&current_ed_results);

    regexp_user = ED_REGEXP;
    current_ed_buffer = add_ed_buffer(ob);

    ED_BUFFER->flags |= EIGHTBIT_MASK;
    ED_BUFFER->shiftwidth = ED_INDENT_SPACES;

    push_object(current_editor);
    setup = apply_master_ob(APPLY_RETRIEVE_ED_SETUP, 1);
    if (setup && setup != (svalue_t *)-1 && setup->type == T_NUMBER && setup->u.number) {
        ED_BUFFER->flags = setup->u.number & ALL_FLAGS_MASK;
        ED_BUFFER->shiftwidth = setup->u.number & SHIFTWIDTH_MASK;
    }
    ED_BUFFER->CurPtr = &ED_BUFFER->Line0;

    if (restricted) {
        P_RESTRICT = 1;
    }

    if(scroll_lines)
        ED_BUFFER->scroll_lines = scroll_lines;
    else
        ED_BUFFER->scroll_lines = 20;

    set_ed_buf();

    /*
     * Check for read on startup, since the buffer is read in. But don't
     * check for write, since we may want to change the file name.
     */
    if (fname
        && (fname =
            check_valid_path(fname, current_editor,
                             "ed_start", 0))
        && !doread(0, fname)) {
        setCurLn(1);
    }
    if (fname) {
        strncpy(P_FNAME, fname, MAXFNAME - 1);
        P_FNAME[MAXFNAME - 1] = 0;
    } else {
        ED_OUTPUT(ED_DEST, "No file.\n");
    }
    return object_ed_results();
}

void object_save_ed_buffer (object_t * ob)
{
    svalue_t *stmp;
    const char *fname;

    regexp_user = ED_REGEXP;
    current_ed_buffer = find_ed_buffer(ob);
    current_editor = ob;

    copy_and_push_string(P_FNAME);
    stmp = apply_master_ob(APPLY_GET_ED_BUFFER_SAVE_FILE_NAME, 1);
    if (stmp && stmp != (svalue_t *)-1) {
        if (stmp->type == T_STRING) {
            fname = stmp->u.string;
            if (*fname == '/')
                fname++;
            dowrite(1, P_LASTLN, fname, 0);
        }
    }
    free_ed_buffer(current_editor);
    outbuf_fix(&current_ed_results);
    if (current_ed_results.buffer)
        FREE_MSTR(current_ed_results.buffer);
    outbuf_zero(&current_ed_results);
}

int object_ed_mode (object_t * ob) {
    regexp_user = ED_REGEXP;
    current_ed_buffer = find_ed_buffer(ob);
    current_editor = ob;

    if (P_MORE)
        return -2;
    if (P_APPENDING)
        return P_CURLN + 1;

    return 0;
}

char *object_ed_cmd (object_t * ob, const char *str)
{
    int status = 0;

    regexp_user = ED_REGEXP;
    current_ed_buffer = find_ed_buffer(ob);
    current_editor = ob;

    if (P_MORE) {
        print_help2();
        return object_ed_results();
    }
    if (P_APPENDING) {
        more_append(str);
        return object_ed_results();
    }

    strncpy(inlin, str, ED_MAXLINE - 2);
    inlin[ED_MAXLINE - 2] = 0;
    strcat(inlin, "\n");

    inptr = inlin;
    if ((status = getlst()) >= 0 || status == NO_LINE_RANGE) {
        if ((status = ckglob()) != 0) {
            if (status >= 0 && (status = doglob()) >= 0) {
                setCurLn(status);
                return object_ed_results();
            }
        } else {
            if ((status = docmd(0)) >= 0) {
                if (status == 1)
                    doprnt(P_CURLN, P_CURLN);
                return object_ed_results();
            }
        }
    }
    report_status(status);
    return object_ed_results();
}
#endif

