// function declarations
int main(int var0, char** var1);
extern unsigned long strtol(unsigned char* var0, unsigned char** var1, unsigned int var2);

int main(int var0, char** var1){
    unsigned int var2;
    unsigned int argc;
    unsigned char** argv;
    unsigned char* p;
    unsigned long num;
    block0: ;
    var2 = 0;
    argc = var0;
    argv = var1;
    if (argc != 2) {
        var2 = -1;
        return var2;
    } else {
        num = strtol(*(((unsigned char**)(argv)) + 1), &p, 10);
        if (((int)(*p)) != 0) {
            var2 = -1;
            return var2;
        } else {
            if (((long)num) > ((long)10)) {
                if (((long)num) > ((long)5)) {
                    var2 = 2;
                    return var2;
                } else {
                    var2 = 1;
                    return var2;
                }
            } else {
                if (((long)num) < ((long)-5)) {
                    var2 = -2;
                    return var2;
                } else {
                    var2 = -1;
                    return var2;
                }
            }
        }
    }
}

