#include <stdio.h>


float fromTo (char in, char out, float par) {
    float parOut;
    switch (in) {
        case 'C': switch (out){
                    case 'C' : parOut =  par; break;
                    case 'D' : parOut =   (100-par)*1.5; break;
                    case 'F' : parOut =   par*1.8+32;    break;
                    case 'K' : parOut =   par+273.15; break;
                    case 'R' : parOut =   par*1.8+491.67; break;
                    default :  parOut =  (float) -1; break; }
        break;
        case 'D': switch (out){
                    case 'C' : parOut =   100-par*2/3; break;
                    case 'D' : parOut =   par; break;
                    case 'F' : parOut =   212 - par*1.2;    break;
                    case 'K' : parOut =   373.15 - par*2/3; break;
                    case 'R' : parOut =   671.67 - par*1.2; break;
                    default : parOut =  (float) -1; break; }
        break;
        case 'F': switch (out){
                    case 'C' : parOut =   (par-32)*5/9; break;
                    case 'D' : parOut =   (212-par)*5/6; break;
                    case 'F' : parOut =   par;    break;
                    case 'K' : parOut =   (par+459.67)*5/9; break;
                    case 'R' : parOut =   par + 459.67; break;
                    default :  parOut =  (float) -1; break; }
        break;
        case 'K': switch (out){
                    case 'C' : parOut =   par - 275.15; break;
                    case 'D' : parOut =   (373.15 - par) * 1.5; break;
                    case 'F' : parOut =   par * 1.8 - 459.67;    break;
                    case 'K' : parOut =   par; break;
                    case 'R' : parOut =   par*1.8; break;
                    default : parOut =  (float) -1; break; }
        break;
        case 'R': switch (out){
                    case 'C' : parOut =   par*5/9-273.15; break;
                    case 'D' : parOut =   (671.67 - par)*5/6; break;
                    case 'F' : parOut =   par-459.67;    break;
                    case 'K' : parOut =   par*5/9; break;
                    case 'R' : parOut =   par; break;
                    default :  parOut =   -1; break;}
        break;    
        default : parOut = (float)-1; break;
    }
    return parOut;
}


int main(int argc, char *argv[]){


    char in, out;
    float parIn;
    float parOut;
    

    printf ("Ihre möglichen Temperatureinheiten: \n (C)elsius \n (D)elisle \n (F)ahrenheit \n (K)elvin \n (R)ankine \n");
    printf("Bitte geben Sie die Einheit der Temperatur ein: ");
    do {scanf("%c",&in);} while (getchar() != '\n'); 

    printf("Bitte geben Sie die gewünschte Einheit der Temperatur ein: ");
    do {scanf("%c",&out);} while (getchar() != '\n');

    printf("Bitte geben Sie die Temperatur ein: ");
    do {scanf("%f",&parIn);} while (getchar() != '\n');
    
/*    printf("Die Temperatur von der sie umrechnen wollen ist %c.\n",in);
    printf("Die Temperatur in die sie umrechnen wollen ist %c.\n",out);
    printf("Die Temperatur die sie eingegeben haben ist %i.\n", parIn);
*/
    parOut = fromTo (in, out, parIn);
    
    printf("Die Temperatur lautet: %f %c. \n", parOut, out);

    return 0;
}















