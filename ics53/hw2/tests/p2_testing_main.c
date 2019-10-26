#include "hw2_p2.h"

int main() {
    //char* toSubstring = "-1 22.3       +";
    char* toSubstring = "-19 22.3       +";
    char* toSubstringDisplay = toSubstring;
    Token* token = tokenizeCalc(&toSubstring);
    printf("tokenizeCalc(&\"%s\") returns a token whose value is %d "
        "(should be -1) and type is %d (should be 1)\n", toSubstringDisplay,
        *((int*) token->datavalue), token->type);
    
    token = tokenizeCalc(&toSubstring);
    printf("tokenizeCalc(&\"%s\") returns a token whose value is %f "
        "(should be -1) and type is %d (should be 1)\n", toSubstringDisplay,
        *((float*) token->datavalue), token->type);
    
    
    
    return 0;
}
