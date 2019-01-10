#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Cfg {
    char *prefix;
    char *name;
    int debug;
};

struct Cfg gcfg = {};

int init_cfg() {
    gcfg.prefix = "";
    gcfg.name = "";
    gcfg.debug = 0;
}

int main(int argc, char * argv[]) {
    printf("tester\n");
    printf("optind: %d, operr: %d\n", optind, opterr);
    init_cfg();

    int ch;
    //无":" 参数         1个":" 先选参数         2个":" 可选参数
    while((ch = getopt(argc, argv, "p:n:d::")) != -1) {
        switch(ch) {
        case 'p':
            printf("option p\n");
            printf("the args[p] value is: %s\n", optarg);
            gcfg.prefix = optarg;
            break;
        case 'n':
            printf("option n\n");
            printf("the args[n] value is: %s\n", optarg);
            gcfg.name = optarg;
            break;
        case 'd':
            printf("option d\n");
            gcfg.debug = 1;
            break;
        case '?':
            printf("unknown option: %c\n", (char)optopt);
            break;
        }
    }

    printf("cfg.prefix %s\n", gcfg.prefix);
    printf("cfg.name %s\n",   gcfg.name);
    printf("cfg.debug %d\n",  gcfg.debug);
    return 0;
}

