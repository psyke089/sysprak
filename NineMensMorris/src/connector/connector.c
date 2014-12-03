#include "connector.h"

char id[11];

/**
 * dummy main damit der connector alleine
 * compiled werden kann (make connector);
 */

int main(int argc, char const *argv[])
{

  if( argv[1] == '\0' ) {
    printf("Bitte Spiel-ID angeben.\n");
    exit(1);
  }

  sprintf(id, "%s", argv[1]);

  performConnection();

  return 0;
}
