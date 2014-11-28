
#include "./logger.h"


//###### main ########
int main(int argc, const char * argv[]) {

    printf("Test: start main\n");
    initLog();
    logPrnt('g','u', "test in grün\n");
    logPrnt('r','e', "test perror\n");

    
    return 0;
}
