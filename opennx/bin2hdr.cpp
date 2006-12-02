#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>


#define MAXARR 0x00010000

static void usage()
{
    fprintf(stderr, "Usage: bin2hdr <infile> <outfile> <identifier>\n");
    exit(1);
}

int main(int argc, char **argv) {
    long isize = -1;
    unsigned char buf[1024];

    if ((argc < 4) || (strlen(argv[1]) < 1) ||
        (strlen(argv[2]) < 1) || (strlen(argv[3]) < 1))
        usage();

    if (strcmp(argv[1], "-") != 0) {
        struct stat st;
        if (stat(argv[1], &st) != 0) {
            perror(argv[1]);
            exit(-1);
        }
        isize = st.st_size;
    }
    FILE *fi = (strcmp(argv[1], "-") == 0) ? stdin : fopen(argv[1], "rb");
    char *ident = argv[3];
    if (!fi) {
        perror(argv[1]);
        exit(-1);
    }
    FILE *fo = (strcmp(argv[2], "-") == 0) ? stdout :fopen(argv[2], "w");
    if (!fo) {
        perror(argv[2]);
        exit(-1);
    }
    bool hdr = false;
    bool fhdr = false;
    int n = 0;
    int i;

    if (stdout == fo)
        fprintf(fo, "#ifndef _BIN2HDR_STDOUT_H_\n#define _BIN2HDR_STDOUT_H_\n");
    else {
        char *p = strrchr(argv[2], '\\');
        if (p == NULL)
            p = strrchr(argv[2], '/');
        strcpy((char *)buf, "_");
        if (p)
            strcat((char *)buf, p);
        else
            strcat((char *)buf, argv[2]);
        strcat((char *)buf, "_");
        for (i = 0; i < (int)strlen((char *)buf); i++) {
            if (!iscsym(buf[i]))
                buf[i] = '_';
        }
        fprintf(fo, "#ifndef %s\n#define %s\n", buf, buf);
        if (isize > MAXARR)
            fprintf(fo, "#include <stdlib.h>\n#include <string.h>\n");
    }
    long bcount = 0;
    long bssize = (isize > MAXARR) ? MAXARR : isize;
    while ((i = fread(buf, 1, sizeof(buf), fi)) > 0) {
        if (!hdr) {
            fprintf(fo, "static const unsigned long cnt_%s = %ld;\n", ident, isize);
            fprintf(fo, "static const unsigned char %s%ld[] = {\n", ident, bcount++);
            hdr = true;
        }
        for (int j = 0; j < i; j++) {
            fprintf(fo, "%s0x%02x,%c", (n % 16) ? "" : "    ", buf[j], ((n + 1) %16) ? ' ' : '\n');
            n++;
        }
        if ((n % bssize) == 0) {
            fprintf(fo, "%s};\n", (n % 16) ? "\n" : "");
            fprintf(fo, "static const unsigned char %s%ld[] = {\n", ident, bcount++);
        }
    }
    if (n) {
        fprintf(fo, "%s};\n\n", (n % 16) ? "\n" : "");
        if (bcount == 1) {
            fprintf(fo, "static const unsigned char *get_%s() { return %s; }\n", ident, ident);
            fprintf(fo, "static void free_%s(const unsigned char *) { }\n", ident);
        } else {
            long bi;
            fprintf(fo, "static const unsigned char *get_%s() {\n", ident);
            fprintf(fo, "    const unsigned char *ret = (const unsigned char *)malloc(%ld);\n", bssize * bcount);
            for (bi = 0; bi < bcount; bi++)
                fprintf(fo, "    memcpy((void *)(ret + %ld), %s%ld, %ld);\n", bssize * bi, ident, bi, bssize);
            fprintf(fo, "    return ret;\n}\n");
            fprintf(fo, "static void free_%s(const unsigned char *ptr) { free((void *)ptr); }\n", ident);
        }
        fprintf(fo, "#endif\n");
    }
    fclose(fi);
    fclose(fo);
    return 0;
}