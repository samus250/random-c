#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef double temptype;

// ask the input
temptype ask_temp(void) {
    char temp[32];
    memset(temp, '\0', 32);
    
    printf("Enter the temperature to convert: ");
    scanf("%s", &temp);
    return atof(temp);
}

// all the conversions
temptype K_C(temptype Ktemp) {
    return Ktemp - 273.15;
}

temptype K_F(temptype Ktemp) {
    return Ktemp * 9/5 - 459.67;
}

temptype K_R(temptype Ktemp) {
    return Ktemp * 9/5;
}

temptype C_K(temptype Ctemp) {
    return Ctemp + 273.15;
}

temptype C_F(temptype Ctemp) {
    return Ctemp * 9/5 + 32;
}

temptype C_R(temptype Ctemp) {
    return (Ctemp + 273.15) * 9/5;
}

temptype F_K(temptype Ftemp) {
    return (Ftemp + 459.67) * 5/9;
}

temptype F_C(temptype Ftemp) {
    return (Ftemp - 32) * 5/9;
}

temptype F_R(temptype Ftemp) {
    return Ftemp + 459.67;
}

temptype R_K(temptype Rtemp) {
    return Rtemp * 5/9;
}

temptype R_C(temptype Rtemp) {
    return (Rtemp - 491.67) * 5/9;
}

temptype R_F(temptype Rtemp){
    return Rtemp - 459.67;
}

int main(void) {
    int rule = 0;
    temptype temp = 0;
    char cont[4];
    memset(cont, '\0', 4);
    
    printf("Welcome to tempconvert!\n\n");
    start:
    
    // first of all, show all the menu and shit
    printf(
        "Please choose the scale of your current datum.\n"
        "1)Kelvin\n"
        "2)Celsius\n"
        "3)Fahrenheit\n"
        "4)Rankine\n\n"
    );
    printf(
        "Please choose the scale you want to convert it to.\n"
        "5)Kelvin\n"
        "6)Celsius\n"
        "7)Fahrenheit\n"
        "8)Rankine\n\n"
    );
    printf("To exit type \"0\".\n\n");
    printf("Now type both selections in order (least to greatest) and press ENTER.\n");
    
    // get the selection
    scanf("%d", &rule);
    
    // check the selection
    switch(rule) {
        case 0:
            return 0;
            break;
        case 16: // K to C
            temp = ask_temp();
            // insert here any way to check for the temp entered
            printf("%.2f K in C is %.2f\n", temp, K_C(temp));
            break;
        case 17: // K to F
            temp = ask_temp();
            printf("%.2f K in F is %.2f\n", temp, K_F(temp));
            break;
        case 18: // K to R
            temp = ask_temp();
            printf("%.2f K in R is %.2f\n", temp, K_R(temp));
            break;
        case 25: // C to K
            temp = ask_temp();
            printf("%.2f C in K is %.2f\n", temp, C_K(temp));
            break;
        case 27: // C to F
            temp = ask_temp();
            printf("%.2f C in ºF is %.2f\n", temp, C_F(temp));
            break;
        case 28: // C to R
            temp = ask_temp();
            printf("%.2f C in R is %.2f\n", temp, C_R(temp));
            break;
        case 35: // F to K
            temp = ask_temp();
            printf("%.2f F in K is %.2f\n", temp, F_K(temp));
            break;
        case 36: // F to C
            temp = ask_temp();
            printf("%.2f F in C is %.2f\n", temp, F_C(temp));
            break;
        case 38: // F to R
            temp = ask_temp();
            printf("%.2f F in R is %.2f\n", temp, F_R(temp));
            break;
        case 45: // R to K
            temp = ask_temp();
            printf("%.2f R in K is %.2f\n", temp, R_K(temp));
            break;
        case 46: // R to C
            temp = ask_temp();
            printf("%.2f R in C is %.2f\n", temp, R_C(temp));
            break;
        case 47: // R to F
            temp = ask_temp();
            printf("%.2f R in F is %.2f\n", temp, R_F(temp));
            break;
        default: // not valid
            printf("Not a valid option. Bye.\n");
            break;
        
    }
    
    printf("Convert something else? (Y/N): ");
    scanf("%3s", &cont);
    
    if(cont[0] == 'y' || cont[0] == 'Y') {
        rule = temp = 0;
        memset(cont, '\0', 4);
        putchar('\n');
        goto start;
    }
    
    return 0;
}

